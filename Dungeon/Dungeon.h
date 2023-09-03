/***********************************************************************
 * File: Dungeon.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for Dungeon main program header
***********************************************************************/
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <string>
#include "fstream"
#include "sstream"
#include "Hero.h"
#include "MerchantShop.h"
#include "Slime.h"
#include "HermitCrab.h" 
#include "IronsmithShop.h"
#include "Turtle.h"
#include "Dragon.h"
#include "Room.h"
#define HERO_INFO_NUM 5 // number of hero information

using namespace std;
using namespace sf;

class Dungeon
{
private:
	RenderWindow* mainWindow; // the window of game
	Font* font = new Font; // the font of game
	string fileName; // save file name
	fstream saveFile; // used to read and write save file 
	Hero* gHero; // hero
	vector<Creature*> creatures; // creatures
	MerchantShop* merchantShop; // merchant shop
	IronsmithShop* ironsmithShop; // ironsmith shop
	vector<vector<Room*>> rooms; // rooms of a level
	vector<Sprite*> collisionSprites; // obstacles of a level

	vector<Item> heroBackpack; // items of hero backpack
	vector<Item> heroInventory; // items of hero inventory
	vector<Item> heroEquipment; // items of hero equipment

	Sprite savePointSprite; // sprite of market save point
	Texture savePointTexture; // texture of market save point
	Sprite entranceSprite; // sprite of doungeon entrance
	Texture entranceTexture; // texture of doungeon entrance
	Sprite characterMenuSprite; // sprite of character menu
	Texture characterMenuTexture;  // texture of character menu
	Sprite chooseSprite; // sprite of inventory selection box
	Texture chooseTexture; // texture of inventory selection box

	vector<Sprite> heroInfoSprite; // sprites of hero information icon
	vector<Texture*> heroInfoTexture; // textures of hero information icon
	vector<Text> heroInfoText; // texts of hero information

	Text characterMenuInfo1, characterMenuInfo2, characterMenuInfo3; // texts of character menu information
	Text savedText; // text of saved
	
	int nowLevel = 0; // the number of levels where the hero is now, with ten levels as a unit
	int totalLevel; // the number of dungeon levels
	int inventoryIndex = 0; // index of inventory selection
	float correctionSpeed = 0; // correction speed
	int homecomingMagicSaveLevel = -1; // used to save the number of levels where the hero is now
	int maxLevel = 10; // the number of levels that the maximum width number of rooms will increase next time
	int minLevel = 0;
	int roomMaxSize = 2; // the maximum width number of rooms
	int roomNum; // used to count the rooms when creating a level
	int conditionDeno; // the denominator of the condition that opens the door to the next level
	int conditionNume; // the numerator of the condition that opens the door to the next level
	int conditionType; // type of the condition that opens the door to the next level
	int soundVolume = 50; // volume of dungeon sound 
	int bgmVolume = 50; // volume of dungeon bgm

	vector<int> roomTotalSize; // the maximum width number of room for ten levels
	vector<vector<vector<int>>> roomPattern; // the pattern of room for ten levels

	Position roomStartPos, roomExitPos; // the start room position and exit room position of current level
	vector<Position> randExitPos; // positions where exit rooms can be generated

	bool buttonCanClick = true; // button whether can be clicked again
	bool isBackpackOpen = false; // hero backpack whether open
	bool keyECanClick = true; // key E whether can be pressed again
	bool isCanScroll = true; // mouse wheel whether can be scrolled again
	bool isExitOpen = false; // the exit of level whether open
	bool savedTextShow = false; // the text of saved whether appear
	bool isBossRoomCreate = true; // the boss room whether need be generated

	clock_t savedTextTime; // used to calculate the text of saved move time
	clock_t keyETime; // used to calculate key E next can pressed against time
	clock_t pauseTime = clock(); // used to calculate menu of pause can be call time

	vector<SoundBuffer> dungeonBGMBuffers; // bgms of dungeon
	SoundBuffer storeBGMBuffer; // bgm of market 
	SoundBuffer walkingSoundBuffer; // the sound of hero walking
    SoundBuffer buySoundBuffer, drinkSoundBuffer, backHomeSoundBuffer; // the sound of buy items, drink potions, used homecoming magic
	SoundBuffer swordSoundBuffer, bowSoundBuffer, magicSoundBuffer; // the sounds of weapon
	Sound storeBGM, dungeonBGM, attackSound, walkingSound, actionSound; // players of bgms and sounds

public:
	// Intent:	setting constructor
	// Pre:		the window of game, save file name, font of game
	// Post:	None
	Dungeon(RenderWindow* window, string _fileName, Font* _font);

	// Intent:	get the number of levels where the hero is now, with ten levels as a unit
	// Pre:		None
	// Post:	the number of levels where the hero is now, with ten levels as a unit
	int getNowLevel();

	// Intent:	deal with event of dungeon
	// Pre:		event of game
	// Post:	None
	void systemEvent(Event* event);

	// Intent:	test move to next position of hero, if nowLevel isn't 0, don't move the hero. Instead, move other objects 
	//          to keep the hero in the center of the screen		
	// Pre:		x of position, y of position
	// Post:	hero whether movable
	bool tryMove(int x, int y);

	// Intent:	draw the objects to the window of game	
	// Pre:		None
	// Post:	None
	void drawInWindow();

	// Intent:	set and draw the hero informations
	// Pre:		None
	// Post:	None
	void drawInfo();

	// Intent:	get the menu of merchant shop whether open
	// Pre:		None
	// Post:	the menu of merchant shop whether open
	bool isMerchantShopMenuOpen();

	// Intent:	get the menu of ironsmith shop whether open
	// Pre:		None
	// Post:	the menu of ironsmith shop whether open
	bool isIronsmithShopMenuOpen();

	// Intent:	get the menu of hero backpack whether open
	// Pre:		None
	// Post:	the menu of hero backpack whether open
	bool isHeroBackpackOpen();

	// Intent:	get the menu of pause can open timer 
	// Pre:		None
	// Post:	the menu of pause can open timer 
	clock_t getPauseTime();

	// Intent:	set volume of all bgms
	// Pre:		volume of bgm 
	// Post:	None
	void setBGMVolume(int volume);

	// Intent:	set volume of all sounds
	// Pre:		volume of sound
	// Post:	None
	void setSoundVolume(int volume);

	// Intent:	play the animation of hero dead
	// Pre:		None
	// Post:	None
	void deadAnimation();

	// Intent:	set and draw the menu of hero backpack
	// Pre:		None
	// Post:	None
	void drawBackpack();

	// Intent:  create a level of dungeon
	// Pre:		None
	// Post:	None
	void createLevel();

	// Intent:  create ten pattern of level
	// Pre:		None
	// Post:	None
	void createLevelPattern();

	// Intent:  create a pattern of room
	// Pre:		the number of level, current x of position, current y of position, generation probability 
	// Post:	None
	void randCreateLevel(int levelNum, int x, int y, float prob = 0.4);

	// Intent: Randomly select true or false based on a given probability
	// Pre:    prob - probability value between 0 and 1 (inclusive)
	// Post:   Returns a random result (true or false) based on the probability
	bool randomSelect(float prob);

	// Intent: update the condition of open exit door
	// Pre:    None
	// Post:   the condition whether complete
	bool updateCondition();

	// Intent: random the condition for opening the exit door
	// Pre:    None
	// Post:   None
	void randCondition();

	// Intent: perform attack events between the hero and creatures
	// Pre:    None
	// Post:   None
	void attackEvent();

	// Intent: Save the game progress and data to save file
	// Pre:    None
	// Post:   None
	void save();

	// Intent: Read and load game progress and data from save file
	// Pre:    None
	// Post:   None
	void readSave();

protected:

	// Intent: Calculate the nth Fibonacci number recursively
	// Pre: a number
	// Post: Returns the num-th Fibonacci number
	int fibonacci(int num);

	// Intent: find the first available space
	// Pre: two-dimensional vector
	// Post: the index of the first available space in the vector, or -1 if no space is available
	int getSpace(vector<vector<int>>* arr);

	// Intent: find the index of the same item id
	// Pre: two-dimensional vector, id of wnat to find item
	// Post: the index of the first occurrence of the same item id in the vector, or -1 if the item is not found
	int findItem(vector<vector<int>>* arr, int id);
};