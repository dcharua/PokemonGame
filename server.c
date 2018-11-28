/*
  PokemonServer.c
  FinalProyect
  24/11/18
  Daniel Charua - Joan Andoni
  Copyright (c) 2018. All rights reserved.
*/


#include "server.h"

///// MAIN FUNCTION
int main(int argc, char * argv[]){
    //declare the variables and seed the randmon factor
    int server_fd;
    player_t player1, player2;
    locks_t data_locks;
    srand(time(NULL));

    printf("\n=== Pokemon Coliseum Server ===\n");

    // Check the correct arguments
    if (argc != 2) {
        usage(argv[0]);
    }

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


//Explanation to the user of the parameters required to run the program
void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}

//Setting up handlers for interruption
void setupHandlers()
{
    struct sigaction new_action;
    new_action.sa_handler = catchInterrupt;
    sigfillset(&new_action.sa_mask);
    // Define the signal to handle
    sigaction(SIGINT, &new_action, NULL);
}

// Function to act on CNTL C
void catchInterrupt(int signal)
{
    printf("Closing server....\n");
    interrupted = 1;
    usleep(1000000);
    exit(1);
}


//Function to allocate memory and initialize functions
void initGame(player_t * player1, player_t * player2, locks_t * data_locks)
{
    // Allocate the arrays in the structures
    player1->pokemon = malloc(sizeof (pokemon_t));
    player2->pokemon = malloc(sizeof (pokemon_t));
    player1->online = 0;
    player2->online = 0;

    // Initialize mutex
    pthread_mutex_init(&data_locks->attack_mutex, NULL);
    pthread_mutex_init(&data_locks->wait_mutex, NULL);
}

//Main loop to wait for incomming connections
void waitForConnections(int server_fd, player_t * player1, player_t * player2, locks_t * data_locks)
{
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

    while (!interrupted) {
        /*---------- POLL ----------*/
        // Create a structure array to hold the file descriptors to poll
        struct pollfd test_fds[1];

        // Fill in the structure
        test_fds[0].fd = server_fd;
        test_fds[0].events = POLLIN;    // Check for incomming data

        // Check if there is any incomming communication
        poll_response = poll(test_fds, 1, timeout);

        // Error when polling
        if (poll_response == -1) {
            // Test if the error was caused by an interruption
            if (errno == EINTR) {
                //printf("Poll did not finish. The program was interrupted\n");
            } else {
                fatalError("ERROR: poll");
            }
        }
        // Timeout finished without reading anything
        else if (poll_response == 0) {}
        else {
            // There is something ready at the socket
            // Check the type of event detected
            if (test_fds[0].revents & POLLIN) {
                //if there is a battle already wait for it to end
                if (player1->online == 1 && player2->online == 1){
                    printf("Server is full, waiting for battle to end\n");
                    while(player1->online == 1 && player2->online == 1){}
                }
                // ACCEPT
                // Wait for a client connection
                client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);

                if (client_fd == -1) {
                    fatalError("ERROR: accept");
                }
                //if 2 players already played restart turn and id
                if (id == 3) {
                    id = 1;
                    turn = 1;
                }

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

                // CREATE A THREAD
                int result = pthread_create(&new_tid, NULL, attentionThread, (void*)connection_data);

                if (result == -1) {
                    perror("pthread_create");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

//Main function to attend players
void * attentionThread(void * arg)
{
    // Receive the data for the battle, mutexes and socket file descriptor
    thread_data_t * connection_data = (thread_data_t *) arg;
    // Get the data form the incoming player
    getPlayerData(connection_data);
    // Wait for both players to be online before battle
    while(connection_data->player1->ready == 0 || connection_data->player2->ready == 0){}
    // Go to battle function
    battle(connection_data);
    // Set players offline
    offline(connection_data);
    // Free connection memory
    free(connection_data);
    // Close the thread
    pthread_exit(NULL);
}

//Function to read player data
void getPlayerData(thread_data_t * connection_data)
{
    char buffer[BUFFER_SIZE];
    getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);
    // If player one is not online, new player is player 1
    if (connection_data->player1->online == 0) {
        // Get data from the player, set player online and send WAIT signal
        sscanf(buffer, "%s %s %f %d %f %f %d %d %d %d", connection_data->player1->name, connection_data->player1->pokemon->name, &connection_data->player1->pokemon->HP, &connection_data->player1->pokemon->MP, &connection_data->player1->pokemon->attack1, &connection_data->player1->pokemon->attack2, &connection_data->player1->pokemon->attack_percent, &connection_data->player1->potions[0], &connection_data->player1->potions[1], &connection_data->player1->potions[2]);
        printf("Player data 1: %s %s %f %d %f %f %d %d %d %d\n", connection_data->player1->name, connection_data->player1->pokemon->name, connection_data->player1->pokemon->HP, connection_data->player1->pokemon->MP, connection_data->player1->pokemon->attack1, connection_data->player1->pokemon->attack2, connection_data->player1->pokemon->attack_percent, connection_data->player1->potions[0], connection_data->player1->potions[1], connection_data->player1->potions[2]);
        connection_data->player1->online = 1;
        sprintf(buffer, "WAIT");
        sendString(connection_data->connection_fd, buffer);
        // While player 2 is not online wait
        while (connection_data->player2->online == 0) { }
        // When player 2 is online send PLAY signal and set player 1 to ready
        sprintf(buffer, "PLAY");
        sendString(connection_data->connection_fd, buffer);
        connection_data->player1->ready = 1;
    } else { //Player 1 is already online, new player is player 2
        // Get data from the player and send set player online and send PLAY signal and set ready
        sscanf(buffer, "%s %s %f %d %f %f %d %d %d %d ", connection_data->player2->name, connection_data->player2->pokemon->name, &connection_data->player2->pokemon->HP, &connection_data->player2->pokemon->MP, &connection_data->player2->pokemon->attack1, &connection_data->player2->pokemon->attack2, &connection_data->player2->pokemon->attack_percent, &connection_data->player2->potions[0], &connection_data->player2->potions[1], &connection_data->player2->potions[2]);
        connection_data->player2->online = 1;
        sprintf(buffer, "PLAY");
        sendString(connection_data->connection_fd, buffer);
        connection_data->player2->ready = 1;
        printf("Player 2 data: %s %s %f %d %f %f %d\n", connection_data->player2->name, connection_data->player2->pokemon->name, connection_data->player2->pokemon->HP, connection_data->player2->pokemon->MP, connection_data->player2->pokemon->attack1, connection_data->player2->pokemon->attack2, connection_data->player2->pokemon->attack_percent);
    }
}

//Main battle function
void battle(thread_data_t * connection_data)
{
    char buffer[BUFFER_SIZE];
    if (connection_data->player_id==1) {
        // Get OPPONET request form player and send opponent data
        getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);
        if (strncmp(buffer, "OPPONENT", 8) == 0) {
            sprintf(buffer, "%s %s %f %d %f %f %d %d %d %d", connection_data->player2->name, connection_data->player2->pokemon->name, connection_data->player2->pokemon->HP, connection_data->player2->pokemon->MP, connection_data->player2->pokemon->attack1, connection_data->player2->pokemon->attack2, connection_data->player2->pokemon->attack_percent, connection_data->player2->potions[0], connection_data->player2->potions[1], connection_data->player2->potions[2]);
            sendString(connection_data->connection_fd, buffer);
        }
        // Get READY form player and start the battle
        getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);
        if (strncmp(buffer, "READY", 5) == 0) {
            // Battle until a player HP is 0 or less
            while (connection_data->player1->pokemon->HP > 0 && connection_data->player2->pokemon->HP > 0 && !interrupted) {
                pthread_mutex_lock(&connection_data->data_locks->attack_mutex);
                // First is player's 1 turn send TURN signal
                sprintf(buffer, "TURN");
                sendString(connection_data->connection_fd, buffer);
                // Recive the attack
                reciveAttack(connection_data);
                // Send the result of the attack
                sendData(connection_data);
                //switch the turn to the second player
                turn = 2;
                pthread_mutex_unlock(&connection_data->data_locks->attack_mutex);

                if (connection_data->player1->pokemon->HP > 0 && connection_data->player2->pokemon->HP > 0 && !interrupted) {
                    pthread_mutex_lock(&connection_data->data_locks->wait_mutex);
                    //Now player 1 waits from player's 1 attack
                    sprintf(buffer, "WAIT");
                    sendString(connection_data->connection_fd, buffer);
                    //wait until player 2 send the attack
                    getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);
                    while(turn == 2) { }
                    // Send the result of player's 2 attack to player 1
                    sendData(connection_data);
                    pthread_mutex_unlock(&connection_data->data_locks->wait_mutex);
                }
            }
            // Battle id over send END signal
            sprintf(buffer, "END");
            sendString(connection_data->connection_fd, buffer);
        }
    } else if (connection_data->player_id==2) {
        // If its player 2 get OPPNONENT data request and send it
        getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);

        if(strncmp(buffer, "OPPONENT", 8) == 0) {
            sprintf(buffer, "%s %s %f %d %f %f %d %d %d %d", connection_data->player1->name, connection_data->player1->pokemon->name, connection_data->player1->pokemon->HP, connection_data->player1->pokemon->MP, connection_data->player1->pokemon->attack1, connection_data->player1->pokemon->attack2, connection_data->player1->pokemon->attack_percent,  connection_data->player1->potions[0],  connection_data->player1->potions[1], connection_data->player1->potions[2]);
            sendString(connection_data->connection_fd, buffer);
        }

        // Get READY signal
        getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);
        if (strncmp(buffer, "READY", 5) == 0) {
            // Battle until a players HP is cero or less
            while(connection_data->player1->pokemon-> HP > 0 && connection_data->player2->pokemon-> HP > 0 && !interrupted) {
                // First is players1 turn so send WAIT signal
                pthread_mutex_lock(&connection_data->data_locks->wait_mutex);

                sprintf(buffer, "WAIT");
                sendString(connection_data->connection_fd, buffer);
                getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);
                // Wait until player 1 sends the attack
                while(turn == 1) {}
                // Send the data from players 1 attack
                sendData(connection_data);

                pthread_mutex_unlock(&connection_data->data_locks->wait_mutex);

                if(connection_data->player1->pokemon->HP > 0 && connection_data->player2->pokemon->HP > 0 && !interrupted) {
                    pthread_mutex_lock(&connection_data->data_locks->attack_mutex);

                    // Now its player's 2 turn
                    sprintf(buffer, "TURN");
                    sendString(connection_data->connection_fd, buffer);
                    // Get the attack from player 2
                    reciveAttack(connection_data);
                    // Send the result of the attack
                    sendData(connection_data);
                    turn = 1;

                    pthread_mutex_unlock(&connection_data->data_locks->attack_mutex);
                }
            }
            // Battle id over send END signal
            sprintf(buffer, "END");
            sendString(connection_data->connection_fd, buffer);
        }
    }
}

//Function to send the attack data to players
void sendData(thread_data_t * connection_data)
{
    char buffer[BUFFER_SIZE];
    //If its player 1 and online
    if(connection_data->player_id==1) {
        if (connection_data->player2->online == 1) {
            //send the action and the attack points and HP
            sprintf(buffer, "%c %f %f %f", action, attack_points, connection_data->player1->pokemon->HP, connection_data->player2->pokemon->HP);
            sendString(connection_data->connection_fd, buffer);
        } else { //If player disconected
            sprintf(buffer, "END");
            sendString(connection_data->connection_fd, buffer);
        }
    //same protocol for player2
    } else if (connection_data->player_id==2) {
        if (connection_data->player1->online == 1) {
            sprintf(buffer, "%c %f %f %f", action, attack_points, connection_data->player2->pokemon->HP, connection_data->player1->pokemon->HP);
            sendString(connection_data->connection_fd, buffer);
        } else {
            sprintf(buffer, "END");
            sendString(connection_data->connection_fd, buffer);
        }
    }
}

//function to recive the attack by the players
void reciveAttack(thread_data_t *connection_data)
{
    char buffer[BUFFER_SIZE];
    getMessage(connection_data->connection_fd, buffer, BUFFER_SIZE);
    sscanf(buffer, "%c", &action);
    //if its and attack send it to attack functions
    if (action == '1' || action == '2') {
        attack(connection_data);
    }
    //if its a function send ot to potion function
    if (action == '3' || action == '4' || action == '5') {
        potion(connection_data);
    }
}

// function to calculate the player attack
void attack(thread_data_t * connection_data)
{
    //random number to check if attack was succesfull
    int prob = rand() % 100 + 1;
    if (connection_data->player_id==1) {
        //calculate points for attack 1
        if (action == '1') {
            attack_points = connection_data->player1->pokemon->MP * connection_data->player1->pokemon->attack1;
        }
        //calculate points for attack 2
        if (action == '2') {
            attack_points = connection_data->player1->pokemon->MP * connection_data->player1->pokemon->attack2;
        }
      //if the random number is lower attack was succesfull, deduct points from rival HP
    	if (prob <= connection_data->player1->pokemon->attack_percent) {
    		connection_data->player2->pokemon->HP -= attack_points;
    	} else { //If attack failed, no attack points
            attack_points = 0.0;
        }

   // Same protocol for player 2
    } else if (connection_data->player_id==2) {
        if (action == '1') {
            attack_points = connection_data->player2->pokemon->MP * connection_data->player2->pokemon->attack1;
        }
        if (action == '2') {
            attack_points = connection_data->player2->pokemon->MP * connection_data->player2->pokemon->attack2;
        }
    	if (prob <= connection_data->player2->pokemon->attack_percent) {
    		connection_data->player1->pokemon->HP -= attack_points;
    	} else {
            attack_points = 0.0;
        }
    }
}

//Function to handle potions intake
void potion(thread_data_t * connection_data)
{
    if(connection_data->player_id==1) {
        if (action == '3') { // potion 1 increments pokemon HP
            connection_data->player1->pokemon->HP += 30;
        } else if (action == '4') {// potion 2 increments pokemon MP
            connection_data->player1->pokemon->MP += 5;
        } else if (action == '5') {//potion 3 increments the pokemon attack porcentage
            connection_data->player1->pokemon->attack_percent += 5;
        }
    //Same protocol for player 2
    } else if (connection_data->player_id==2) {
        if (action == '3') {
            connection_data->player2->pokemon->HP += 30;
        } else if (action == '4') {
            connection_data->player2->pokemon->MP += 5;
        } else if (action == '5') {
            connection_data->player2->pokemon->attack_percent += 5;
        }
    }
}

//function to set players offline
void offline(thread_data_t * connection_data)
{
    connection_data->player1->online = 0;
    connection_data->player2->online = 0;
    connection_data->player1->ready = 0;
    connection_data->player2->ready = 0;
}

//function to free the memmory and close server
void closeGame(player_t * player1, player_t * player2, locks_t * data_locks)
{
    printf("DEBUG: Clearing the memory for the thread\n");
    free(player1->pokemon);
    free(player2->pokemon);
}
