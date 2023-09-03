/***********************************************************************
 * File: Enum.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for store enums of game
***********************************************************************/
#pragma once

// id of game items
enum ItemsID
{
	Null = 0,
	Sword = 1,
	MagicWand = 2,
	Bow = 3,
	Bomb = 4,
	HPWater = 5,
	MPWater = 6,
	HomecomingMagic = 7,
	Helmet = 8,
	Breastplate = 9,
	Boots = 10,
	SwordLevelUp = 11,
	MagicWandLevelUp = 12,
	BowLevelUp = 13,
	BombLevelUp = 14,
	HelmetLevelUp = 15,
	BreastplateLevelUp = 16,
	BootsLevelUp = 17,
	Arrow = 18,
};

// state of hero
enum State
{
	None = 0,
	Frozen = 1,
};

// type of the exit door open condition 
enum Condition
{
	KillAllCreature = 0,
	CompletePuzzle = 1,
};

// id of game creatures
enum CreatureID
{
	Slime = 0,
	HermitCrab = 1,
	Turtle = 2,
	Dragon = 3,
};