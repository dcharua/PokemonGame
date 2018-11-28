/*
  PokemonServer.c
  FinalProyect
  24/11/18
  Daniel Charua - Joan Andoni
  Copyright (c) 2018. All rights reserved.
*/

// Custom libraries
#include "prints.h"
#include "sockets.h"

// Library for the wait
#include <unistd.h>
#include <errno.h>
#include <signal.h>

// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>

// Definition of the size of the buffer that we are
// going to use for the communication
#define BUFFER_SIZE 1024

// Structure for the pokemon attributes
typedef struct pokemon {
    char name[10];
    float HP;
    int MP;
    float attack1;
    float attack2;
    int attack_percent;
} pokemon_t;

// Structure for the player attributes
typedef struct player {
    char name[10];
    int gender;
    int potions[3];
    int stages[4];
    pokemon_t * pokemon;
} player_t;

//golabl variables for catch interrupt
int online = 0;
int interrupted = 0;

// DEFINITION OF THE FUNCTIONS

// Functions that read files
void readPokemon(char* filename, pokemon_t* pokemon);
void readPlayer(char* filename, player_t* player);

// Initial functions
void mainMenu(player_t * player);
void setupHandlers();
void catchInterrupt(int signal);
void introduction(player_t * player);
void genderName(player_t * player);
void printStatus(player_t * player);
void backpack (player_t * player);
char potions(player_t* player);
void potionsPictures(player_t * player);

// Fight functions
void selectStage(player_t * player);
int fight(player_t * player, pokemon_t * opponent, int stage);
void attack(player_t * player, pokemon_t * opponent, float attack);

// Functions that write files
void writeFile(char* filename, player_t* player);
void writePokemon(char* filename, player_t * player);

// Online game
void playOnline(player_t * player, char* filename);
void battleOnline(player_t * player, int connection_fd);
void battleDefend(player_t * player, player_t * opponent, int connection_fd, float full_HP, float opponent_full_HP);
void battleAttack(player_t * player, player_t * opponent, int connection_fd, float full_HP, float opponent_full_HP);
void checkOpAction(char action, float attack, player_t * player);
void checkAction(char action, float attack, player_t * opponent);
