// Basic libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>

// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>

// Custom libraries
#include "sockets.h"

// Definition of the size of the buffer that we are
// going to use for the communication
#define BUFFER_SIZE 1024

// Structure of the pokemon attributes
typedef struct pokemon{
  char name[10];
  float HP;
  int MP;
  float attack1;
  float attack2;
  int attack_percent;
} pokemon_t;

// Structure of the player attributes
typedef struct player{
  char name[10];
  int gender;
  int potions[3];
  int stages[4];
  pokemon_t * pokemon;
} player_t;


// DEFINITION OF THE FUNCTIONS

// Functions that read files
int read_file(char* filename, player_t* player);
int read_items(char* filename, player_t* player);

// Main menu loop
void loop_Mainmenu(player_t * player);
void introduction(player_t * player);
void select_stage(player_t * player);

// Functions that write files
int write_file(char* filename, player_t* player);
int write_pokemon(char* filename, player_t * player);

// Functions that print without arguments
void genders();
void male();
void female();
void pikachufront();
void map();
void youwon();

// Functions that print with parameters
void GenderName(player_t * player);
void printStatus(player_t * player);
void backpack (player_t * player);
void potions(player_t* player);
void potionsPictures(player_t * player);
void pikachuBack(float hp, float hpFull, char* name);
void gengar(float hp, float hpFull, char* name);
void charizard(float hp, float hpFull, char* name);
void zapdos(float hp, float hpFull, char* name);
void mew(float hp, float hpFull, char* name);


// Online game
void playOnline(player_t * player);
void battleOnline(player_t * player, int connection_fd);
void battleDefend(player_t * player, player_t * opponent, int connection_fd, float full_HP, float opponent_full_HP);
void battleAttack(player_t * player, player_t * opponent, int connection_fd, float full_HP, float opponent_full_HP);

int main(int argC, char *argV[])
{
    player_t player;
    player.pokemon = malloc(sizeof (pokemon_t));
    int savedProgress = 0;
    char* fpikachu = "Pikachu.txt";
	char* fpikachuTwo = "PikachuTwo.txt";
	char* fItems = "Items.txt";

    printf("\t\tWELCOME TO THE WORLD OF POKEMONS\n");
    printf("*****************************************************************\n\n");
    printf("  Do you have a saved progress? { Yes: 1 | No: 2 }\n");

    while ((savedProgress != 1)&&(savedProgress != 2)) {
        printf("\tOption: ");
        scanf("%d", &savedProgress);
        switch(savedProgress) {
            case 1:
                read_items(fItems, &player);
                read_file(fpikachuTwo, &player);
            break;

            case 2:
                introduction(&player);
                read_file(fpikachu, &player);
            break;

            default:
                printf("Invalid option, try again\n\n");
            break;
        }
    }

    loop_Mainmenu(&player);

  return 0;
}

int read_file(char* filename, player_t* player)
{
    FILE* filePtr;
    filePtr = fopen(filename, "r");

    if (filePtr == NULL) {
        printf("Could not open file '%s' for reading.\n", filename);
        return 1;
    }

    fscanf(filePtr, " %s", player->pokemon->name);
    fscanf(filePtr, " %f", &player->pokemon->HP);
    fscanf(filePtr, " %d", &player->pokemon->MP);
    fscanf(filePtr, " %f", &player->pokemon->attack1);
    fscanf(filePtr, " %f", &player->pokemon->attack2);
    fscanf(filePtr, " %d", &player->pokemon->attack_percent);

    fclose(filePtr);
    return 0;
}

int read_items(char* filename, player_t* player)
{
	FILE* filePtr;
	filePtr = fopen(filename, "r");
	int i = 0;

	if (filePtr == NULL)
		printf("Could not open file '%s' for reading.\n", filename);

	fscanf(filePtr, "%s", player->name);

	for (i = 0; i < 3; i++)
        fscanf(filePtr, " %d", &player->potions[i]);

	for (i = 0; i < 4; i++)
        fscanf(filePtr, " %d", &player->stages[i]);

	fclose(filePtr);
	return 0;
}

void introduction(player_t * player)
{
    printf("\n\nHello my name is Profesor Elm...\n");
    printf("¿What is your name?\n\t Name: ");
    scanf("%s", player->name);
    GenderName(player);

    // HERE WE ARE GOING TO PICK THE STARTER POKEMON
    usleep(1000000);
    printf("\nAnd now we are going to give you a new pokémon named Pikachu...\n");
    pikachufront();

    printf("\nGreat choice. That pokémon is awesome\n");
    usleep(2000000);
    printf("\nHere you have a map that you have to complete to become a Pokemon Master\n");
    usleep(2000000);
    printf("\nIn each stage there is a battle of differents pokemon and you have to fight to get more potions\n");
    usleep(2000000);
    printf("\nYou won't be able to  pass to another level if the previous one hasn't been complete yet\n");
    usleep(2000000);
    printf("\nTheres other option where you can play against other players online so train hard to beat them\n");
    usleep(2000000);
    printf("\nYou have a lot items in your backpack that you can use on your pokemon\n");
    usleep(2000000);
    printf("\nWell thats all %s\n", player->name);
    usleep(2000000);
    printf("\nNow you can start, ¡GOOD LUCK IN YOUR JOURNEY!\n\n");
    usleep(2000000);
}

void GenderName(player_t * player)
{
    int gender = 0;

    while ((gender != 1) && (gender != 2))
    {
        printf("\n Well %s first things first, what gender do you want to be?\n\n", player->name);
        usleep(1000000);
        genders();
        printf("\nChose an option : ");
        scanf("%d", &gender);
        switch(gender)
        {
            case 1:
                printf("\nPerfect then, you will be him: \n\n");
                usleep(1000000);
                male();
                printf("\n\t\t%s\n", player->name);
            break;

            case 2:
                printf("\nPerfect now you are her: \n\n");
                usleep(1000000);
                female();
                printf("\n\t\t%s\n", player->name);
            break;

            default:
                printf("Invalid option, try again\n\n");
            break;
        }
    }
}

void loop_Mainmenu(player_t * player)
{
    int option = 1;
    char* fitems = "Items.txt";
	char* fpikachu = "PikachuTwo.txt";

    while (option != 0) {
        printf("\n-------------------------------------------------------------------------------");
        printf("\n  Welcome to the main menu %s, please choose one of the options below.\n\n", player->name);
        printf("\t1. Go to the Map and main story\n");
        printf("\t2. Check whats your backpack\n");
        printf("\t3. Check your pokemon stats\n");
        printf("\t4. Play Online\n");
        printf("\t5. Save game\n");
        printf("\t0. Quit the game\n\n");
        printf("Select a option : ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                // CHECK
                select_stage(player);
            break;

            case 2:
                // CHECK
                backpack(player);
            break;

            case 3:
                // CHECK
                pikachufront();
                printStatus(player);
            break;

            case 4:
                // CHECK
      			playOnline(player);
            break;

            case 5:
      		    write_file(fitems, player);
      			write_pokemon(fpikachu, player);
      			printf("\n\n\tCongratulations . . . You already saved the game\n\n");
            break;

            case 0:
                printf("\nHave you already saved your game? { Yes: 0 | No: 1 }\n");
                printf("\tOption: ");
                scanf("%d", &option);
            break;

            default:
                printf("Invalid option, try again\n\n");
            break;
        }
    }
}

void printStatus(player_t * player)
{
    printf("\n\t\t%s stats:\n", player->pokemon->name);
    printf("\t\t\tHP:     %.0f\n", player->pokemon->HP);
    printf("\t\t\tMP:     %d\n", player->pokemon->MP);
    printf("\t\t\tAtt1:   %.2f\n", player->pokemon->attack1);
    printf("\t\t\tAtt2:   %.2f\n", player->pokemon->attack2);
    printf("\t\t\tAtt%%:   %d\n", player->pokemon->attack_percent);
}

void backpack (player_t * player){
  char bp = '1';

  while (bp != 0){
    printf("\n-----------------------------------------------------------------------------");
    printf("\n\t\t\t\tWelcome to your backpack %s!\n\n", player->name);
    printf("\t1. View Map. \n");
    printf("\t2. View Potions. \n");
    printf("\t3. Check my pokemon stats. \n");
    printf("\t0. Exit Backpack. \n\n");
    printf("Enter an option from above: ");
    bp = getchar();
    getchar();

    switch(bp){
      case '1':
        map();
        break;

      case '2':
        potions(player);
        break;

      case '3':
        pikachufront();
        printStatus(player);
        break;

      case '0':
        break;

      default:
        printf("Invalid option, please try again.\n");
        break;
    }
  }
}

void HP(float hpFight, float hpFull){
  float percent, percent2;
  percent2 = 50/hpFull;
  percent = percent2*hpFight;

  printf("HP: %.0f/%.0f ", hpFight, hpFull);
  for(int i = 0; i < percent; i++)
    printf("*");
  printf("\n\n");
}


void attack(player_t * player, player_t * opponent, float attack){
	int prob1 = rand() % 100 + 1;
	int prob2 = rand() % 100 + 1;
	int oppAttack1 = rand() % 2 + 1;
	float finalattack1 = opponent->pokemon->MP * opponent->pokemon->attack1;
  float finalattack2 = opponent->pokemon->MP * opponent->pokemon->attack2;
  float playerAttack = (player->pokemon->MP * attack);

	if (prob1 <= player->pokemon->attack_percent) {
		opponent->pokemon->HP -= playerAttack;
		printf("\nYou take %.0f of his HP!\n", playerAttack );
	} else {
    printf("\nYour attack has failed\n");
  }

  if (prob2 <= opponent->pokemon->attack_percent) {
		if (oppAttack1 == 1) {
			player->pokemon->HP -= finalattack1;
			printf("\nThe opponent has taken %.0f of your HP!\n", finalattack1);
		} else if (oppAttack1 == 2) {
			player->pokemon->HP -= finalattack2;
			printf("\nThe opponent has taken %.0f of your HP!\n", finalattack2);
		}
	} else {
    printf("\nHis attack failed\n");
  }
}

int fight(player_t * player, player_t * opponent, int stage){
	char fchoice;
	float hpFull = opponent->pokemon->HP, PikachuHPFull = player->pokemon->HP;

  printf("\n\nAre you ready to battle? Yes : 1 | No : 0\n\tAnswer : ");
  fchoice = getchar();
  getchar();

  while (fchoice != '0') {
    if (stage == 1)
      gengar(opponent->pokemon->HP, hpFull, opponent->pokemon->name);
		else if (stage == 2)
      charizard(opponent->pokemon->HP, hpFull, opponent->pokemon->name);
		else if (stage == 3)
      zapdos(opponent->pokemon->HP, hpFull, opponent->pokemon->name);
		else if (stage == 4)
      mew(opponent->pokemon->HP, hpFull, opponent->pokemon->name);

		pikachuBack(player->pokemon->HP, PikachuHPFull, player->pokemon->name);
		srand(time(NULL));
    fchoice = getchar();
    getchar();
    switch(fchoice){
      case '1':
			   attack(player, opponent, player->pokemon->attack1);
			   break;

      case '2':
			   attack(player, opponent, player->pokemon->attack2);
         break;

      case '3':
        backpack(player);
        break;

      case '0':
        printf("If you exit the battle you'll be returned to the main menu\nand all progress of the battle will be lost.\n");
        printf("Are you sure you want to run away? YES : 0 | NO : 1 \n");
        fchoice = getchar();
        getchar();
  			player->pokemon->HP = 0;
        break;

      default:
        printf("Invalid option, please try again.\n");
        break;
    }

    if (player->pokemon->HP <= 0) {
			printf("\n\t\tYOU LOSE...\n\n");
			return 0;
		} else if (opponent->pokemon->HP <= 0) {
			printf("\n\t\tYOU WON!!!!!!!!\n\n");
			return 1;
		}
  }
  return 0;
}

void select_stage(player_t * player){
	int s = 1;
	player_t gengar, charizard, zapdos,  mew;
  gengar.pokemon = malloc(sizeof (pokemon_t));
  charizard.pokemon = malloc(sizeof (pokemon_t));
  zapdos.pokemon = malloc(sizeof (pokemon_t));
  mew.pokemon = malloc(sizeof (pokemon_t));
  char* fgengar = "Gengar.txt";
	read_file(fgengar, &gengar);
	char* fcharizard = "Charizard.txt";
	read_file(fcharizard, &charizard);
	char* fzapdos = "Zapdos.txt";
	read_file(fzapdos, &zapdos);
	char* fmew = "Mew.txt";
	read_file(fmew, &mew);

  while (s != 0) {
    map();
    printf("\nWhich stage do you want to enter? | 1 | 2 | 3 | 4 | \n\t If you want to return to the main menu | 0 |\n\n Option : ");
    scanf("%d", &s);
    getchar();
    switch(s) {
      case 1:
        player->stages[0] = fight(player, &gengar, s);
        if (player->stages[0] == 1)
          printf("The stage one is complete\n");
        break;

      case 2:
        if(player->stages[0] == 1) {
          player->stages[1] = fight(player, &charizard, s);
          if (player->stages[1] == 1) {
            printf("The stage two is complete\n");
          }
        } else {
          printf("You have to complete previous stages to enter this stage, please try again.\n");
        }
        break;

      case 3:
        if(player->stages[0] == 1 && player->stages[1] == 1) {
          player->stages[2] = fight(player, &zapdos, s);
          if (player->stages[2] == 1) {
            printf("The stage three is complete\n");
          }
        } else {
          printf("You have to complete previous stages to enter this stage, please try again.\n");
        }
        break;

      case 4:
        if(player->stages[0] == 1 && player->stages[1] == 1 && player->stages[2] == 1) {
          player->stages[3] = fight(player, &mew, s);
          youwon();
          exit(0);
        } else {
          printf("You have to complete previous stages to enter this stage, please try again.\n");
        }
        break;

      case 0:
        break;

      default:
        printf("The stage number you entered is invalid, please try again.\n");
        break;
    }
  }
}

void playOnline(player_t * player){
  char address[20];
  char port[4];
  char buffer[BUFFER_SIZE];
  int connection_fd = 0;
  // Start the server
  connection_fd = connectSocket("127.0.0.1", "8989");
  if (connection_fd ){
    // Send my data
    sprintf(buffer, "%s %s %f %d %f %f %d", player->name, player->pokemon->name, player->pokemon->HP, player->pokemon->MP, player->pokemon->attack1, player->pokemon->attack2, player->pokemon->attack_percent);
  }
  sendString(connection_fd, buffer);
  // RECV
  // Receive the response
  getMessage(connection_fd, buffer, BUFFER_SIZE);

  if(strncmp(buffer, "WAIT", 4) == 0)
    printf("Waiting for another player\n");
  while ( strncmp(buffer, "PLAY", 4) != 0){
    getMessage(connection_fd, buffer, BUFFER_SIZE);
  }
  battleOnline(player, connection_fd);
}

void battleOnline(player_t * player, int connection_fd){
  char buffer[BUFFER_SIZE];
  float full_HP = player->pokemon->HP;
  player_t  opponent;
  opponent.pokemon = malloc(sizeof (pokemon_t));
  char opponent_name[15];
  //Get opponent data
  sprintf(buffer, "OPPONENT");
  sendString(connection_fd, buffer);
  getMessage(connection_fd, buffer, BUFFER_SIZE);
  sscanf(buffer, "%s %s %f %d %f %f %d", opponent.name,  opponent.pokemon->name, &opponent.pokemon->HP, &opponent.pokemon->MP, &opponent.pokemon->attack1, &opponent.pokemon->attack2, &opponent.pokemon->attack_percent);
  float opponent_full_HP = opponent.pokemon->HP;
  sprintf(buffer, "READY");
  sendString(connection_fd, buffer);
  //Battle Loop
  printf("===== WELCOME TO THE COLISEUM =====\n");
  while(strncmp(buffer, "END", 3) != 0){
    getMessage(connection_fd, buffer, BUFFER_SIZE);
    if(strncmp(buffer, "TURN", 4) == 0)
      battleAttack(player, &opponent, connection_fd, full_HP, opponent_full_HP);

    else if(strncmp(buffer, "WAIT", 4) == 0)
      battleDefend(player, &opponent, connection_fd, full_HP, opponent_full_HP);
  }
}

void battleAttack(player_t * player, player_t * opponent, int connection_fd, float full_HP, float opponent_full_HP){
  float attack;
  char action;
  char buffer[BUFFER_SIZE];
  gengar(opponent->pokemon->HP,  opponent_full_HP, opponent->pokemon->name);
  pikachuBack(player->pokemon->HP, full_HP, player->pokemon->name);
  //scanf(" %c", &action);
  action = getchar();
  getchar();
  sprintf(buffer, "%c", action);
  sendString(connection_fd, buffer);
  //recibo el resultado
  getMessage(connection_fd, buffer, BUFFER_SIZE);
  sscanf(buffer, "%f %f %f", &attack, &player->pokemon->HP, &opponent->pokemon->HP);
  if (attack > 0){
   printf("\n=================================\nYou Take %f of his HP\n=================================\n", attack);
 } else {
    printf("\n##################\nYour attack has failed\n##################\n");
  }
  if (opponent->pokemon->HP <= 0)
    printf("\n!!!!!!!!!!!!!!!!!!!!!\nYOU HAVE WON\n!!!!!!!!!!!!!!!!!!!!!\n");
}

//No esta acabada
void battleDefend(player_t * player, player_t * opponent, int connection_fd,  float full_HP, float opponent_full_HP){
  float attack;
  char buffer[BUFFER_SIZE];
  //recibo el resultado del ataque del oponente
  sprintf(buffer, "WAITING");
  sendString(connection_fd, buffer);
  printf("Waiting for player attack\n");
  getMessage(connection_fd, buffer, BUFFER_SIZE);
  sscanf(buffer, "%f %f %f", &attack, &player->pokemon->HP, & opponent->pokemon->HP);
  if (attack > 0){
   printf("\n""""""""""""""""""""""""""""""\nHe Takes %f of your HP\n""""""""""""""""""""""""""""""\n", attack);
 }else{
    printf("\n!!!!!!!!!!!!!!!!!!!!!\nHis attack has failed\n!!!!!!!!!!!!!!!!!!!!!\n");
  }
  if (player->pokemon->HP <= 0)
    printf("\n!!!!!!!!!!!!!!!!!!!!!\nYOU HAVE LOST\n!!!!!!!!!!!!!!!!!!!!!\n");
}

void potions(player_t * player){
  char potion = '1';
  while (potion != '0'){
    potionsPictures(player);
    printf("Go out : 0\n\tWhat potion do you want %s? : ", player->name);
    potion = getchar();
    getchar();

    switch (potion) {
      case '1':
        if(player->potions[0] > 0) {
          printf("\nThis is your HP before using the potion: %.0f", player->pokemon->HP);
          player->pokemon->HP += 30;
          player->potions[0]--;
          printf("\nThis is your HP after using the potion: %.0f\n", player->pokemon->HP);
        } else {
          printf("\nYou don't have any potions left\n\n");
        }
        break;

      case '2':
        if(player->potions[1] > 0) {
          printf("\nThis is your MP before using the potion: %d", player->pokemon->MP);
          player->pokemon->MP += 5;
          player->potions[1]--;
          printf("\nThis is your MP after using the potion: %d\n",player->pokemon->MP);
        } else {
          printf("\nYou don't have any potions left\n\n");
        }
        break;

      case '3':
        if(player->potions[2] > 0) {
          printf("\nThis is your Attack%% before using the potion: %d%% ", player->pokemon->attack_percent);
          player->pokemon->attack_percent += 5;
          player->potions[2]--;
          printf("\nThis is your Attack%% after using the potion: %d%%\n", player->pokemon->attack_percent);
        } else {
          printf("\nYou don't have any potions left\n\n");
        }
        break;

      case '0':
        break;

      default:
        printf("Invalid option, please try again.\n");
        break;
    }
  }
}

int write_file(char* filename, player_t* player){
	FILE* filePtr;
	int i;
	filePtr = fopen(filename, "w");

	if (filePtr == NULL)
		printf("Could not open file '%s' for reading.\n", filename);

    fprintf(filePtr, " %s", player->name);

    for (i = 0; i < 3; i++)
		fprintf(filePtr, " %d", player->potions[i]);

	for (i = 0; i < 4; i++)
		fprintf(filePtr, " %d", player->stages[i]);


	fclose(filePtr);
	return 0;
}

int write_pokemon(char* filename, player_t * player){
    FILE* filePtr;
	filePtr = fopen(filename, "w");
	if (filePtr == NULL)
		printf("Could not open file '%s' for reading.\n", filename);

	fprintf(filePtr, " %s", player->pokemon->name);
	fprintf(filePtr, " %f", player->pokemon->HP);
	fprintf(filePtr, " %d", player->pokemon->MP);
	fprintf(filePtr, " %f", player->pokemon->attack1);
	fprintf(filePtr, " %f", player->pokemon->attack2);
	fprintf(filePtr, " %d", player->pokemon->attack_percent);

	fclose(filePtr);
	return 0;
}

void map() {
  printf("\n           ********  **  **        ***                  *          ****       ** *** \n");
  printf("          *        **  **  **     * *          ****    * *        *    **    *  *  * \n");
  printf("       ***                   **  * *         **    ** *   **    **       ** *    4  *\n");
  printf("      *                        ** ****      *        *      * **           *    /  * \n");
  printf("     *                                ******                 *                 /   * \n");
  printf("    *          -------------------------2--%c                         --------/   *  \n",92);
  printf("   *          /                        **   %c                       /           *   \n",92);
  printf("  *          /                        *  *   %c                     /           *    \n",92);
  printf(" *          /                        *    *   %c                   /            *    \n",92);
  printf("*          /                        *     *    %c------%c         /             *    \n",92,92);
  printf("*       1-/                        *       *           %c        /               *   \n",92);
  printf("*                             *****         *     *      %c----3/             *****  \n",92);
  printf(" *                 ****       *              *   * *                       ***       \n");
  printf("  *               *    ***     *              ***   *                   ***          \n");
  printf("   *             *        **    *                    **              ***             \n");
  printf("    ****        *           **   *                     **          **                \n");
  printf("        **     *              ** *                       ****    **                  \n");
  printf("          *****                 *                            ****                    \n");
}

void pikachufront() {
  printf("\n   88             88  \n");
  printf("   888           888  \n");
  printf("   8888         8888  \n");
  printf("   8   8       8   8  \n");
  printf("   8    8888888    8  \n");
  printf("   88             88    \n");
  printf("   88             88      77777\n");
  printf("   8    O     O    8     7   7 \n");
  printf("   8##     .     ##8   77  77  \n");
  printf("    8##    _    ##8   7   7   \n");
  printf("    88           88  7   7    \n");
  printf("   88             887  7    \n");
  printf("  8  /           %c  87      \n", 92);
  printf("  8 /             %c 87       \n", 92);
  printf("  8/               %c8        \n", 92);
  printf("   888    8888    888         \n");
  printf("  8   888888888888   8        \n");
  printf("   88888        88888         \n");
}

void potionsPictures(player_t * player) {
    printf("\n\n   Potion 1  \n\n");
    printf("     888     \n");
    printf("    88888    \n");
    printf("     888      Number of potions: %d\n", player->potions[0]);
    printf("    88888    \n");
    printf("   8888888    Increases your pokemon's HP by 30 points\n");
    printf("  888888888  \n");
    printf("  888888888  \n");
    printf("   8888888   \n\n\n");

    printf("   Potion 2   \n\n");
    printf("     888      \n");
    printf("    88888     \n");
    printf("     888      Number of potions: %d\n",  player->potions[1]);
    printf("    88888     \n");
    printf("   8888888    Increases your pokemon's MP by 5 points \n");
    printf("  888888888   \n");
    printf(" 88888888888  \n");
    printf("8888888888888 \n\n\n");

    printf("   Potion 3 \n\n");
    printf("     888   \n");
    printf("    88888  \n");
    printf("     888      Number of potions: %d\n",  player->potions[2]);
    printf("     888   \n");
    printf("     888      Increases your pokemon's Attack%% by 5%%\n");
    printf("     888   \n");
    printf("     888   \n");
    printf("     888   \n\n\n");
}

void pikachuBack(float hp, float hpFull, char* name) {
	printf("%s ", name);
	HP(hp, hpFull);
	printf(" 88888     8           8   \n");
  printf("  88888    88         88   \n");
  printf("   8   8   8 8       8 8   \n");
  printf("    8   8  8  8888888  8   \n");
  printf("   8   8   8       8   8   \n");
  printf("  8   8    8            8  \n");
  printf("   8  8   8              8 \n");
  printf("    8 8   8             8           1. Thunder Bolt |  2. Electro Ball\n");
  printf("    888  8            88            3. Backpack     |  0. Exit Battle\n");
  printf("    888  8              83 \n");
  printf("-----88888______________8----------------------------------------------------\n");
}

void gengar(float hp, float hpFull, char* name) {
  printf("\n----------------------------------------------------------------------------\n\t\t\t");
	printf("%s ", name);
	HP(hp, hpFull);
  printf("                                            8%c                /8  \n",92);
  printf("                                            8 8   /%c /%c /%c   8 8  \n",92,92,92);
  printf("                                            8  8 O  O  O  O 8  8   \n");
  printf("                                            8  O           O   8   \n");
  printf("                                           88 O             O 88   \n");
  printf("                                          88 O    %c      /   O 88 \n",92);
  printf("                                        88   O               O  88 \n");
  printf("                                          88  O    ^^^^^^   O  88  \n");
  printf("                                            8  O    ^^^^   O  8    \n");
  printf("                                             8   OOOOOOOOO   8     \n");
  printf("                                              8    8   8    8      \n");
  printf("                                               88888   88888       \n\n");
}

void charizard(float hp, float hpFull, char* name) {
  printf("\n----------------------------------------------------------------------------\n\t\t\t");
	printf("%s ", name);
	HP(hp, hpFull);
  printf("                                                    /%c   /%c           \n",92,92);
  printf("                                                    0000000             \n");
  printf("                                                   0       0            \n");
  printf("                                              8888 0 O   O 0 8888       \n");
  printf("                                           888      0     0      888    \n");
  printf("                                         888         0   0         888  \n");
  printf("                                        888   888    0. .0    888   888 \n");
  printf("                                        88  88   88   000   88   88  88 \n");
  printf("                                        88 8      88       88      8 88 \n");
  printf("                                         88       88       88       88  \n");
  printf("                                                 8    888    8          \n");
  printf("                                                  8888   8888           \n\n");
}

void zapdos(float hp, float hpFull, char* name) {
  printf("\n----------------------------------------------------------------------------\n\t\t\t");
	printf("%s ", name);
	HP(hp, hpFull);
  printf("                                      888       |%c        /|       888\n",92);
  printf("                                        8888  88| %c  88  / |88  8888  \n",92);
  printf("                                          88888  88%c8  8/88  88888    \n",92);
  printf("                                           88888            88888      \n");
  printf("                                         8888       %c  /       8888   \n",92);
  printf("                                       88888888    %c    /    88888888 \n",92);
  printf("                                          88888888  %c  /  88888888    \n",92);
  printf("                                             88888888%c/88888888       \n",92);
  printf("                                           8888888888888888888888      \n");
  printf("                                        888888   8888888888   888888   \n");
  printf("                                       888        88888888        888  \n");
  printf("                                                   V    V              \n\n");
}

void mew(float hp, float hpFull, char* name) {
  printf("\n----------------------------------------------------------------------------\n\t\t\t");
	printf("%s ", name);
	HP(hp, hpFull);
  printf("                                              88             8888               \n");
  printf("                                             8   888 888888 8    8       88888  \n");
  printf("                                             8      88    88   88       8     8 \n");
  printf("                                              8                8       8     8  \n");
  printf("                                             8                8        8    8   \n");
  printf("                                             8o        oo      8        8   8   \n");
  printf("                                             8.o      o.o      8         8  8   \n");
  printf("                                             8oo      oo   8  8           8  8  \n");
  printf("                                              8          88     8          8  8 \n");
  printf("                                               8       88        8          8 8 \n");
  printf("                                                8888888            8        8 8 \n");
  printf("                                                 8                  8      8 8  \n");
  printf("                                                8    8                8   8 8   \n");
  printf("                                               8   8 8                888 8     \n");
  printf("                                              8  8   88               8888      \n");
  printf("                                             888      88       8     88         \n");
  printf("                                                      888      8    88          \n");
  printf("                                                     8   8     88     88        \n");
  printf("                                                  8 8    888888  88      8      \n");
  printf("                                                 8/    88          88    %c8    \n",92);
  printf("                                                 8888888             88888      \n\n");
}

void genders() {
  printf("      888888888888             8888888             \n");
  printf("     88           88         888888888888          \n");
  printf("    88             88      888888888888888         \n");
  printf("  888             8888    88888888888888888        \n");
  printf(" 8       8888888888888   8888888888888888888       \n");
  printf("  8888     88888888888  8     888888888888888      \n");
  printf("     8  O  8      88    8  O        888888888   8  \n");
  printf("     8            88    8           88 888888  888 \n");
  printf("     8__          8      8__       88   88888 8888 \n");
  printf("      8       888|8       8       88     88888888  \n");
  printf("       88888888|||8        88888888        8888    \n");
  printf("         888   8||8         888   88               \n");
  printf("         888   8||8        8888   888              \n");
  printf("       88   888888        888   888888             \n");
  printf("       88     88         888888888888888           \n");
  printf("         88888                88888                \n\n");
  printf("        MALE: 1              FEMALE: 2             \n");
}

void male() {
  printf("\t      888888888888     \n");
  printf("\t     88           88   \n");
  printf("\t    88             88  \n");
  printf("\t  888             8888 \n");
  printf("\t 8       8888888888888 \n");
  printf("\t  8888     88888888888 \n");
  printf("\t     8  O  8      88   \n");
  printf("\t     8            88   \n");
  printf("\t     8__          8    \n");
  printf("\t      8       888|8    \n");
  printf("\t       88888888|||8    \n");
  printf("\t         888   8||8    \n");
  printf("\t         888   8||8    \n");
  printf("\t       88   888888     \n");
  printf("\t       88     88       \n");
  printf("\t         88888         \n");
}

void female() {
  printf("\t        8888888             \n");
  printf("\t      888888888888          \n");
  printf("\t    888888888888888         \n");
  printf("\t   88888888888888888        \n");
  printf("\t  8888888888888888888       \n");
  printf("\t 8     888888888888888      \n");
  printf("\t 8  O        888888888   8  \n");
  printf("\t 8           88 888888  888 \n");
  printf("\t  8__       88   88888 8888 \n");
  printf("\t   8       88     88888888  \n");
  printf("\t    88888888        8888    \n");
  printf("\t     888   88               \n");
  printf("\t    8888   888              \n");
  printf("\t   888   888888             \n");
  printf("\t  888888888888888           \n");
  printf("\t       88888                \n");
}

void youwon() {
	printf("      YY      YY   OOO    UU     UU  \n");
	printf("       YY    YY  OO   OO  UU     UU  \n");
	printf("        YY  YY  OO     OO UU     UU  \n");
	printf("         YYYY   OO     OO UU     UU  \n");
	printf("          YY    OO     OO UU     UU  \n");
	printf("          YY     OO   OO   UU   UU   \n");
	printf("          YY       OOO      UUUUU    \n\n");

	printf("  WW                WW   OOO     NNN     NN \n");
	printf("   WW              WW  OO   OO   NNNN    NN \n");
	printf("    WW     W      WW  OO     OO  NN NN   NN \n");
	printf("     WW   WWW    WW   OO     OO  NN  NN  NN \n");
	printf("      WW WW WW  WW    OO     OO  NN   NN NN \n");
	printf("       WWWW  WWWW      OO   OO   NN    NNNN \n");
	printf("        WW    WW         OOO     NN     NNN \n\n\n");
}
