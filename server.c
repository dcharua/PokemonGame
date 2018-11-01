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
  int online;
  int ready;
  int turn;
  pokemon_t * pokemon;
} player_t;

// Structure for the mutexes to keep the data consistent
typedef struct locks_struct {
  // Mutex array for the players data
  pthread_mutex_t * players_mutex;
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
void closeGame(player_t * player1, player_t * player2, locks_t * data_locks);
void catchInterrupt(int signal);

///// MAIN FUNCTION
int main(int argc, char * argv[]){
  int server_fd;
  player_t player1, player2;
  locks_t data_locks;

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
  // Allocate the arrays for the mutexes
  data_locks->players_mutex = malloc(MAX_PLAYERS * sizeof (pthread_mutex_t));

  // Initialize the mutexes, using a different method for dynamically created ones
  for (int i=0; i<MAX_PLAYERS; i++){
    pthread_mutex_init(&data_locks->players_mutex[i], NULL);
  }
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

  while (!interrupted && players < MAX_PLAYERS ){
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
        players++;

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
  //char buffer[BUFFER_SIZE];
  getPlayerData(connection_data);
  while(connection_data->player1->ready == 0 || connection_data->player2->ready == 0){}
  battle(connection_data);
  //free connection memory
  free(connection_data);
  //close the thread
  pthread_exit(NULL);
}

void getPlayerData(thread_data_t * connection_data){
  char buffer[BUFFER_SIZE];
  // Receive the request
  //if massage was not recived succesfully from client exit
  if (!recvString(connection_data->connection_fd, buffer, BUFFER_SIZE)){
    perror("Message recive");
    exit(EXIT_FAILURE);
  }
  if(connection_data->player1->online == 0){
    // Process the request being careful of data consistency
    sscanf(buffer, "%s %s %f %d %f %f %d", connection_data->player1->name, connection_data->player1->pokemon->name, &connection_data->player1->pokemon->HP, &connection_data->player1->pokemon->MP, &connection_data->player1->pokemon->attack1, &connection_data->player1->pokemon->attack2, &connection_data->player1->pokemon->attack_percent);
    //printf("%s %s %f %d %f %f %d\n", connection_data->player1->name, connection_data->player1->pokemon->name, connection_data->player1->pokemon->HP, connection_data->player1->pokemon->MP, connection_data->player1->pokemon->attack1, connection_data->player1->pokemon->attack2, connection_data->player1->pokemon->attack_percent);
    connection_data->player1->online = 1;
    sprintf(buffer, "WAIT");
    sendString(connection_data->connection_fd, buffer);
    while (connection_data->player2->online == 0){}
    sprintf(buffer, "PLAY");
    sendString(connection_data->connection_fd, buffer);
    connection_data->player1->ready = 1;
  } else {
    // Process the request being careful of data consistency
    sscanf(buffer, "%s %s %f %d %f %f %d", connection_data->player2->name, connection_data->player2->pokemon->name, &connection_data->player2->pokemon->HP, &connection_data->player2->pokemon->MP, &connection_data->player2->pokemon->attack1, &connection_data->player2->pokemon->attack2, &connection_data->player2->pokemon->attack_percent);
    connection_data->player2->online = 1;
    sprintf(buffer, "PLAY");
    sendString(connection_data->connection_fd, buffer);
    connection_data->player2->ready = 1;
    //printf("%s %s %f %d %f %f %d\n", connection_data->player2->name, connection_data->player2->pokemon->name, connection_data->player2->pokemon->HP, connection_data->player2->pokemon->MP, connection_data->player2->pokemon->attack1, connection_data->player2->pokemon->attack2, connection_data->player2->pokemon->attack_percent);
  }
  // if(connection_data->player1->ready == 1 && connection_data->player2->ready == 1){
  //   battle(connection_data);
  // }
}

void battle(thread_data_t * connection_data){

  if(connection_data->player_id==1){
    if (!recvString(connection_data->connection_fd, buffer, BUFFER_SIZE)){
      perror("Message recive");
      exit(EXIT_FAILURE);
    }
    if(strncmp(buffer, "READY", 5) == 0){
      sprintf(buffer, "TURN");
      sendString(connection_fd, buffer);
    }

  } else if (connection_data->player_id==2){
    if (!recvString(connection_data->connection_fd, buffer, BUFFER_SIZE)){
      perror("Message recive");
      exit(EXIT_FAILURE);
    }
    if(strncmp(buffer, "READY", 5) == 0){
      sprintf(buffer, "WAIT");
      sendString(connection_fd, buffer);
    }
  }
}

void attack(thread_data_t * connection_data){
	int prob1 = rand() % 100 + 1;
	int prob2 = rand() % 100 + 1;
	int oppAttack1 = rand() % 2 + 1;
	float finalattack1 = opponent->MP * opponent->attack1, finalattack2 = opponent->MP * opponent->attack2, pikachuAttack = (pikachu->MP * attack);

	if (prob1 <= pikachu->attack_percent) {
		opponent->HP -= pikachuAttack;
		printf("\nYou take %.0f of his HP!\n", pikachuAttack);
	} else {
    printf("\nYour attack has failed\n");
  }

  if (prob2 <= opponent->attack_percent) {
		if (oppAttack1 == 1) {
			pikachu->HP -= finalattack1;
			printf("\nThe opponent has taken %.0f of your HP!\n", finalattack1);
		} else if (oppAttack1 == 2) {
			pikachu->HP -= finalattack2;
			printf("\nThe opponent has taken %.0f of your HP!\n", finalattack2);
		}
	} else {
    printf("\nHis attack failed\n");
  }
}

void closeGame(player_t * player1, player_t * player2, locks_t * data_locks){
  printf("DEBUG: Clearing the memory for the thread\n");
  free(player1->pokemon);
  free(player2->pokemon);
  free(data_locks->players_mutex);
}
