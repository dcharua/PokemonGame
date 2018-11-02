#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>
// Custom libraries
#include "sockets.h"

#define BUFFER_SIZE 1024

typedef struct pokemon{
  char name[10];
  float HP;
  int MP;
  float attack1;
  float attack2;
  int attack_percent;
}pokemon;

void potions(pokemon* pikachu, char* name, int* potion1, int* potion2, int* potion3);
void introduction(char* name);
int read_file(char* filename, pokemon* pokemon);
int read_items(char* filename, char* name, int potions[3], int stages[4]);
void genders();
void male();
void female();
void pikachufront();
void map();
void potionsPictures(int number1, int number2, int number3);
void pikachuBack(float hp, float hpFull, char* name);
void gengar(float hp, float hpFull, char* name);
void charizard(float hp, float hpFull, char* name);
void zapdos(float hp, float hpFull, char* name);
void mew(float hp, float hpFull, char* name);
void youwon();
void loop_Mainmenu(char* name, pokemon pikachu, int numPotions[3], int stage[4]);
void playOnline(char* name, pokemon * pikachu);
void battleOnline(char * name, pokemon * pikachu, int connection_fd);
void battleDefend(char * name, pokemon * pikachu, int connection_fd);
void battleAttack(char * name, pokemon * pikachu, int connection_fd);

int main(int argC, char *argV[]){
  pokemon pikachu;
	int numPotions[3] = { 5, 3, 2 }, usedPotions[3], stage[4] = { 0 }, usedstage[4];
  char savedProgress;
  char name[30];
  char* fpikachu = "Pikachu.txt";
	char* fpikachuTwo = "PikachuTwo.txt";
	char* fItems = "Items.txt";

  printf("\t\tWELCOME TO THE WORLD OF POKEMONS\n");
  printf("*****************************************************************\n\n");
  printf("Do you have a saved progress? (y/n) : ");
  scanf("%c", &savedProgress);

  if (savedProgress == 'y') {
    read_file(fpikachuTwo, &pikachu);
    read_items(fItems, name, usedPotions, usedstage);
    loop_Mainmenu(name, pikachu, usedPotions, usedstage);
  }

  else if (savedProgress == 'n') {
    read_file(fpikachu, &pikachu);
    introduction(name);
    loop_Mainmenu(name, pikachu, numPotions, stage);
  }

  return 0;
}


int read_file(char* filename, pokemon* pokemon){
  FILE* filePtr;

  filePtr = fopen(filename, "r");
  if (filePtr == NULL) {
    printf("Could not open file '%s' for reading.\n", filename);
    return 1;
  }

  fscanf(filePtr, " %s", pokemon->name);
  fscanf(filePtr, " %f", &pokemon->HP);
  fscanf(filePtr, " %d", &pokemon->MP);
  fscanf(filePtr, " %f", &pokemon->attack1);
  fscanf(filePtr, " %f", &pokemon->attack2);
  fscanf(filePtr, " %d", &pokemon->attack_percent);

  fclose(filePtr);
  return 0;
}

int read_items(char* filename, char* name, int potions[3], int stages[4]){
	FILE* filePtr;
	filePtr = fopen(filename, "r");
	int i;

	if (filePtr == NULL)
		printf("Could not open file '%s' for reading.\n", filename);


	fscanf(filePtr, "%s", name);
	for (i = 0; i < 3; i++)
		fscanf(filePtr, " %d", &potions[i]);

	for (i = 0; i < 4; i++)
		fscanf(filePtr, " %d", &stages[i]);


	fclose(filePtr);
	return 0;
}


int write_file(char* filename, char* name, int potions[3], int stages[4]){
	FILE* filePtr;
	int i;
	filePtr = fopen(filename, "w");

	if (filePtr == NULL)
		printf("Could not open file '%s' for reading.\n", filename);


	fprintf(filePtr, " %s", name);
	for (i = 0; i < 3; i++)
		fprintf(filePtr, " %d", potions[i]);

	for (i = 0; i < 4; i++)
		fprintf(filePtr, " %d", stages[i]);


	fclose(filePtr);
	return 0;
}

int write_pokemon(char* filename, pokemon pokemon){
	FILE* filePtr;
	filePtr = fopen(filename, "w");
	if (filePtr == NULL)
		printf("Could not open file '%s' for reading.\n", filename);


	fprintf(filePtr, " %s", pokemon.name);
	fprintf(filePtr, " %f", pokemon.HP);
	fprintf(filePtr, " %d", pokemon.MP);
	fprintf(filePtr, " %f", pokemon.attack1);
	fprintf(filePtr, " %f", pokemon.attack2);
	fprintf(filePtr, " %d", pokemon.attack_percent);

	fclose(filePtr);
	return 0;
}



void printStatus(pokemon pokemon){
  printf("\n\t\t%s stats:\n", pokemon.name);
  printf("\t\t\tHP:     %.0f\n", pokemon.HP);
  printf("\t\t\tMP:     %d\n", pokemon.MP);
  printf("\t\t\tAtt1:   %.2f\n", pokemon.attack1);
  printf("\t\t\tAtt2:   %.2f\n", pokemon.attack2);
  printf("\t\t\tAtt%%:   %d\n", pokemon.attack_percent);
}

void backpack (char* name, pokemon* pikachu, int* potion1, int* potion2, int* potion3){
  int bp = 1;

  while (bp != 0){
    printf("\n-----------------------------------------------------------------------------");
    printf("\n\t\t\t\tWelcome to your backpack %s!\n\n", name);
    printf("\t1. View Map. \n");
    printf("\t2. View Potions. \n");
    printf("\t3. Check my pokemon stats. \n");
    printf("\t0. Exit Backpack. \n\n");
    printf("Enter an option from above: ");
    scanf("%d", &bp);

    switch(bp){
      case 1:
        map();
        break;

      case 2:
        potions(pikachu, name, potion1, potion2, potion3);
        break;

      case 3:
        pikachufront();
        printStatus(*pikachu);
        break;

      case 0:
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


void attack(pokemon* pikachu, pokemon* opponent, float attack){
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

int fight(char* name, pokemon pikachu, pokemon opponent, int stage, int* potion1,int* potion2,int* potion3){
	int fchoice;
	float hpFull = opponent.HP, PikachuHPFull = pikachu.HP;

  printf("\n\nAre you ready to battle? Yes : 1 | No : 0\n\tAnswer : ");
  scanf("%d", &fchoice);

  while (fchoice != 0) {
    if (stage == 1)
      gengar(opponent.HP, hpFull, opponent.name);
		else if (stage == 2)
      charizard(opponent.HP, hpFull, opponent.name);
		else if (stage == 3)
      zapdos(opponent.HP, hpFull, opponent.name);
		else if (stage == 4)
      mew(opponent.HP, hpFull, opponent.name);

		pikachuBack(pikachu.HP, PikachuHPFull, pikachu.name);
		srand(time(NULL));
		scanf("%d", &fchoice);

    switch(fchoice){
      case 1:
			   attack(&pikachu, &opponent, pikachu.attack1);
			   break;

      case 2:
			   attack(&pikachu, &opponent, pikachu.attack2);
         break;

      case 3:
        backpack(name, &pikachu, potion1, potion2, potion3);
        break;

      case 0:
        printf("If you exit the battle you'll be returned to the main menu\nand all progress of the battle will be lost.\n");
        printf("Are you sure you want to run away? YES : 0 | NO : 1 \n");
        scanf("%d", &fchoice);
  			pikachu.HP = 0;
        break;

      default:
        printf("Invalid option, please try again.\n");
        break;
    }

    if (pikachu.HP <= 0) {
			printf("\n\t\tYOU LOSE...\n\n");
			return 0;
		} else if (opponent.HP <= 0) {
			printf("\n\t\tYOU WON!!!!!!!!\n\n");
			return 1;
		}
  }
  return 0;
}

void select_stage(char* name, pokemon pikachu, int* potion1, int* potion2, int* potion3, int stage[4]){
	int s = 1;
	pokemon gengar, charizard, zapdos, mew;
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
    switch(s) {
      case 1:
        stage[0] = fight(name, pikachu, gengar, s, potion1, potion2, potion3);
        if (stage[0] == 1)
          printf("The stage one is complete\n");
        break;

      case 2:
        if(stage[0] == 1) {
          stage[1] = fight(name, pikachu, charizard, s, potion1, potion2, potion3);
          if (stage[1] == 1) {
            printf("The stage two is complete\n");
          }
        } else {
          printf("You have to complete previous stages to enter this stage, please try again.\n");
        }
        break;

      case 3:
        if(stage[0] == 1 && stage[1] == 1) {
          stage[2] = fight(name, pikachu, zapdos, s, potion1, potion2, potion3);
          if (stage[2] == 1) {
            printf("The stage three is complete\n");
          }
        } else {
          printf("You have to complete previous stages to enter this stage, please try again.\n");
        }
        break;

      case 4:
        if(stage[0] == 1 && stage[1] == 1 && stage[2] == 1) {
          stage[3] = fight(name, pikachu, mew, s, potion1, potion2, potion3);
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

void loop_Mainmenu(char* name,  pokemon pikachu, int numPotions[3], int stage[4]){
  int option = 1;
	char* fitems = "Items.txt";
	char* fpikachu = "PikachuTwo.txt";

  while (option != 0) {
    printf("\n-------------------------------------------------------------------------------");
    printf("\n  Welcome to the main menu %s, please choose one of the options below.\n\n", name);
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
        select_stage(name, pikachu, &numPotions[0], &numPotions[1], &numPotions[2], stage);
        break;

      case 2:
        backpack(name, &pikachu, &numPotions[0], &numPotions[1], &numPotions[2]);
        break;

      case 3:
        pikachufront();
        printStatus(pikachu);
        break;

      case 4:
  			playOnline(name, &pikachu);
        option = 0;
        break;

      case 5:
  			write_file(fitems, name, numPotions, stage);
  			write_pokemon(fpikachu, pikachu);
  			printf("\n\nCongratulations. You already saved the game\n\n");
        break;

      case 0:
        printf("\nYes : 0 | No : 1\n");
        printf("Do you already saved your game? : ");
        scanf("%d", &option);
        break;

      default:
        break;
    }
  }
}

void playOnline(char* name, pokemon * pikachu){
  char address[20];
  char port[4];
  char buffer[BUFFER_SIZE];
  int connection_fd = 0;
  printf("Enter the server address (127.0.0.1)\n");
  fflush(stdin);
  fflush(stdout);
  scanf("%s", address);
  printf("Enter the port (8989)\n");
  scanf("%s", port);
  printf("%s %s\n", address, port);
  // Start the server
  connection_fd = connectSocket("127.0.0.1", "8989");
  printf("%d\n", connection_fd);
  if (connection_fd ){
    // Prepare the message to the server
    sprintf(buffer, "%s %s %f %d %f %f %d", name, pikachu->name, pikachu->HP, pikachu->MP, pikachu->attack1, pikachu->attack2, pikachu->attack_percent);
  }
  sendString(connection_fd, buffer);
  // RECV
  // Receive the response
  if (!recvString(connection_fd, buffer, BUFFER_SIZE) ){
    printf("Server closed the connection\n");
    exit(1);
  }

  if(strncmp(buffer, "WAIT", 4) == 0)
    printf("Waiting for another player\n");
  while ( strncmp(buffer, "PLAY", 4) != 0){
    if (!recvString(connection_fd, buffer, BUFFER_SIZE) ){
       printf("Server closed the connection\n");
       exit(1);
    }
  }
  battleOnline(name, pikachu, connection_fd);
}

void battleOnline(char * name, pokemon * pikachu, int connection_fd){
  char buffer[BUFFER_SIZE];
  printf("===== WELCOME TO THE COLISEUM =====\n");
  sprintf(buffer, "READY");
  sendString(connection_fd, buffer);
  while(strncmp(buffer, "END", 3) != 0){
    if (!recvString(connection_fd, buffer, BUFFER_SIZE) ){
       printf("Server closed the connection\n");
       exit(1);
     }
     if(strncmp(buffer, "TURN", 4) == 0)
       battleAttack(name, pikachu, connection_fd);

     if(strncmp(buffer, "WAIT", 4) == 0)
       battleDefend(name, pikachu, connection_fd);
  }
}

void battleAttack(char * name, pokemon * pikachu, int connection_fd){
  float PikachuHPFull = pikachu->HP;
  char opt;
  char buffer[BUFFER_SIZE];

  pikachuBack(pikachu->HP, PikachuHPFull, pikachu->name);
  scanf("%c", &opt);
  sprintf(buffer, "%c", opt);
  sendString(connection_fd, buffer);
}

//No esta acabada
void battleDefend(char * name, pokemon * pikachu, int connection_fd){
  float HP;
  char buffer[BUFFER_SIZE];
  if (!recvString(connection_fd, buffer, BUFFER_SIZE) ){
     printf("Server closed the connection\n");
     exit(1);
   }
   sscanf(buffer, "%f %f", &pikachu->HP, &HP);
}

void GenderName(char* name){
  int gender = 0;
  while ((gender != 1)&&(gender != 2)){
      printf("\n Well %s first things first, what gender do you want to be?\n\n", name);
      genders();
      printf("\nChose an option : ");
      scanf("%d", &gender);
      switch(gender){
        case 1:
          printf("\nPerfect then, you will be him: \n\n");
          male();
          printf("\n\t%s\n", name);
          break;

        case 2:
          printf("\nPerfect now you are her: \n\n");
          female();
          printf("\n\t%s\n", name);
          break;

        default:
          printf("Invalid option. Try again\n");
          break;
    }
  }
}

void potions(pokemon* pikachu, char* name, int* potion1, int* potion2, int* potion3){
  int potion = 1;
  while (potion != 0){
    potionsPictures(*potion1, *potion2, *potion3);
    printf("Go out : 0\n\tWhat potion do you want %s? : ", name);
    scanf("%d", &potion);

    switch (potion) {
      case 1:
        if(*potion1 > 0) {
          printf("\nThis is your HP before using the potion: %.0f", pikachu->HP);
          pikachu->HP += 30;
          *potion1 -= 1;
          printf("\nThis is your HP after using the potion: %.0f\n", pikachu->HP);
        } else {
          printf("\nYou don't have any potions left\n\n");
        }
        break;

      case 2:
        if(*potion2 > 0) {
          printf("\nThis is your MP before using the potion: %d", pikachu->MP);
          pikachu->MP += 5;
          *potion2 -= 1;
          printf("\nThis is your MP after using the potion: %d\n", pikachu->MP);
        } else {
          printf("\nYou don't have any potions left\n\n");
        }
        break;

      case 3:
        if(*potion3 > 0) {
          printf("\nThis is your Attack%% before using the potion: %d%% ", pikachu->attack_percent);
          pikachu->attack_percent += 5;
          *potion3 -= 1;
          printf("\nThis is your Attack%% after using the potion: %d%%\n", pikachu->attack_percent);
        } else {
          printf("\nYou don't have any potions left\n\n");
        }
        break;

      case 0:
        break;

      default:
        printf("Invalid option, please try again.\n");
        break;
    }
  }
}

void introduction(char* name){
  printf("\n\nWell... How do they call you? : ");
  scanf("%s", name);
  GenderName(name);
  printf("\nAnd now we are going to give you a new Pokemon named Pikachu...\n");
  pikachufront();
  printf("\n... I hope you like it\n");
  printf("\n\tHere you have a map that you have to complete to become a Pokemon Master\n");
  printf("\n\tIn each stage there is a battle of differents pokemon and you have to fight\n");
  printf("\n\tYou won't be able to  pass to another level if the previous one hasn't been complete yet\n");
  printf("\n\tHere you have a lot items that you can use on your pikachu, thats all %s\n", name);
  printf("\nNow you can start... Good Luck catching them all :)\n\n");
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

void potionsPictures(int number1, int number2, int number3) {
    printf("\n\n   Potion 1  \n\n");
    printf("     888     \n");
    printf("    88888    \n");
    printf("     888      Number of potions: %d\n", number1);
    printf("    88888    \n");
    printf("   8888888    Increases your pokemon's HP by 30 points\n");
    printf("  888888888  \n");
    printf("  888888888  \n");
    printf("   8888888   \n\n\n");

    printf("   Potion 2   \n\n");
    printf("     888      \n");
    printf("    88888     \n");
    printf("     888      Number of potions: %d\n", number2);
    printf("    88888     \n");
    printf("   8888888    Increases your pokemon's MP by 5 points \n");
    printf("  888888888   \n");
    printf(" 88888888888  \n");
    printf("8888888888888 \n\n\n");

    printf("   Potion 3 \n\n");
    printf("     888   \n");
    printf("    88888  \n");
    printf("     888      Number of potions: %d\n", number3);
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
