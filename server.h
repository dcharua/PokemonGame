/*
  PokemonServer.c
  FinalProyect
  24/11/18
  Daniel Charua - Joan Andoni
  Copyright (c) 2018. All rights reserved.
*/

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
char action;
float attack_points;
pthread_cond_t player1_turn = PTHREAD_COND_INITIALIZER;
pthread_cond_t player2_turn = PTHREAD_COND_INITIALIZER;

typedef struct pokemon {
    char name[10];
    float HP;
    int MP;
    float attack1;
    float attack2;
    int attack_percent;
} pokemon_t;

typedef struct player {
    char name[10];
    int potions[3];
    int online;
    int ready;
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

// Function declarations
void usage(char * program);
void setupHandlers();
void catchInterrupt(int signal);
void initGame(player_t * player1, player_t * player2, locks_t * data_locks);
void waitForConnections(int server_fd, player_t * player1, player_t * player2, locks_t * data_locks);
void * attentionThread(void * arg);
void getPlayerData(thread_data_t * connection_data);
void battle(thread_data_t * connection_data);
void sendData(thread_data_t * connection_data);
void reciveAttack(thread_data_t *connection_data);
void attack(thread_data_t * connection_data);
void potion(thread_data_t * connection_data);
void closeGame(player_t * player1, player_t * player2, locks_t * data_locks);
void offline(thread_data_t * connection_data);
