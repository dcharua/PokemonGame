// Custom libraries
#include "prints.h"
#include "sockets.h"

// Library for the wait
#include <unistd.h>

// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>

// Definition of the size of the buffer that we are
// going to use for the communication
#define BUFFER_SIZE 1024

// Structure of the pokemon attributes
typedef struct pokemon {
    char name[10];
    float HP;
    int MP;
    float attack1;
    float attack2;
    int attack_percent;
} pokemon_t;

// Structure of the player attributes
typedef struct player {
    char name[10];
    int gender;
    int potions[3];
    int stages[4];
    pokemon_t * pokemon;
} player_t;

// DEFINITION OF THE FUNCTIONS

// Functions that read files
void read_pokemon(char* filename, pokemon_t* pokemon);
void read_player(char* filename, player_t* player);

// Initial functions
void loop_Mainmenu(player_t * player);
void introduction(player_t * player);
void GenderName(player_t * player);
void printStatus(player_t * player);
void backpack (player_t * player);
char potions(player_t* player);
void potionsPictures(player_t * player);

// Fight functions
void select_stage(player_t * player);
int fight(player_t * player, pokemon_t * opponent, int stage);
void attack(player_t * player, pokemon_t * opponent, float attack);

// Functions that write files
void write_file(char* filename, player_t* player);
void write_pokemon(char* filename, player_t * player);

// Online game
void playOnline(player_t * player);
void battleOnline(player_t * player, int connection_fd);
void battleDefend(player_t * player, player_t * opponent, int connection_fd, float full_HP, float opponent_full_HP);
void battleAttack(player_t * player, player_t * opponent, int connection_fd, float full_HP, float opponent_full_HP);
void checkOpAction(char action, float attack, player_t * player);
void checkAction(char action, float attack, player_t * opponent);


int main(int argC, char *argV[])
{
    player_t player;
    int savedProgress = 0;

    // Definition of the files of the players that are saved
    char* filePlayerSaved = "SavedFiles/player.txt";
    char* filePokemonSaved = "SavedFiles/pokemon.txt";

    // Allocate memory for the pokemon struct that the player have
    player.pokemon = malloc(sizeof (pokemon_t));

    printf("\t\tWELCOME TO THE WORLD OF POKEMONS\n");
    printf("*****************************************************************\n\n");
    printf("  Do you have a saved progress? { Yes: 1 | No: 2 }\n");

    while ((savedProgress != 1)&&(savedProgress != 2)) {
        printf("\tOption: ");
        scanf("%d", &savedProgress);
        getchar();
        switch(savedProgress) {
            case 1:
                read_player(filePlayerSaved, &player);
                read_pokemon(filePokemonSaved, player.pokemon);
            break;

            case 2:
                introduction(&player);
            break;

            default:
                printf("Invalid option, try again\n\n");
            break;
        }
    }

    loop_Mainmenu(&player);

    return 0;
}

void read_pokemon(char* filename, pokemon_t* pokemon)
{
    FILE* filePtr;
    filePtr = fopen(filename, "r");

    if (filePtr == NULL)
        printf("Could not open file '%s' for reading.\n", filename);

    fscanf(filePtr, " %s", pokemon->name);
    fscanf(filePtr, " %f", &pokemon->HP);
    fscanf(filePtr, " %d", &pokemon->MP);
    fscanf(filePtr, " %f", &pokemon->attack1);
    fscanf(filePtr, " %f", &pokemon->attack2);
    fscanf(filePtr, " %d", &pokemon->attack_percent);

    fclose(filePtr);
}

void read_player(char* filename, player_t* player)
{
    int i = 0;
	FILE* filePtr;
	filePtr = fopen(filename, "r");

	if (filePtr == NULL)
		printf("Could not open file '%s' for reading.\n", filename);

	fscanf(filePtr, "%s", player->name);
    fscanf(filePtr, "%d", &player->gender);

	for (i = 0; i < 3; i++)
        fscanf(filePtr, " %d", &player->potions[i]);

	for (i = 0; i < 4; i++)
        fscanf(filePtr, " %d", &player->stages[i]);

	fclose(filePtr);
}

void introduction(player_t * player)
{
    char filePokemon[30] = "DefaultFiles/";
    int pokemonOption = 0;

    printf("\n\nHello my name is Profesor Elm...\n");
    printf("¿What is your name?\n\t Name: ");
    scanf("%s", player->name);
    getchar();
    GenderName(player);

    usleep(1000000);
    printf("\nAnd now we are going to give you a new pokémon...\n");
    usleep(1000000);
    printf("\nYou have different options pick wich one you prefer\n");
    while (pokemonOption == 0) {
        printf("\n1.-Pikachu | 2.-Charizard | 3.-Gengar | 4.-Zapdos | 5.-Mew \n");
        printf("\tOption: ");
        scanf("%d", &pokemonOption);
        getchar();
        switch (pokemonOption) {
            case 1:
                strcat(filePokemon, "Pikachu.txt");
                pikachufront();
            break;

            case 2:
                strcat(filePokemon, "Charizard.txt");
            break;

            case 3:
                strcat(filePokemon, "Gengar.txt");
            break;

            case 4:
                strcat(filePokemon, "Zapdos.txt");
            break;

            case 5:
                strcat(filePokemon, "Mew.txt");
            break;

            default:
                printf("Invalid option, try again\n");
            break;
        }
    }

    read_pokemon(filePokemon, player->pokemon);

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
    int gender = 0, i = 0;

    while ((gender != 1) && (gender != 2)) {
        printf("\n Well %s first things first, what gender do you want to be?\n\n", player->name);
        usleep(1000000);
        genders();
        printf("\nChose an option : ");
        scanf("%d", &gender);
        getchar();
        switch(gender) {
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

    player->gender = gender;

    for (i = 0; i < 3; i++)
        player->potions[i] = 3;

    for (i = 0; i < 4; i++)
        player->stages[i] = 0;
}

void loop_Mainmenu(player_t * player)
{
    int option = 1;
    char* filePlayerSaved = "SavedFiles/player.txt";
    char* filePokemonSaved = "SavedFiles/pokemon.txt";

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
        getchar();
        switch (option) {
            case 1:
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
      		    write_file(filePlayerSaved, player);
      			write_pokemon(filePokemonSaved, player);
      			printf("\n\n\tCongratulations . . . You have successfully saved the game\n\n");
            break;

            case 0:
                printf("\nHave you already saved your game? { Yes: 0 | No: 1 }\n");
                printf("\tOption: ");
                scanf("%d", &option);
                getchar();
            break;

            default:
                printf("Invalid option, try again\n\n");
            break;
        }
    }
}

void select_stage(player_t * player)
{
    int stage = 1;
    pokemon_t gengar, charizard, zapdos, mew;

    //Read all the pokemons from the stages
    char* fileGengar = "DefaultFiles/Gengar.txt";
	read_pokemon(fileGengar, &gengar);

    char* fileCharizard = "DefaultFiles/Charizard.txt";
	read_pokemon(fileCharizard, &charizard);

    char* fileZapdos = "DefaultFiles/Zapdos.txt";
	read_pokemon(fileZapdos, &zapdos);

    char* fileMew = "DefaultFiles/Mew.txt";
	read_pokemon(fileMew, &mew);

    while (stage != 0) {
        map();
        printf("\nWhich stage do you want to enter? { 1 | 2 | 3 | 4 } \n\t If you want to return to the main menu { 0 }\n\n Option : ");
        scanf("%d", &stage);
        getchar();
        switch(stage) {
            case 1:
                player->stages[0] = fight(player, &gengar, stage);
                if (player->stages[0] == 1)
                    printf("The stage one is complete\n");
            break;

            case 2:
                if(player->stages[0] == 1) {
                    player->stages[1] = fight(player, &charizard, stage);
                    if (player->stages[1] == 1) {
                        printf("The stage two is complete\n");
                    }
                } else {
                    printf("You have to complete previous stages to enter this stage, please try again.\n");
                }
            break;

            case 3:
                if (player->stages[0] == 1 && player->stages[1] == 1) {
                    player->stages[2] = fight(player, &zapdos, stage);
                    if (player->stages[2] == 1) {
                        printf("The stage three is complete\n");
                    }
                } else {
                    printf("You have to complete previous stages to enter this stage, please try again.\n");
                }
            break;

            case 4:
                if (player->stages[0] == 1 && player->stages[1] == 1 && player->stages[2] == 1) {
                    player->stages[3] = fight(player, &mew, stage);
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

int fight(player_t * player, pokemon_t * opponent, int stage)
{
	int fightChoice = 1;
	float opponentHpFull = opponent->HP, playerHpFull = player->pokemon->HP;

    printf("\n\nAre you ready to battle? { Yes: 1 | No: 0 }\n\tAnswer : ");
    scanf("%d", &fightChoice);
    getchar();
    while (fightChoice != 0) {
        if (stage == 1)
            gengar(opponent->HP, opponentHpFull, opponent->name);
		else if (stage == 2)
            charizard(opponent->HP, opponentHpFull, opponent->name);
		else if (stage == 3)
            zapdos(opponent->HP, opponentHpFull, opponent->name);
		else if (stage == 4)
            mew(opponent->HP, opponentHpFull, opponent->name);

        pikachuBack(player->pokemon->HP, playerHpFull, player->pokemon->name);
        srand(time(NULL));

        scanf("%d", &fightChoice);
        getchar();
        switch(fightChoice) {
            case 1:
                attack(player, opponent, player->pokemon->attack1);
            break;

            case 2:
                attack(player, opponent, player->pokemon->attack2);
            break;

            case 3:
                backpack(player);
            break;

            case 0:
                printf("If you exit the battle you'll be returned to the main menu\nand all progress of the battle will be lost.\n");
                printf("Are you sure you want to run away? YES : 0 | NO : 1 \n");
                scanf("%d", &fightChoice);
                getchar();
                player->pokemon->HP = 0;
            break;

            default:
                printf("Invalid option, please try again.\n");
            break;
        }

        if (player->pokemon->HP <= 0) {
            printf("\n\t\tYOU LOSE...\n\n");
            player->pokemon->HP = playerHpFull;
            opponent->HP = opponentHpFull;
			return 0;
		} else if (opponent->HP <= 0) {
			printf("\n\t\tYOU WON!!!!!!!!\n\n");
            player->pokemon->HP = playerHpFull;
            opponent->HP = opponentHpFull;
			return 1;
		}
    }
  return 0;
}

void attack(player_t * player, pokemon_t * opponent, float attack)
{
    int probabilityPlayer = rand() % 100 + 1;
    float playerAttack = (player->pokemon->MP * attack);

    int opponentAttack = rand() % 2 + 1;
    int probabilityOpponent = rand() % 100 + 1;
    float opponentAttack1 = opponent->MP * opponent->attack1;
    float opponentAttack2 = opponent->MP * opponent->attack2;

    if (probabilityPlayer <= player->pokemon->attack_percent) {
      opponent->HP -= playerAttack;
	    printf("\nYou take %.0f of his HP!\n", playerAttack );
	   } else {
        printf("\nYour attack has failed\n");
    }

    if (probabilityOpponent <= opponent->attack_percent) {
        if (opponentAttack == 1) {
			player->pokemon->HP -= opponentAttack1;
			printf("\nThe opponent has taken %.0f of your HP!\n", opponentAttack1);
		} else if (opponentAttack == 2) {
			player->pokemon->HP -= opponentAttack2;
			printf("\nThe opponent has taken %.0f of your HP!\n", opponentAttack2);
		}
	} else {
        printf("\nHis attack failed\n");
    }
}

void backpack (player_t * player)
{
    int option = 1;

    while (option != 0) {
        printf("\n-----------------------------------------------------------------------------");
        printf("\n\t\t\t\tWelcome to your backpack %s!\n\n", player->name);
        printf("\t1. View Map. \n");
        printf("\t2. View Potions. \n");
        printf("\t3. Check my pokemon stats. \n");
        printf("\t0. Exit Backpack. \n\n");
        printf("Enter an option from above: ");
        scanf("%d", &option);
        getchar();

        switch(option) {
            case 1:
                map();
            break;

            case 2:
                potions(player);
            break;

            case 3:
                pikachufront();
                printStatus(player);
            break;

            case 0:
            break;

            default:
                printf("Invalid option, please try again.\n");
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

char potions(player_t * player)
{
    int potion;
    char response = '0';

    potionsPictures(player);
    printf("Go out : 0\n\tWhich potion do you want %s? : ", player->name);
    scanf("%d", &potion);
    getchar();

    switch (potion) {
        case 1:
            if (player->potions[0] > 0) {
                printf("\nThis is your HP before using the potion: %.0f", player->pokemon->HP);
                player->pokemon->HP += 30;
                player->potions[0]--;
                printf("\nThis is your HP after using the potion: %.0f\n", player->pokemon->HP);
            } else {
                printf("\nYou don't have any potions left\n\n");
            }
            response = '3';
        break;

        case 2:
            if (player->potions[1] > 0) {
                printf("\nThis is your MP before using the potion: %d", player->pokemon->MP);
                player->pokemon->MP += 5;
                player->potions[1]--;
                printf("\nThis is your MP after using the potion: %d\n",player->pokemon->MP);
            } else {
                printf("\nYou don't have any potions left\n\n");
            }
            response = '4';
        break;

        case 3:
            if (player->potions[2] > 0) {
                printf("\nThis is your Attack%% before using the potion: %d%% ", player->pokemon->attack_percent);
                player->pokemon->attack_percent += 5;
                player->potions[2]--;
                printf("\nThis is your Attack%% after using the potion: %d%%\n", player->pokemon->attack_percent);
            } else {
                printf("\nYou don't have any potions left\n\n");
            }
            response = '5';
        break;

        case 0:
        break;

        default:
            printf("Invalid option, please try again.\n");
        break;
    }

    return response;
}

void playOnline(player_t * player)
{
    //char address[20];
    //char port[4];
    char buffer[BUFFER_SIZE];
    int connection_fd = 0;

    // Start the server
    connection_fd = connectSocket("127.0.0.1", "8989");

    if (connection_fd ){
        // Send my data
        sprintf(buffer, "%s %s %f %d %f %f %d %d %d %d", player->name, player->pokemon->name, player->pokemon->HP, player->pokemon->MP, player->pokemon->attack1, player->pokemon->attack2, player->pokemon->attack_percent, player->potions[0],  player->potions[1],  player->potions[2]);
    }
    sendString(connection_fd, buffer);

    // RECV
    // Receive the response
    getMessage(connection_fd, buffer, BUFFER_SIZE);

    if(strncmp(buffer, "WAIT", 4) == 0) {
        printf("Waiting for another player\n");
    }

    while ( strncmp(buffer, "PLAY", 4) != 0) {
        getMessage(connection_fd, buffer, BUFFER_SIZE);
    }

    battleOnline(player, connection_fd);
}

void battleOnline(player_t * player, int connection_fd)
{
    char buffer[BUFFER_SIZE];
    float full_HP = player->pokemon->HP;
    player_t opponent;
    opponent.pokemon = malloc(sizeof (pokemon_t));

    // Get opponent data
    sprintf(buffer, "OPPONENT");
    sendString(connection_fd, buffer);
    getMessage(connection_fd, buffer, BUFFER_SIZE);
    sscanf(buffer, "%s %s %f %d %f %f %d %d %d %d", opponent.name,  opponent.pokemon->name, &opponent.pokemon->HP, &opponent.pokemon->MP, &opponent.pokemon->attack1, &opponent.pokemon->attack2, &opponent.pokemon->attack_percent, &opponent.potions[0], &opponent.potions[1], &opponent.potions[2]);
    float opponent_full_HP = opponent.pokemon->HP;
    sprintf(buffer, "READY");
    sendString(connection_fd, buffer);

    // Battle Loop
    printf("===== WELCOME TO THE COLISEUM =====\n");
    while(strncmp(buffer, "END", 3) != 0) {
        getMessage(connection_fd, buffer, BUFFER_SIZE);
        if(strncmp(buffer, "TURN", 4) == 0) {
            battleAttack(player, &opponent, connection_fd, full_HP, opponent_full_HP);
        } else if(strncmp(buffer, "WAIT", 4) == 0) {
            battleDefend(player, &opponent, connection_fd, full_HP, opponent_full_HP);
        }
    }
}

void battleAttack(player_t * player, player_t * opponent, int connection_fd, float full_HP, float opponent_full_HP)
{
    float attack;
    char action;
    char buffer[BUFFER_SIZE];
    int again = 1;
    gengar(opponent->pokemon->HP,  opponent_full_HP, opponent->pokemon->name);
    pikachuBack(player->pokemon->HP, full_HP, player->pokemon->name);
    //scanf(" %c", &action);
    action = getchar();
    getchar();
    while(again) {
        switch(action) {
            case '1':
                again = 0;
            break;

            case '2':
                again = 0;
            break;

            case '3':
                action = potions(player);
                printf("ation %c", action);
                if (action != '0') {
                    again = 0;
                } else {
                    again = 1;
                }
            break;

            case '0':
                sprintf(buffer, "END");
                sendString(connection_fd, buffer);
                exit(0);
            break;

            default:
                printf("Invalid option, please try again.\n");
                action = getchar();
                getchar();
            break;
        }
    }

    sprintf(buffer, "%c", action);
    sendString(connection_fd, buffer);

    // Recibo el resultado
    getMessage(connection_fd, buffer, BUFFER_SIZE);
    sscanf(buffer, "%c %f %f %f", &action, &attack, &player->pokemon->HP, &opponent->pokemon->HP);
    checkAction(action, attack, opponent);
}

void checkAction(char action, float attack, player_t * opponent)
{
    if(action == '1' || action == '2') {
        if (attack > 0) {
            printf("\n=================================\nYou Take %f of his HP\n=================================\n", attack);
        } else {
            printf("\n##################\nYour attack has failed\n##################\n");
        }
        if (opponent->pokemon->HP <= 0) {
            printf("\n!!!!!!!!!!!!!!!!!!!!!\nYOU HAVE WON\n!!!!!!!!!!!!!!!!!!!!!\n");
        }
    }
}

void battleDefend(player_t * player, player_t * opponent, int connection_fd,  float full_HP, float opponent_full_HP)
{
    float attack;
    char buffer[BUFFER_SIZE];
    char action;

    // Recibo el resultado del ataque del oponente
    sprintf(buffer, "WAITING");
    sendString(connection_fd, buffer);
    printf("Waiting for player attack\n");
    getMessage(connection_fd, buffer, BUFFER_SIZE);
    sscanf(buffer, "%c %f %f %f", &action, &attack, &player->pokemon->HP, & opponent->pokemon->HP);
    checkOpAction(action, attack, player);
}

void checkOpAction(char action, float attack, player_t * player)
{
    if(action == '1' || action == '2'){
        if (attack > 0){
            printf("\n""""""""""""""""""""""""""""""\nHe Takes %f of your HP\n""""""""""""""""""""""""""""""\n", attack);
        } else {
            printf("\n!!!!!!!!!!!!!!!!!!!!!\nHis attack has failed\n!!!!!!!!!!!!!!!!!!!!!\n");
        }
        if (player->pokemon->HP <= 0) {
            printf("\n!!!!!!!!!!!!!!!!!!!!!\nYOU HAVE LOST\n!!!!!!!!!!!!!!!!!!!!!\n");
        }
    } else if (action == '3' || action =='4' || action =='5') {
        printf("\n!!!!!!!!!!!!!!!!!!!!!\nYour opponent has taken a potion\n!!!!!!!!!!!!!!!!!!!!!\n");
    }
}

void write_file(char* filename, player_t* player)
{
    int i;
	FILE* filePtr;
	filePtr = fopen(filename, "w");

	if (filePtr == NULL)
		printf("Could not open file '%s' for writting.\n", filename);

    fprintf(filePtr, " %s", player->name);
    fprintf(filePtr, " %d", player->gender);

    for (i = 0; i < 3; i++)
		fprintf(filePtr, " %d", player->potions[i]);

	for (i = 0; i < 4; i++)
		fprintf(filePtr, " %d", player->stages[i]);

	fclose(filePtr);
}

void write_pokemon(char* filename, player_t * player)
{
    FILE* filePtr;
	filePtr = fopen(filename, "w");

	if (filePtr == NULL)
		printf("Could not open file '%s' for writting.\n", filename);

	fprintf(filePtr, " %s", player->pokemon->name);
	fprintf(filePtr, " %f", player->pokemon->HP);
	fprintf(filePtr, " %d", player->pokemon->MP);
	fprintf(filePtr, " %f", player->pokemon->attack1);
	fprintf(filePtr, " %f", player->pokemon->attack2);
	fprintf(filePtr, " %d", player->pokemon->attack_percent);

	fclose(filePtr);
}

void potionsPictures(player_t * player)
{
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
