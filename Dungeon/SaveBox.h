/***********************************************************************
 * File: SaveBox.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for box of game save
***********************************************************************/
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdio>
#include "Position.h"
#include "Size.h"

using namespace std;
using namespace sf;

class SaveBox
{
private:
	RenderWindow* mainWindow; // the window of game
	Font font; // the font of game
	fstream saveFile; // save file
	
	Sprite playerSprite; // sprite of player icon
	Texture playerTexture; // texture of player icon
    Sprite deleteSprite; // sprite of delete save icon
	Texture deleteTexture; // texture of delete save icon
	Sprite deleteYesSprite; // sprite of yes delete save icon
	Texture deleteYesTexture; // texture of yes delete save icon
	Sprite deleteNoSprite; // sprite of no delete save icon
	Texture deleteNoTexture; // texture of no delete save icon
	Sprite buttonSprite; // sprite of button to read save or create save
	Texture buttonTexture; // texture of button to read save or create save
	vector<Text> buttonText; // texts of the save information 
	
	string fileName; // this save file name
	string rank = ""; // rank of player
	string currentHp = ""; // current hp of player
	string maxHp = ""; // max hp of player
	string currentMp = ""; // current mp of player
	string maxMp = ""; // max mp of player
	string coin = ""; // amount of player coins
	string atk = ""; // atk of player
	string levels = ""; // current game levels

	bool haveData; // the save file whether have data
	bool armors[3]; // what armor is the player currently wearing

	int weaponID; // the ID of the weapon the player is currently wielding

public:
	// Intent:	setting constructor
	// Pre:		the window of game, save file name
	// Post:	None
	SaveBox(RenderWindow* window, string _fileName)
	{
		mainWindow = window;
		fileName = _fileName;		
		font.loadFromFile("resources/sansation.ttf"); // set font
	    
		// create texts to save informations
		for (int i = 0; i < 6; i++)
		{
			Text text("", font, 50);
			text.setFillColor(Color::Black);
			buttonText.push_back(text);
		}

		haveData = loadSaveFile(); // call loadSaveFile() to load file, and check whether have data

		// set the button of delete save
		deleteTexture.loadFromFile("./image/deleteData.png");
		deleteSprite.setTexture(deleteTexture);
		deleteSprite.setPosition(buttonSprite.getPosition().x + 850, (200 + buttonSprite.getPosition().y));

		// set set the button of yes delete save
		deleteYesTexture.loadFromFile("./image/deleteData_Yes.png");
		deleteYesSprite.setTexture(deleteYesTexture);
		deleteYesSprite.setScale(0, 0);
		deleteYesSprite.setPosition(buttonSprite.getPosition().x + 800, (200 + buttonSprite.getPosition().y) + 30);

		// set set the button of no delete save
		deleteNoTexture.loadFromFile("./image/deleteData_No.png");
		deleteNoSprite.setTexture(deleteNoTexture);
		deleteNoSprite.setScale(0, 0);	
		deleteNoSprite.setScale(0, 0);	
		deleteNoSprite.setPosition(buttonSprite.getPosition().x + 900, (200 + buttonSprite.getPosition().y));

		if (haveData) // if the save file have data
		{
			// set the button is read save 
			buttonTexture.loadFromFile("./image/havesavebox.png");
			buttonSprite.setTexture(buttonTexture);
			buttonSprite.setPosition((mainWindow->getSize().x - buttonSprite.getTexture()->getSize().x) / 2, (mainWindow->getSize().y - buttonSprite.getTexture()->getSize().y) / 2);
			buttonSprite.setScale(2, 2);	
		}
		else
		{
			// set the button is create save 
			buttonTexture.loadFromFile("./image/nosavebox.png");
			buttonSprite.setTexture(buttonTexture);
			buttonSprite.setPosition((mainWindow->getSize().x - buttonSprite.getTexture()->getSize().x) / 2, (mainWindow->getSize().y - buttonSprite.getTexture()->getSize().y) / 2);
			buttonSprite.setScale(2, 2);
			deleteSprite.setScale(0, 0); // hide the button of delete save
		}
	}

	// Intent:	set position of save box all objects 
	// Pre:		x of position, y of position
	// Post:	None
	void setPosition(int x, int y)
	{
		buttonSprite.setPosition(x, y);
		resetPosition(); // call resetTextPosition() to set all objects of save position
	}

	// Intent:	set size of save box
	// Pre:		width, height
	// Post:	None
	void setSize(float width, float height)
	{
		// set scale of save box to adjust size
		buttonSprite.setScale(width / buttonSprite.getTexture()->getSize().x, height / buttonSprite.getTexture()->getSize().y);
	}

	// Intent:	get position of save box
	// Pre:		None
	// Post:	the position of save box
	Position getPosition()
	{
		return Position(buttonSprite.getPosition().x, buttonSprite.getPosition().y);
	}

	// Intent:	get size of save box
	// Pre:		None
	// Post:	the size of save box
	Size getSize()
	{
		return Size(buttonSprite.getTexture()->getSize().x, buttonSprite.getTexture()->getSize().y);
	}

	// Intent:	draw save box all objects to the window of game
	// Pre:		None
	// Post:	None
	void drawInWindow()
	{
		// draw all objects to the window of game
		mainWindow->draw(buttonSprite);
        mainWindow->draw(playerSprite);

		for (int i = 0; i < 6; i++) mainWindow->draw(buttonText[i]);
						
		mainWindow->draw(deleteSprite);
		mainWindow->draw(deleteYesSprite);
		mainWindow->draw(deleteNoSprite);
	}

	// Intent:	check click event of save box 
	// Pre:		event of game
	// Post:	if save file have data return file name
	string isClick(Event event)
	{
		// if left mouse is click save box
		if (buttonSprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
		{
			if (haveData) // if save file have data
			{
				return fileName; // return file name
			}
			else // if save file haven't data
			{
				// write initial game data to save file
				ofstream file("./resources/data/" + fileName + ".txt");
				file << "1 10 10 10 10 1 0 0 0 0 0 0 0 15 0" << endl;

				for (int i = 0; i < 8; i++) 
					file << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" << endl;

				file.close();
				haveData = loadSaveFile(); // call loadSaveFile() to load file, and check whether have data

				// set the button is read save 
				buttonTexture.loadFromFile("./image/havesavebox.png");
				buttonSprite.setTexture(buttonTexture);
				deleteSprite.setScale(1, 1);
				playerSprite.setScale(1, 1);
			}
		}

		// if left mouse click the button of delete save
		if (deleteSprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
		{
			deleteYesSprite.setScale(0.5, 0.5); // appear button of yes delelte
			deleteNoSprite.setScale(1, 1); // appear button of no delelte
			deleteSprite.setScale(0, 0); // hide button of yes delelte
		}
		else if (deleteYesSprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) // if left mouse click the button of yes delete save
		{
			int result = remove(("./resources/data/" + fileName + ".txt").c_str()); // delete save file
			deleteYesSprite.setScale(0, 0); // hide button of yes delete 
			deleteNoSprite.setScale(0, 0); // hide button of no delete 
			deleteSprite.setScale(0, 0); // hide button of delete 
			playerSprite.setScale(0, 0); // hide player icon
			haveData = false;
			rank = currentHp = maxHp = currentMp = maxMp = atk = coin = levels = "";
			setInfo(); // call setInfo() to set texts of save information, positions, and player icon

			// set the button is create save 
			buttonTexture.loadFromFile("./image/nosavebox.png");
			buttonSprite.setTexture(buttonTexture);
		}
		else if (deleteNoSprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) // if left mouse click the button of no delete save
		{
			deleteYesSprite.setScale(0, 0); // hide button of yes delete 
			deleteNoSprite.setScale(0, 0); // hide button of no delete 
			deleteSprite.setScale(1, 1); // appear button of delete 
		}

		return "";
	}

	// Intent:	get save file name
	// Pre:		None
	// Post:	file name
	string getFileName()
	{
		return fileName;
	}

	// Intent:	load date of save file
	// Pre:		None
	// Post:	save file whether have data
	bool loadSaveFile()
	{
		saveFile.open("./resources/data/" + fileName + ".txt");

		if (saveFile.is_open())
		{
			string inputData = "";
			getline(saveFile, inputData); // get a line from save file 
			stringstream ss(inputData);
			ss >> rank >> currentHp >> maxHp >> currentMp >> maxMp >> atk >> coin >> levels >> armors[0] >> armors[1] >> armors[2] >> weaponID;
			setInfo(); // call setInfo() to set texts of save information, positions, and player icon
			saveFile.close();
		}
		else return false; // if save file can't open, return false

		return true;
	}

	// Intent:	set texts of save information, positions, and player icon
	// Pre:		None
	// Post:	None
	void setInfo()
	{
		// set texts of save information
		buttonText[0].setString(rank);
		buttonText[1].setString(maxHp);
		buttonText[2].setString(maxMp);
		buttonText[3].setString(atk);
		buttonText[4].setString(coin);
		buttonText[5].setString(levels);

		// set player icon
		if (weaponID >= 0 && weaponID <= 4) playerTexture.loadFromFile("./image/EGG/id_" + to_string(weaponID) + "/front_" + to_string(armors[0]) + to_string(armors[1]) + to_string(armors[2]) + to_string(weaponID) + ".png");
		else playerTexture.loadFromFile("./image/EGG/id_" + to_string(0) + "/front_" + to_string(armors[0]) + to_string(armors[1]) + to_string(armors[2]) + to_string(0) + ".png");

		playerSprite.setTexture(playerTexture);

		resetPosition(); // call resetTextPosition() to set all objects of save position
	}

	// Intent:	reset all objects of save postion
	// Pre:		None
	// Post:	None
	void resetPosition()
	{
		// set all objects of save postion
		buttonText[0].setPosition(buttonSprite.getPosition().x + 10 - rank.length() * 10, buttonSprite.getPosition().y);
		buttonText[1].setPosition(buttonSprite.getPosition().x + buttonSprite.getPosition().x / 2.5, buttonSprite.getPosition().y + 20);
		buttonText[2].setPosition(buttonSprite.getPosition().x + buttonSprite.getPosition().x / 2.5, buttonSprite.getPosition().y + 93);
		buttonText[3].setPosition(buttonSprite.getPosition().x + buttonSprite.getPosition().x / 2.5 + 300, buttonSprite.getPosition().y + 20);
		buttonText[4].setPosition(buttonSprite.getPosition().x + buttonSprite.getPosition().x / 2.5 + 300, buttonSprite.getPosition().y + 93);
		buttonText[5].setPosition(buttonSprite.getPosition().x + 710 - levels.length() * 10, buttonSprite.getPosition().y + 80);

		playerSprite.setPosition(buttonSprite.getPosition().x - playerTexture.getSize().x / 2, buttonSprite.getPosition().y);

		deleteSprite.setPosition(buttonSprite.getPosition().x + 820, buttonSprite.getPosition().y + 50);
		deleteYesSprite.setPosition(buttonSprite.getPosition().x + 780, buttonSprite.getPosition().y + 80);
		deleteNoSprite.setPosition(buttonSprite.getPosition().x + 880, buttonSprite.getPosition().y + 50);
	}
};
