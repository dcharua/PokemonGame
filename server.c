//
//  Pokemon.c
//  FinalProyect
//  17/04/15
//  Copyright (c) 2015. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
// Signals library
#include <errno.h>
#include <signal.h>
#include <poll.h>
// Sockets libraries
#include <netdb.h>
#include <sys/poll.h>
// Posix threads library
#include <pthread.h>

// Custom libraries
#include "sockets.h"
#include "fatal_error.h"

#define MAX_PLAYERS 2
#define BUFFER_SIZE 1024
#define MAX_QUEUE 5

///// Structure definitions
int interrupted = 0;
int turn = 1;
float attack_points;
pthread_cond_t player1_turn = PTHREAD_COND_INITIALIZER;
pthread_cond_t player2_turn = PTHREAD_COND_INITIALIZER;

typedef struct pokemon{
  char name[10];
  float HP;
  int MP;
  float attack1;
  float attack2;
  int attack_percent;
} pokemon_t;

typedef struct player{
  char name[10];
  int potions[3];
  int online;
  int ready;
  int turn;
  pokemon_t * pokemon;
} player_t;

// Structure for the mutexes to keep the data consistent
typedef struct locks_struct {
  pthread_mutex_t attack_mutex;
  pthread_mutex_t wait_mutex;
} locks_t;


// Data that will be sent to each structure
typedef struct data_struct {
  // The file descriptor for the socket
  int connection_fd;
  // The file descriptor for the socket
  int player_id;
  // A pointer to a players data
  player_t * player1;
  player_t * player2;
  // A pointer to a locks structure
  locks_t * data_locks;
} thread_data_t;

//
void usage(char * program);
void setupHandlers();
void initGame(player_t * player1, player_t * player2, locks_t * data_locks);
void waitForConnections(int server_fd, player_t * player1, player_t * player2, locks_t * data_locks);
void * attentionThread(void * arg);
void getPlayerData(thread_data_t * connection_data);
void battle(thread_data_t * connection_data);
void sendData(thread_data_t * connection_data);
void reciveAttack(thread_data_t *connection_data);
void attack(thread_data_t * connection_data, char action);
void potion(thread_data_t * connection_data, char action);
void closeGame(player_t * player1, player_t * player2, locks_t * data_locks);
void catchInterrupt(int signal);

///// MAIN FUNCTION
int main(int argc, char * argv[]){
  int server_fd;
  player_t player1, player2;
  locks_t data_locks;
  srand(time(NULL));

  printf("\n=== Pokemon Coliseum Server ===\n");

  // Check the correct arguments
  if (argc != 2)
    usage(argv[0]);

  // Configure the handler to catch SIGINT
  setupHandlers();

  // Initialize the data structures
  initGame(&player1, &player2, &data_locks);

  // Show the IPs assigned to this computer
  printLocalIPs();
  // Start the server
  server_fd = initServer(argv[1], MAX_QUEUE);
  // Listen for connections from the clients
  waitForConnections(server_fd, &player1, &player2, &data_locks);
  // Close the socket
  close(server_fd);
  printf("in\n");
  fflush(stdout);
  // Clean the memory used
  closeGame(&player1, &player2, &data_locks);

  // Finish the main thread
  pthread_exit(NULL);

  return 0;
}

///// FUNCTION DEFINITIONS

/*
    Explanation to the user of the parameters required to run the program
*/
void usage(char * program){
  printf("Usage:\n");
  printf("\t%s {port_number}\n", program);
  exit(EXIT_FAILURE);
}

void setupHandlers(){
  struct sigaction new_action;

  new_action.sa_handler = catchInterrupt;
  sigfillset(&new_action.sa_mask);

  //Define the signal to handle
  sigaction(SIGINT, &new_action, NULL);
}

//Function to act on CNTL C and send exit message to server
void catchInterrupt(int signal){
  interrupted = 1;
  exit(1);
}


/*
    Function to initialize all the information necessary
    This will allocate memory for the accounts, and for the mutexes
*/
void initGame(player_t * player1, player_t * player2, locks_t * data_locks){
  // Allocate the arrays in the structures

  player1->pokemon = malloc(sizeof (pokemon_t));
  player2->pokemon = malloc(sizeof (pokemon_t));
  player1->online = 0;
  player2->online = 0;
  // Initialize mutex
  pthread_mutex_init(&data_locks->attack_mutex, NULL);
  pthread_mutex_init(&data_locks->wait_mutex, NULL);

}

/*
    Main loop to wait for incomming connections
*/
void waitForConnections(int server_fd, player_t * player1, player_t * player2, locks_t * data_locks){
  struct sockaddr_in client_address;
  socklen_t client_address_size;
  char client_presentation[INET_ADDRSTRLEN];
  int client_fd;
  int id = 1;
  pthread_t new_tid;
  thread_data_t * connection_data = NULL;
  int poll_response;
  int timeout = 500;		// Time in milliseconds (0.5 seconds)

  // Get the size of the structure to store client information
  client_address_size = sizeof client_address;

  while (!interrupted){
	//// POLL
    // Create a structure array to hold the file descriptors to poll
    struct pollfd test_fds[1];
    // Fill in the structure
    test_fds[0].fd = server_fd;
    test_fds[0].events = POLLIN;    // Check for incomming data
    // Check if there is any incomming communication
    poll_response = poll(test_fds, 1, timeout);

     // Error when polling
    if (poll_response == -1){
      // Test if the error was caused by an interruption
      if (errno == EINTR){
        //printf("Poll did not finish. The program was interrupted\n");
      }else{
        fatalError("ERROR: poll");
      }
    }
	// Timeout finished without reading anything
    else if (poll_response == 0){
      //printf("No response after %d seconds\n", timeout);
    }
	// There is something ready at the socket
    else{
      // Check the type of event detected
      if (test_fds[0].revents & POLLIN){
  			// ACCEPT
  			// Wait for a client connection
  			client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);
  			if (client_fd == -1)
  				fatalError("ERROR: accept");


        // Get the data from the client
        inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
        printf("Received incomming connection from %s on port %d\n", client_presentation, client_address.sin_port);

  			// Prepare the structure to send to the thread
        connection_data = malloc(sizeof (thread_data_t));
        connection_data->connection_fd = client_fd;
        connection_data->player_id = id++;
        connection_data->player1 = player1;
        connection_data->player2 = player2;
        connection_data->data_locks = data_locks;
        if (id == 3)
          id = 1;

  			//CREATE A THREAD
        int result = pthread_create(&new_tid, NULL, attentionThread, (void*)connection_data);
        if (result == -1){
          perror("pthread_create");
          exit(EXIT_FAILURE);
        }
      }
    }
  }
}

void * attentionThread(void * arg){
  // Receive the data for the bank, mutexes and socket file descriptor
  thread_data_t * connection_data = (thread_data_t *) arg;
  //get the data form the incoming player
  getPlayerData(connection_data);
  //Wait for boyh players to be online before battle
  while(connection_data->player1->ready == 0 || connection_data->player2->ready == 0){}
  //go to battle function
  battle(connection_data);
  //free connection memory
  free(connection_data);
  //close the thread
  pthread_exit(NULL);
}

void getPlayerData(thread_data_t * connection_data){
  char buffer[BUFFER_SIZE];
  getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);
  //if player one is not online, new player is player 1
  if(connection_data->player1->online == 0){
    //get data from the player, set player online and send WAIT signal
    sscanf(buffer, "%s %s %f %d %f %f %d", connection_data->player1->name, connection_data->player1->pokemon->name, &connection_data->player1->pokemon->HP, &connection_data->player1->pokemon->MP, &connection_data->player1->pokemon->attack1, &connection_data->player1->pokemon->attack2, &connection_data->player1->pokemon->attack_percent);
    printf("Player data: %s %s %f %d %f %f %d\n", connection_data->player1->name, connection_data->player1->pokemon->name, connection_data->player1->pokemon->HP, connection_data->player1->pokemon->MP, connection_data->player1->pokemon->attack1, connection_data->player1->pokemon->attack2, connection_data->player1->pokemon->attack_percent);
    connection_data->player1->online = 1;
    sprintf(buffer, "WAIT");
    sendString(connection_data->connection_fd, buffer);
    // while player 2 is not online wait
    while (connection_data->player2->online == 0){}
    //When player 2 is online send PLAY signal and set player 1 to ready
    sprintf(buffer, "PLAY");
    sendString(connection_data->connection_fd, buffer);
    connection_data->player1->ready = 1;
  } else { //Player 1 is already online, new player is player 2
    // get data from the player and send set player online and send PLAY signal and set ready
    sscanf(buffer, "%s %s %f %d %f %f %d", connection_data->player2->name, connection_data->player2->pokemon->name, &connection_data->player2->pokemon->HP, &connection_data->player2->pokemon->MP, &connection_data->player2->pokemon->attack1, &connection_data->player2->pokemon->attack2, &connection_data->player2->pokemon->attack_percent);
    connection_data->player2->online = 1;
    sprintf(buffer, "PLAY");
    sendString(connection_data->connection_fd, buffer);
    connection_data->player2->ready = 1;
    printf("Player 2 data: %s %s %f %d %f %f %d\n", connection_data->player2->name, connection_data->player2->pokemon->name, connection_data->player2->pokemon->HP, connection_data->player2->pokemon->MP, connection_data->player2->pokemon->attack1, connection_data->player2->pokemon->attack2, connection_data->player2->pokemon->attack_percent);
  }
}

void battle(thread_data_t * connection_data){
  turn = 1;
  char buffer[BUFFER_SIZE];
  // id its player 1
  if(connection_data->player_id==1){
    //get OPPONET request form player and send opponent data
    getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);
    if(strncmp(buffer, "OPPONENT", 8) == 0){
      sprintf(buffer, "%s %s %f %d %f %f %d", connection_data->player2->name, connection_data->player2->pokemon->name, connection_data->player2->pokemon->HP, connection_data->player2->pokemon->MP, connection_data->player2->pokemon->attack1, connection_data->player2->pokemon->attack2, connection_data->player2->pokemon->attack_percent);
      sendString(connection_data->connection_fd, buffer);
    }
    //get READY form player and start the battle
    getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);
    if(strncmp(buffer, "READY", 5) == 0){
      //battle until a player HP is 0 or less
      printf("HP1:%f HP2:%f\n", connection_data->player1->pokemon->HP, connection_data->player2->pokemon->HP);
      while(connection_data->player1->pokemon->HP > 0 && connection_data->player2->pokemon->HP > 0){
        pthread_mutex_lock(&connection_data->data_locks->attack_mutex);
          //first is player's 1 turn send TURN signal
          sprintf(buffer, "TURN");
          sendString(connection_data->connection_fd, buffer);
          //recive the attack
          reciveAttack(connection_data);
          //send the result of the attack
          sendData(connection_data);
          turn = 2;
        pthread_mutex_unlock(&connection_data->data_locks->attack_mutex);

        pthread_mutex_lock(&connection_data->data_locks->wait_mutex);
          //Now player 1 waits from player's 1 attack
          sprintf(buffer, "WAIT");
          sendString(connection_data->connection_fd, buffer);
          //wait until player 2 send the attack
          getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);
          printf("turn %d\n", turn );
          while(turn == 2){}
          printf("player 1 done waitig now turn 1\n");
          //send the result of player's 2 attack to player 1
          sendData(connection_data);
        pthread_mutex_unlock(&connection_data->data_locks->wait_mutex);
      }
    }
  } else if (connection_data->player_id==2){
    // if its player 2 get OPPNONENT data request and send it
    getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);
    if(strncmp(buffer, "OPPONENT", 8) == 0){
      sprintf(buffer, "%s %s %f %d %f %f %d", connection_data->player1->name, connection_data->player1->pokemon->name, connection_data->player1->pokemon->HP, connection_data->player1->pokemon->MP, connection_data->player1->pokemon->attack1, connection_data->player1->pokemon->attack2, connection_data->player1->pokemon->attack_percent);
      sendString(connection_data->connection_fd, buffer);
    }
    //Get READY signal
    getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);
    if(strncmp(buffer, "READY", 5) == 0){
      //battle until a players HP is cero or less
      while(connection_data->player1->pokemon-> HP > 0 && connection_data->player2->pokemon-> HP > 0){
        //first is players1 turn so send WAIT signal
        pthread_mutex_lock(&connection_data->data_locks->wait_mutex);

          sprintf(buffer, "WAIT");
          sendString(connection_data->connection_fd, buffer);
          getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);

          printf("turn %d\n", turn );
          //wait until player 1 sends the attack
          while(turn == 1){}
          printf("player 2 done waitig now turn 2\n");
          //send the data from players 1 attack
          sendData(connection_data);
        pthread_mutex_unlock(&connection_data->data_locks->wait_mutex);

        pthread_mutex_lock(&connection_data->data_locks->attack_mutex);
          //now its player's 2 turn
          sprintf(buffer, "TURN");
          sendString(connection_data->connection_fd, buffer);

          //get the attack from player 2
          reciveAttack(connection_data);
          //send the result of the attack
          sendData(connection_data);
          turn = 1;
        pthread_mutex_unlock(&connection_data->data_locks->attack_mutex);


      }
    }
  }
  //battle id over send END signal
  sprintf(buffer, "END");
  sendString(connection_data->connection_fd, buffer);
}

void sendData(thread_data_t * connection_data){
  char buffer[BUFFER_SIZE];
  if(connection_data->player_id==1){
    if (connection_data->player2->online == 1){
    printf("sending data player1\n");
    sprintf(buffer, "%f %f %f",  attack_points, connection_data->player1->pokemon->HP, connection_data->player2->pokemon->HP);
    sendString(connection_data->connection_fd, buffer);
    }else{
      sprintf(buffer, "END");
      sendString(connection_data->connection_fd, buffer);
    }
  } else if (connection_data->player_id==2){
    if (connection_data->player1->online == 1){
      printf("sending data player 2\n");
      sprintf(buffer, "%f %f %f", attack_points, connection_data->player2->pokemon->HP, connection_data->player1->pokemon->HP);
      sendString(connection_data->connection_fd, buffer);
    } else{
      sprintf(buffer, "END");
      sendString(connection_data->connection_fd, buffer);
    }
  }
}

void reciveAttack(thread_data_t *connection_data){
  char buffer[BUFFER_SIZE];
  char action;
  getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);
  sscanf(buffer, "%c", &action);
  printf("action %c\n", action);
  if (action == '1' || action == '2')
    attack(connection_data, action);
  if (action == '3' || action == '4' || action == '5')
    potion(connection_data, action);
}

void attack(thread_data_t * connection_data, char action){
	int prob = rand() % 100 + 1;
  if(connection_data->player_id==1){
    //pthread_mutex_lock(&connection_data->data_locks->attack_mutex);
      if (action == '1')
        attack_points = connection_data->player1->pokemon->MP * connection_data->player1->pokemon->attack1;
      if (action == '2')
        attack_points = connection_data->player1->pokemon->MP * connection_data->player1->pokemon->attack2;

      printf("prob %d, attack %d \n", prob, connection_data->player1->pokemon->attack_percent);
    	if (prob <= connection_data->player1->pokemon->attack_percent) {
    		connection_data->player2->pokemon->HP -= attack_points;
    		printf("\nPlayer 1 takes %f of Player 2 HP!\n", attack_points);
    	} else {
        printf("\nYour attack has failed\n");
        attack_points = 0.0;
      }
    //pthread_mutex_unlock(&connection_data->data_locks->attack_mutex);
  } else if(connection_data->player_id==2){
    //pthread_mutex_lock(&connection_data->data_locks->attack_mutex);
      if (action == '1')
        attack_points = connection_data->player2->pokemon->MP * connection_data->player2->pokemon->attack1;
      if (action == '2')
        attack_points = connection_data->player2->pokemon->MP * connection_data->player2->pokemon->attack2;

      printf("Player 2 prob %d, attack %d \n", prob, connection_data->player1->pokemon->attack_percent);
    	if (prob <= connection_data->player2->pokemon->attack_percent) {
    		connection_data->player1->pokemon->HP -= attack_points;
        printf("\nPlayer 2 takes %f of Player 1 HP!\n",attack_points);
    	} else {
        printf("\nYour attack has failed\n");
        attack_points = 0.0;
      }
    //pthread_mutex_unlock(&connection_data->data_locks->attack_mutex);
  }
}

void potion(thread_data_t * connection_data, char action){
  if(connection_data->player_id==1){
    if (action == '3'){
      connection_data->player1->pokemon->HP += 30;
    } else if (action == '4'){
      connection_data->player1->pokemon->MP += 5;
    } else if( action == '5'){
      connection_data->player1->pokemon->attack_percent += 5;
    }
  } else if(connection_data->player_id==2){
    if (action == '3'){
      connection_data->player2->pokemon->HP += 30;
    } else if (action == '4'){
      connection_data->player2->pokemon->MP += 5;
    } else if( action == '5'){
      connection_data->player2->pokemon->attack_percent += 5;
    }
  }
}


void closeGame(player_t * player1, player_t * player2, locks_t * data_locks){
  printf("DEBUG: Clearing the memory for the thread\n");
  free(player1->pokemon);
  free(player2->pokemon);
}
