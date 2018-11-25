#ifndef PRINTS_H
#define PRINTS_H

// Basic libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Functions that print without arguments
void genders();
void male();
void female();
void map();
void youwon();

// Function that allows to print the life bar
void HP(float hpFight, float hpFull);

// Pokemon to pick
void pikachu();
void gengar();
void charizard();
void zapdos();
void mew();

// Pokemon back on the battle
void pikachuBack(float hp, float hpFull, char* name);
void gengarBack(float hp, float hpFull, char* name);
void charizardBack(float hp, float hpFull, char* name);
void zapdosBack(float hp, float hpFull, char* name);
void mewBack(float hp, float hpFull, char* name);

// Pokemon front on the battle
void pikachuFront(float hp, float hpFull, char* name);
void gengarFront(float hp, float hpFull, char* name);
void charizardFront(float hp, float hpFull, char* name);
void zapdosFront(float hp, float hpFull, char* name);
void mewFront(float hp, float hpFull, char* name);


void genders()
{
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

void male()
{
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


void map()
{
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

void youwon()
{
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

void HP(float hpFight, float hpFull)
{
    float percent2 = 50/hpFull;
    float percent = percent2*hpFight;

    printf("HP: %.0f/%.0f ", hpFight, hpFull);
    for(int i = 0; i < percent; i++)
        printf("*");

    printf("\n\n");
}

void pikachu()
{
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

void gengar()
{
    printf("\n      8%c                /8  \n",92);
    printf("      8 8   /%c /%c /%c   8 8  \n",92,92,92);
    printf("      8  8 O  O  O  O 8  8   \n");
    printf("      8  O           O   8   \n");
    printf("     88 O             O 88   \n");
    printf("    88 O    %c      /   O 88 \n",92);
    printf("  88   O               O  88 \n");
    printf("    88  O    ^^^^^^   O  88  \n");
    printf("      8  O    ^^^^   O  8    \n");
    printf("       8   OOOOOOOOO   8     \n");
    printf("        8    8   8    8      \n");
    printf("         88888   88888       \n\n");
}

void charizard()
{
    printf("\n               /%c   /%c            \n",92,92);
    printf("               0000000              \n");
    printf("              0       0             \n");
    printf("         8888 0 O   O 0 8888        \n");
    printf("      888      0     0      888     \n");
    printf("    888         0   0         888   \n");
    printf("   888   888    0. .0    888   888  \n");
    printf("  88   88   88   000   88   88   88 \n");
    printf("   88 8      88       88      8 88  \n");
    printf("    88       88       88       88   \n");
    printf("            8    888    8           \n");
    printf("             8888   8888            \n\n");
}

void zapdos()
{
    printf("\n  888       |%c        /|       888 \n",92);
    printf("    8888  88| %c  88  / |88  8888   \n",92);
    printf("      88888  88%c8  8/88  88888     \n",92);
    printf("       88888            88888       \n");
    printf("     8888       %c  /       8888    \n",92);
    printf("   88888888    %c    /    88888888  \n",92);
    printf("      88888888  %c  /  88888888     \n",92);
    printf("         88888888%c/88888888        \n",92);
    printf("       8888888888888888888888       \n");
    printf("    888888   8888888888   888888    \n");
    printf("   888        88888888        888   \n");
    printf("               V    V               \n\n");
}

void mew()
{
    printf("\n   88             8888               \n");
    printf("  8   888 888888 8    8       88888  \n");
    printf("  8      88    88   88       8     8 \n");
    printf("   8                8       8     8  \n");
    printf("  8                8        8    8   \n");
    printf("  8o        oo      8        8   8   \n");
    printf("  8.o      o.o      8         8  8   \n");
    printf("  8oo      oo   8  8           8  8  \n");
    printf("   8          88     8          8  8 \n");
    printf("    8       88        8          8 8 \n");
    printf("     8888888            8        8 8 \n");
    printf("      8                  8      8 8  \n");
    printf("     8    8                8   8 8   \n");
    printf("    8   8 8                888 8     \n");
    printf("   8  8   88               8888      \n");
    printf("  888      88       8     88         \n");
    printf("           888      8    88          \n");
    printf("          8   8     88     88        \n");
    printf("       8 8    888888  88      8      \n");
    printf("      8/    88          88    %c8    \n",92);
    printf("      8888888             88888      \n\n");
}

void pikachuBack(float hp, float hpFull, char* name)
{
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

void gengarBack(float hp, float hpFull, char* name)
{
	printf("%s ", name);
	HP(hp, hpFull);
	printf("     88          88              \n");
    printf("     8 8  8    88  88            \n");
    printf("     8  88 8888      88          \n");
    printf("    8                |_88         \n");
    printf("   8  <                 88        \n");
    printf("  8                  ____88       \n");
    printf("  8  <   <           UUUU88      \n");
    printf("   8                     88         1. Shadow Punch |  2. Shadow Ball\n");
    printf("    8   <               88          3. Backpack     |  0. Exit Battle\n");
    printf("     8                88         \n");
    printf("-------888___________88------------------------------------------------------\n");
}

void charizardBack(float hp, float hpFull, char* name)
{
	printf("%s ", name);
	HP(hp, hpFull);
    printf("                 888O          \n");
    printf("               88   8          \n");
	printf("       ccccc888   78           \n");
    printf("          88  0   8            \n");
    printf("        cccc     8 88          \n");
    printf("      88   8    8  888         \n");
    printf("     8 8   8    8  8 88        \n");
    printf("    8  8  88    8 8    8       \n");
    printf("   8   8 88     88      8      \n");
    printf("   8   888               8          1. Flamethrower |  2. Flare Blitz\n");
    printf("   8                     8          3. Backpack     |  0. Exit Battle\n");
    printf("   8                     8    \n");
    printf("---8____________________8----------------------------------------------------\n");
}

void zapdosBack(float hp, float hpFull, char* name)
{
    printf("%s ", name);
    HP(hp, hpFull);
    printf("        8           8         \n");
    printf("        88         88         \n");
    printf("        8 8       8 8         \n");
    printf("        8  8888888  8         \n");
    printf("     888             888      \n");
    printf("        88              88>   \n");
    printf("      88             888      \n");
    printf("     8888888     8  88        \n");
    printf("          88     88 8               1. Drill Peck   |  2. Thunder\n");
    printf("        88       8 88               3. Backpack     |  0. Exit Battle\n");
    printf("      88         8  8         \n");
    printf("-----8__________8-------------------------------------------------------------\n");
}

void mewBack(float hp, float hpFull, char* name)
{
	printf("%s ", name);
	HP(hp, hpFull);
	printf("        888                   \n");
    printf("       8  8                   \n");
    printf("       8  8 8888888           \n");
    printf("        8  8       888        \n");
    printf("         8  8        888      \n");
    printf("      88  8  8        8888    \n");
    printf("     88    8  8          88   \n");
    printf("    88      8  8          88        1. Psychic      |  2. Ancient Power\n");
    printf("     88     8  8         88         3. Backpack     |  0. Exit Battle\n");
    printf("      88    8  8        88    \n");
    printf("-------88___8__8_______88--------------------------------------------------------------\n");
}


void pikachuFront(float hp, float hpFull, char* name)
{
    printf("\n----------------------------------------------------------------------------\n\t\t\t");
	printf("%s ", name);
	HP(hp, hpFull);
    printf("\n                                         88             88  \n");
    printf("                                         888           888  \n");
    printf("                                         8888         8888  \n");
    printf("                                         8   8       8   8  \n");
    printf("                                         8    8888888    8  \n");
    printf("                                         88             88    \n");
    printf("                                         88             88      77777\n");
    printf("                                         8    O     O    8     7   7 \n");
    printf("                                         8##     .     ##8   77  77  \n");
    printf("                                          8##    _    ##8   7   7   \n");
    printf("                                          88           88  7   7    \n");
    printf("                                         88             887  7    \n");
    printf("                                        8  /           %c  87      \n", 92);
    printf("                                        8 /             %c 87       \n", 92);
    printf("                                        8/               %c8        \n", 92);
    printf("                                         888    8888    888         \n");
    printf("                                        8   888888888888   8        \n");
    printf("                                         88888        88888         \n");
}


void gengarFront(float hp, float hpFull, char* name)
{
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

void charizardFront(float hp, float hpFull, char* name)
{
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

void zapdosFront(float hp, float hpFull, char* name)
{
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

void mewFront(float hp, float hpFull, char* name)
{
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

#endif  /* NOT FATAL_ERROR_H */
