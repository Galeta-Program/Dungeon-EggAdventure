/***********************************************************************
 * File: DungeonGUI.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for GUI of Dungeon Game main program header
***********************************************************************/
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "SaveBox.h"
#include "Dungeon.h"

using namespace std;
using namespace sf;

class DungeonGUI
{
private:
	RenderWindow *mainWindow; // the window of game
	Font* font = new Font; // the font of game
	Event event; // the event of game
	string saveName; // the name of save file

	SaveBox* saveBox1; // the box of save data1
	SaveBox* saveBox2; // the box of save data2
	SaveBox* saveBox3; // the box of save data3

	int gameState = 1; // the current screen of the game
	int soundVolume = 50; // all game volume of sound
	int bgmVolume = 50; // all game volume of bgm
	int mouseX; // original x of the mouse position

	bool isSaveMenuOpen = false; // the save menu whether opened
	bool isClick = true; // the mouse whether clicked
	bool isPause = false; // the game whether paused
	bool isOperationMethodOpen = false; // the menu of operation method whether opened
	bool isVolumeSettingOpen = false; // the menu of volume setting whether opened
	bool isSettingBGM = false; // whether setting bgm
	bool isSettingSound = false; // whether setting sound

	Texture backgroundTexture; // texture of game background
	Sprite backgroundSprite; // sprite of game background
	Texture menuBackground; // texture of save menu background
	Sprite menuSprite; // sprite of save menu background
	Texture pauseBGTexture; // texture of pause menu background
	Sprite pauseBGSprite; // sprite of pause menu background
	Texture operationMethodTexture; // texture of operation method menu
	Sprite operationMethodSprite; // sprite of operation method menu
	Texture volumeSettingTexture; // texture of setting volume menu
	Sprite volumeSettingSprite; // sprite of setting volume menu
	Texture bgmVolumeBallTexture; // texture of the objects for adjusting bgm volume
	Sprite bgmVolumeBallSprite; // sprite of the objects for adjusting bgm volume
	Texture bgmVolumeLineTexture; // texture of the object used to view bgm volume ranges
	Sprite bgmVolumeLineSprite; // sprite of the object used to view bgm volume ranges
	Texture soundVolumeBallTexture; // texture of the objects for adjusting sound volume
	Sprite soundVolumeBallSprite; // sprite of the objects for adjusting sound volume
	Texture soundVolumeLineTexture; // texture of the object used to view sound volume ranges
	Sprite soundVolumeLineSprite; // sprite of the object used to view sound volume ranges

	RectangleShape operationMethodButton; // the button to switch to the operation method menu
	RectangleShape volumeSettingButton; // the button to switch to the setting volume menu
	RectangleShape backButton; // the button to close save menu

	Text backButtonText; // the text of close save menu
	Text bgmVolumeText; // the text of bgm volume
	Text soundVolumeText; // the text of sound volume

	Sound menuBGM; // player of main menu

public:
	// Intent:	default constructor
	// Pre:		None
	// Post:	None
	DungeonGUI();

	// Intent:	screen of game main menu
	// Pre:		None
	// Post:	None
	void homeScreen();

	// Intent:	screen of main game
	// Pre:		None
	// Post:	None
	void gameScreen();

	// Intent:	check event of save menu
	// Pre:		None
	// Post:	whether boxes of save clicked 
	bool saveMeunEvent();

	// Intent:	draw the objects of save menu on window of game
	// Pre:		None
	// Post:	None
	void drawSaveMeun();

	// Intent:	check event of setting menu
	// Pre:		None
	// Post:	None
	void settingEvent();

	// Intent:	draw the objects of setting menu on window of game
	// Pre:		None
	// Post:	None
	void drawSetttingMeun();

	// Intent:	executive the gui of dungeon
	// Pre:		None
	// Post:	None
	void exe();
};