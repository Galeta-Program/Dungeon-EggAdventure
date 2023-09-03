/***********************************************************************
 * File: main.cpp
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for game of Dungeon main program
***********************************************************************/
#include "main.h"

int main()
{
    srand(time(NULL));
    DungeonGUI dungeonGUI; // create a gui of dungeon
    dungeonGUI.exe(); // executive a gui of dungeon
    return EXIT_SUCCESS;
}

