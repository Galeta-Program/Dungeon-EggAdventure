/***********************************************************************
 * File: Dungeon.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for Room of a Level main program header
***********************************************************************/
#pragma once
#include <SFML/Graphics.hpp>
#include "Position.h"
#include "Puzzle.h"
#include "Hero.h"
#include "ColdHoleBullet.h"
#define ROOM_SIZE 2000 // the size of room

using namespace std;
using namespace sf;

class Room
{
private:
	RenderWindow* mainWindow; // the window of game
	vector<clock_t> startTimes; // used to update mechanism timers
	vector<Text> conditionText; // the texts of open exit door condition
	vector<Position> creaturePos; // positions of creature can be generated 
	vector<Puzzle*> puzzles; // the puzzle games
	Position startPos; // the starting position of generated objects
	Hero* gHero; // hero 
	Sprite backgroundSprite; // sprite of room background
	Texture backgroundTexture; // texture of room background

	vector<Sprite> objectSprite; // sprites of wall 
	vector<Texture> objectTexture; // textures of wall
	vector<Sprite>  lightningSprite; // sprites of lightning
	vector<Texture> lightningTexture; // textures of lightning
	vector<Sprite>  smallLaserSprite; // sprites of small laser
	vector<Texture> smallLaserTexture; // textures of small laser
	vector<Sprite>  ballSprite; // sprites of ball
	vector<Texture>  ballTexture; // textures of ball
	vector<Sprite> exitSprite; // sprites of exit
	vector<Texture> exitTexture; // textures of exit
	vector<Sprite>  coldHoleSprite; // sprites of cold hole
	vector<Texture> coldHoleTexture; // textures of cold hole
	vector<vector<ColdHoleBullet*>> coldHoleBullets; // bullets of cold hole
		
	int style = -1;	// the room style
	float rotationAngle = 0; // angle of ball rotation
	float rotationSpeed = 0.006; // speed of ball rotation
	float radius = 700; // radius of ball rotation
	vector<float> deltas; // the delta of mechanism

public:
	// Intent:	setting constructor
	// Pre:		the window of game, hero
	// Post:	None
	Room(RenderWindow* window, Hero *hero)
	{
		mainWindow = window;
		gHero = hero;

		// set the room background 
		backgroundTexture.loadFromFile("./image/levelBackground.png");
		backgroundSprite.setTexture(backgroundTexture);

		startPos.x = 0;
		startPos.y = 0;

		// add the wall into room and set wall  
		objectSprite.assign(15, Sprite());
		objectTexture.assign(15, Texture());

		objectTexture[0].loadFromFile("./image/Wall/rowWall.png");
		objectSprite[0].setPosition(startPos.x, startPos.y);
		objectSprite[0].setTexture(objectTexture[0]);
		objectTexture[1].loadFromFile("./image/Wall/colWall.png");
		objectSprite[1].setPosition(startPos.x, startPos.y);
		objectSprite[1].setTexture(objectTexture[1]);

		objectTexture[2].loadFromFile("./image/Wall/rowWall.png");
		objectSprite[2].setPosition(startPos.x + ROOM_SIZE - objectTexture[2].getSize().x, startPos.y);
		objectSprite[2].setTexture(objectTexture[2]);
		objectTexture[3].loadFromFile("./image/Wall/colWall.png");
		objectSprite[3].setPosition(startPos.x + ROOM_SIZE - objectTexture[3].getSize().x, startPos.y);
		objectSprite[3].setTexture(objectTexture[3]);

		objectTexture[4].loadFromFile("./image/Wall/rowWall.png");
		objectSprite[4].setPosition(startPos.x, startPos.y + ROOM_SIZE - objectTexture[4].getSize().y);
		objectSprite[4].setTexture(objectTexture[4]);
		objectTexture[5].loadFromFile("./image/Wall/colWall.png");
		objectSprite[5].setPosition(startPos.x, startPos.y + ROOM_SIZE - objectTexture[5].getSize().y);
		objectSprite[5].setTexture(objectTexture[5]);

		objectTexture[6].loadFromFile("./image/Wall/rowWall.png");
		objectSprite[6].setPosition(startPos.x + ROOM_SIZE - objectTexture[6].getSize().x, startPos.y + ROOM_SIZE - objectTexture[6].getSize().y);
		objectSprite[6].setTexture(objectTexture[6]);
		objectTexture[7].loadFromFile("./image/Wall/colWall.png");
		objectSprite[7].setPosition(startPos.x + ROOM_SIZE - objectTexture[7].getSize().x, startPos.y + ROOM_SIZE - objectTexture[7].getSize().y);
		objectSprite[7].setTexture(objectTexture[7]);
	}

	bool isExitOpen = false; // the exit door whether open

	// Intent:	get sprites of wall
	// Pre:		None
	// Post:	sprites of wall
	vector<Sprite>* getSpirte()
	{
		return &objectSprite;
	}

	// Intent:	move all objects of room
	// Pre:		delta x, delta y
	// Post:	None
	void move(int x, int y)
	{
		// move start position
		startPos.x += x;
		startPos.y += y;

		// move walls
		for (int i = 0; i < objectSprite.size(); i++)
		{
			objectSprite[i].move(x, y);
		}

		// move lightnings
		for (int i = 0; i < lightningSprite.size(); i++)
		{
			lightningSprite[i].move(x, y);
		}

		// move small lasers
		for (int i = 0; i < smallLaserSprite.size(); i++)
		{
			smallLaserSprite[i].move(x, y);
		}

		// move balls
		for (int i = 0; i < ballSprite.size(); i++)
		{
			ballSprite[i].move(x, y);
		}

		// move clod holes
		for (int i = 0; i < coldHoleSprite.size(); i++)
		{
			coldHoleSprite[i].move(x, y);

			for (int j = 0; j < coldHoleBullets[i].size(); j++) coldHoleBullets[i][j]->move(x, y);
		}

		// move exits
		for (int i = 0; i < exitSprite.size(); i++)
		{
			exitSprite[i].move(x, y);
		}

		// move the texts of condition
		for (int i = 0; i < conditionText.size(); i++)
		{
			conditionText[i].move(x, y);
		}

		// move puzzles
		for (int i = 0; i < puzzles.size(); i++)
		{
			puzzles[i]->move(x, y);
		}

		// move positions of creature can be generated 
		for (int i = 0; i < creaturePos.size(); i++)
		{
			creaturePos[i].x += x;
			creaturePos[i].y += y;
		}

		// move background of room
		backgroundSprite.move(x, y);
	}

	// Intent:	get start position
	// Pre:		None
	// Post:	start position
	Position getStartPos()
	{
		return startPos;
	}

	// Intent:	get a sprite of exit
	// Pre:		None
	// Post:	a sprite of exit
	Sprite* getExitSprite()
	{
		// style 2 is exit room, and return first sprite of exit
		if (style == 2 && exitSprite.size() > 0) return &exitSprite[0];
	}

	// Intent:	get positions of creature can be generated 
	// Pre:		None
	// Post:	positions of creature can be generated 
	vector<Position>* getCreaturePos()
	{
		return &creaturePos;
	}

	// Intent:	set room style
	// Pre:		room style
	// Post:	None
	void setRoomStyle(int _style)
	{
		if (style != -1) return;

		style = _style;

		switch (style)
		{
		case 0:
			room0Style();
			break;
		case 1:
			room1Style();
			break;
		case 2:
			room2Style();
			break;
		case 3:
			room3Style();
			break;
		case 4:
			room4Style();
			break;
		case 5:
			room5Style();
			break;
		case 6:
			room6Style();
			break;
		case 7:
			room7Style();
			break;
		case 8:
			room8Style();
			break;
		case 9:
			room9Style();
			break;
		case 10:
			room10Style();
			break;
		case 11:
			room11Style();
			break;
		}
	}

	// Intent:	update all mechanism state
	// Pre:		current time, correction speed of game, event of game
	// Post:	if pressed right mouse on the exit of level will return true
	bool update(clock_t endTime, float correctionSpeed, Event* event)
	{
		if (FloatRect(0, 0, mainWindow->getSize().x, mainWindow->getSize().y).intersects(backgroundSprite.getGlobalBounds()))
		{
			Sprite sprite = gHero->getSprite(); // give sprite of hero to sprite to simulate

		// decrease the size of sprite to simulate
			sprite.setPosition(sprite.getPosition().x + 100, sprite.getPosition().y + 100);
			sprite.setScale(0.3, 0.3);

			for (int i = 0; i < lightningSprite.size(); i++)
			{
				if (endTime - startTimes[i] <= 800 - correctionSpeed * 20)
				{
					// if time up sprite of lightning change the texture
					lightningSprite[i].setTexture(lightningTexture[0 + 3 * i]);
				}
				else if (endTime - startTimes[i] > 800 - correctionSpeed * 20 && endTime - startTimes[i] <= 1600 - correctionSpeed * 20)
				{
					// if time up sprite of lightning change the texture
					lightningSprite[i].setTexture(lightningTexture[1 + 3 * i]);
				}
				else if (endTime - startTimes[i] > 1600 - correctionSpeed * 20 && endTime - startTimes[i] <= 2400 - correctionSpeed * 20)
				{
					// if time up sprite of lightning change the texture
					lightningSprite[i].setTexture(lightningTexture[2 + 3 * i]);
				}

				if (endTime - startTimes[i] > 2400 - correctionSpeed * 20)
				{
					// if time up and sprite to simulate overly in sprite of lightning
					if (sprite.getGlobalBounds().intersects(lightningSprite[i].getGlobalBounds()))
					{
						gHero->damage(gHero->getMaxHP() / 3); // hero get the damage
					}

					startTimes[i] = clock(); // reset timer
				}
			}

			for (int i = 0; i < smallLaserSprite.size(); i++)
			{
				// small laser will keep going up or going down
				smallLaserSprite[i].move(0, deltas[i] + (deltas[i] > 0 ? correctionSpeed : -correctionSpeed));

				// if small laser are over the movable range, the delta will equal to negative delta
				if (smallLaserSprite[i].getPosition().y <= 100 + startPos.y || smallLaserSprite[i].getPosition().y >= 1800 + startPos.y) deltas[i] = -deltas[i];

				// if sprite to simulate overly in small laser,
				if (sprite.getGlobalBounds().intersects(smallLaserSprite[i].getGlobalBounds()))
				{
					if (endTime - startTimes[i] >= 1000) // if time up
					{
						gHero->damage(gHero->getMaxHP() / 4); // hero get damage
						startTimes[i] = clock(); // reset time
					}
				}
			}

			for (int i = 0; i < ballSprite.size(); i++)
			{
				// if is even time
				if (i % 2 == 0)
				{
					rotationAngle += rotationSpeed + correctionSpeed * 0.008; // speed of ball rotation add to the angle of ball rotation
					radius += deltas[i]; // delta add to radius
					if (radius >= 800 || radius <= 0) deltas[i] = -deltas[i]; // if greater or less than the range, the delta will equal to negative delta
				}

				// set next position of ball, the number of ball is even and odd will be on the contrary 
				float x = (i % 2 == 0) ? (startPos.x + ROOM_SIZE / 2) + radius * cos(rotationAngle) : (startPos.x + ROOM_SIZE / 2) - radius * cos(rotationAngle);
				float y = (i % 2 == 0) ? (startPos.y + ROOM_SIZE / 2) + radius * sin(rotationAngle) : (startPos.y + ROOM_SIZE / 2) - radius * sin(rotationAngle);
				ballSprite[i].setPosition(x, y);

				// if sprite to simulate overly in ball,
				if (sprite.getGlobalBounds().intersects(ballSprite[i].getGlobalBounds()))
				{
					if (endTime - startTimes[i] >= 1000) // if time up
					{
						gHero->damage(gHero->getMaxHP() / 4); // hero get damage
						startTimes[i] = clock(); // reset time
					}
				}
			}

			for (int i = 0; i < coldHoleSprite.size(); i++)
			{
				if (clock() - startTimes[i] >= 3500) // if time up
				{
					startTimes[i] = clock(); // reset time

					// add a bullet of cold hole
					coldHoleBullets[i].push_back(new ColdHoleBullet(mainWindow, gHero, Position(coldHoleSprite[i].getPosition().x, coldHoleSprite[i].getPosition().y), Size(coldHoleTexture[i].getSize().x, coldHoleTexture[i].getSize().y), 1.8 + correctionSpeed));
				}

				for (int j = 0; j < coldHoleBullets[i].size(); j++)
				{
					coldHoleBullets[i][j]->moveBullet(); // bullet move

					// if time of bullet up, delete this bullut
					if (clock() - *(coldHoleBullets[i][j]->getStartTime()) >= 4000)
					{
						// delete bullet
						delete coldHoleBullets[i][j];
						coldHoleBullets[i].erase(coldHoleBullets[i].begin() + j);
					}
				}

				for (int j = 0; j < coldHoleBullets[i].size(); j++)
				{
					// if bullet overly in sprite to simulate
					if (sprite.getGlobalBounds().intersects(coldHoleBullets[i][j]->getSprite()->getGlobalBounds()))
					{
						gHero->setState(State::Frozen, 2000); // hero be frozen
						gHero->damage(1 + gHero->getMaxHP() / 20); // hero get damage

						// delete bullet
						delete coldHoleBullets[i][j];
						coldHoleBullets[i].erase(coldHoleBullets[i].begin() + j);
					}
				}
			}

			for (int i = 0; i < exitSprite.size(); i++)
			{
				// if exit door is open
				if (isExitOpen)
				{
					exitSprite[i].setTexture(exitTexture[1]); // set sprite is opening door texture
					conditionText[i].setFillColor(Color::Green); // set thr color of condition text is green 

					// if pressed right mouse on the exit, will return true
					if (gHero->getSprite().getGlobalBounds().intersects(exitSprite[i].getGlobalBounds()))
					{
						return Mouse::isButtonPressed(Mouse::Right);
					}
				}
				else exitSprite[i].setTexture(exitTexture[0]); // set sprite is no open door texture
			}

			for (int i = 0; i < puzzles.size(); i++)
			{
				puzzles[i]->playEvent(event); // puzzles event
			}
		}
		
		return false;
	}

	// Intent:	create a puzzle game
	// Pre:		None
	// Post:	None
	void createPuzzle()
	{
		// create and set a puzzle game
		puzzles.push_back(new Puzzle(mainWindow, gHero));
		puzzles.back()->setPosition(startPos.x + 850, startPos.y + 1300);
	}

	// Intent:	check the puzzle whether completed
	// Pre:		None
	// Post:    the puzzle whether completed
	bool isPuzzleComp()
	{
		if (puzzles.size() > 0)
		{
			return puzzles.back()->isGameOver();
		}

		return false;
	}

	// Intent:	set the text of condition
	// Pre:		text
	// Post:    None
	void setText(string text)
	{
		for (int i = 0; i < conditionText.size(); i++)
		{
			conditionText[i].setString(text);
		}
	}

	// Intent:	get style of room
	// Pre:		None
	// Post:    the number of room style
	int getRoomStyle()
	{
		return style;
	}

	// Intent:	draw all object to thewindow of game
	// Pre:		None
	// Post:    None
	void drawInWindow()
	{
		if (FloatRect(0, 0, mainWindow->getSize().x, mainWindow->getSize().y).intersects(backgroundSprite.getGlobalBounds()))
		{
			mainWindow->draw(backgroundSprite);
		}

		for (int i = 0; i < objectSprite.size(); i++)
		{
			// if wall is overly on screen of game, draw it to window
			if (FloatRect(0, 0, mainWindow->getSize().x, mainWindow->getSize().y).intersects(objectSprite[i].getGlobalBounds()))
			{
				mainWindow->draw(objectSprite[i]);
			}
		}

		for (int i = 0; i < lightningSprite.size(); i++)
		{
			// if lightning is overly on screen of game, draw it to window
			if (FloatRect(0, 0, mainWindow->getSize().x, mainWindow->getSize().y).intersects(lightningSprite[i].getGlobalBounds()))
			{
				mainWindow->draw(lightningSprite[i]);
			}
		}

		for (int i = 0; i < smallLaserSprite.size(); i++)
		{
			// if small laser is overly on screen of game, draw it to window
			if (FloatRect(0, 0, mainWindow->getSize().x, mainWindow->getSize().y).intersects(smallLaserSprite[i].getGlobalBounds()))
			{
				mainWindow->draw(smallLaserSprite[i]);
			}
		}

		for (int i = 0; i < ballSprite.size(); i++)
		{
			// if ball is overly on screen of game, draw it to window
			if (FloatRect(0, 0, mainWindow->getSize().x, mainWindow->getSize().y).intersects(ballSprite[i].getGlobalBounds()))
			{
				mainWindow->draw(ballSprite[i]);
			}
		}

		for (int i = 0; i < coldHoleSprite.size(); i++)
		{
			// if cold hole is overly on screen of game, draw it to window
			if (FloatRect(0, 0, mainWindow->getSize().x, mainWindow->getSize().y).intersects(coldHoleSprite[i].getGlobalBounds()))
			{
				mainWindow->draw(coldHoleSprite[i]);
			}

			for (int j = 0; j < coldHoleBullets[i].size(); j++)
			{
				// if buttle of cold hold is overly on screen of game, draw it to window
				if (FloatRect(0, 0, mainWindow->getSize().x, mainWindow->getSize().y).intersects(coldHoleBullets[i][j]->getSprite()->getGlobalBounds()))
				{
					mainWindow->draw(*coldHoleBullets[i][j]->getSprite());
				}
			}
		}

		for (int i = 0; i < exitSprite.size(); i++)
		{
			// if exit is overly on screen of game, draw it to window
			if (FloatRect(0, 0, mainWindow->getSize().x, mainWindow->getSize().y).intersects(exitSprite[i].getGlobalBounds()))
			{
				mainWindow->draw(exitSprite[i]);
			}
		}

		for (int i = 0; i < conditionText.size(); i++)
		{
			// if the text of condition is overly on screen of game, draw it to window
			if (FloatRect(0, 0, mainWindow->getSize().x, mainWindow->getSize().y).intersects(conditionText[i].getGlobalBounds()))
			{
				mainWindow->draw(conditionText[i]);
			}
		}

		for (int i = 0; i < puzzles.size(); i++)
		{
			puzzles[i]->drawInWindow();
		}
	}

protected:

	// Intent:	create style 0 of room
	// Pre:		None
	// Post:    None
	void room0Style()
	{ 
		// create and set walls
		objectTexture[8].loadFromFile("./image/Wall/allRangeWall.png");
		objectSprite[8].setTexture(objectTexture[8]);
		objectSprite[8].setPosition(startPos.x, startPos.y);
	}

	// Intent:	create style 1 of room
	// Pre:		None
	// Post:    None
	void room1Style()
	{

	}

	// Intent:	create style 2 of room
	// Pre:		None
	// Post:    None
	void room2Style()
	{
		// set font
		Font* font = new Font;
		font->loadFromFile("resources/sansation.ttf");
		
		// create and set the text of condition
		conditionText.assign(1, Text());		
		conditionText[0].setFont(*font);
		conditionText[0].setCharacterSize(50);
		conditionText[0].setFillColor(Color::Red);
		conditionText[0].setPosition(startPos.x + 950, startPos.y + 650);

		// create and set exit
		exitSprite.assign(1, Sprite());
		exitTexture.assign(2, Texture());
		exitTexture[0].loadFromFile("./image/exit_close.png");
		exitTexture[1].loadFromFile("./image/exit_open.png");
		exitSprite[0].setTexture(exitTexture[0]);
		exitSprite[0].setPosition(startPos.x + 750, startPos.y + 750);
	}

	// Intent:	create style 3 of room
	// Pre:		None
	// Post:    None
	void room3Style()
	{
		// create and set walls
		objectTexture[8].loadFromFile("./image/Wall/centerBlockWall.png");
		objectSprite[8].setTexture(objectTexture[8]);
		objectSprite[8].setPosition(startPos.x + 300, startPos.y + 300);

		objectTexture[9].loadFromFile("./image/Wall/centerBlockWall.png");	
		objectSprite[9].setTexture(objectTexture[9]);
		objectSprite[9].setPosition(startPos.x + 520, startPos.y + 520);

		objectTexture[10].loadFromFile("./image/Wall/centerBlockWall.png");
		objectSprite[10].setTexture(objectTexture[10]);
		objectSprite[10].setPosition(startPos.x + 1400, startPos.y + 400);

		objectTexture[11].loadFromFile("./image/Wall/centerBlockWall.png");
		objectSprite[11].setTexture(objectTexture[11]);
		objectSprite[11].setPosition(startPos.x + 500, startPos.y + 1400);

		objectTexture[12].loadFromFile("./image/Wall/centerBlockWall.png");
		objectSprite[12].setTexture(objectTexture[12]);
		objectSprite[12].setPosition(startPos.x + 1400, startPos.y + 1400);
		
		deltas.assign(2, 3.5); // create deltas 
		startTimes.assign(2, clock()); // create timers

		// create and set small lasers
		smallLaserSprite.assign(2, Sprite());
		smallLaserTexture.assign(2, Texture());

		smallLaserTexture[0].loadFromFile("./image/Trap/smallLaser.png");
		smallLaserSprite[0].setTexture(smallLaserTexture[0]);
		smallLaserSprite[0].setPosition(100, 500);

		smallLaserTexture[1].loadFromFile("./image/Trap/smallLaser.png");
		smallLaserSprite[1].setTexture(smallLaserTexture[1]);
		smallLaserSprite[1].setPosition(1650, 1500);

		// create positions of creature can be generated 
		creaturePos.push_back(Position(startPos.x + 400, startPos.y + 100));
		creaturePos.push_back(Position(startPos.x + 800, startPos.y + 800));
		creaturePos.push_back(Position(startPos.x + 950, startPos.y + 1750));
	}

	// Intent:	create style 4 of room
	// Pre:		None
	// Post:    None
	void room4Style()
	{
		// create and set walls
		objectTexture[8].loadFromFile("./image/Wall/centerColWall.png");
		objectSprite[8].setTexture(objectTexture[8]);
		objectSprite[8].setPosition(startPos.x + 400, startPos.y + 400);

		objectTexture[9].loadFromFile("./image/Wall/centerRowWall.png");
		objectSprite[9].setTexture(objectTexture[9]);
		objectSprite[9].setPosition(startPos.x + 1100, startPos.y + 400);

		objectTexture[10].loadFromFile("./image/Wall/centerColWall.png");
		objectSprite[10].setTexture(objectTexture[10]);
		objectSprite[10].setPosition(startPos.x + 1420, startPos.y + 400);

		objectTexture[11].loadFromFile("./image/Wall/centerBlockWall.png");
		objectSprite[11].setTexture(objectTexture[11]);
		objectSprite[11].setPosition(startPos.x + 400, startPos.y + 1400);

		objectTexture[12].loadFromFile("./image/Wall/centerRowWall.png");
		objectSprite[12].setTexture(objectTexture[12]);
		objectSprite[12].setPosition(startPos.x + 1100, startPos.y + 1400);

		startTimes.assign(5, clock()); // create timers

		// create and set lightnings
		lightningSprite.assign(5, Sprite());
		lightningTexture.assign(15, Texture());

		for (int i = 0; i < 15; i += 3)
		{
			lightningTexture[i].loadFromFile("./image/Trap/lightningState1.png");
			lightningTexture[i + 1].loadFromFile("./image/Trap/lightningState2.png");
			lightningTexture[i + 2].loadFromFile("./image/Trap/lightningState3.png");
			lightningSprite[i / 5].setTexture(lightningTexture[i]);
		}

		lightningSprite[0].setPosition(50, 50);
		lightningSprite[1].setPosition(1450, 50);
		lightningSprite[2].setPosition(50, 1450);
		lightningSprite[3].setPosition(1450, 1450);
		lightningSprite[4].setPosition(850, 850);

		// create positions of creature can be generated 
		creaturePos.push_back(Position(startPos.x + 300, startPos.y + 150));
		creaturePos.push_back(Position(startPos.x + 1200, startPos.y + 150));
		creaturePos.push_back(Position(startPos.x + 800, startPos.y + 800));
		creaturePos.push_back(Position(startPos.x + 300, startPos.y + 1750));
	}

	// Intent:	create style 5 of room
	// Pre:		None
	// Post:    None
	void room5Style()
	{
		// create and set walls
		objectTexture[8].loadFromFile("./image/Wall/centerColWall.png");
		objectSprite[8].setTexture(objectTexture[8]);
		objectSprite[8].setPosition(startPos.x + 400, startPos.y + 400);

		objectTexture[9].loadFromFile("./image/Wall/centerRowWall.png");
		objectSprite[9].setTexture(objectTexture[9]);
		objectSprite[9].setPosition(startPos.x + 400, startPos.y + 400);

		objectTexture[10].loadFromFile("./image/Wall/centerBlockWall.png");
		objectSprite[10].setTexture(objectTexture[10]);
		objectSprite[10].setPosition(startPos.x + 1400, startPos.y + 1400);

		startTimes.push_back(clock()); // create timers

		// create and set cold holes
		coldHoleTexture.push_back(Texture());
		coldHoleSprite.push_back(Sprite());
		coldHoleBullets.push_back(vector<ColdHoleBullet*>());
		coldHoleTexture[0].loadFromFile("./image/Trap/coldHole.png");
		coldHoleSprite[0].setTexture(coldHoleTexture[0]);
		coldHoleSprite[0].setPosition(startPos.x + ROOM_SIZE / 2 - coldHoleTexture[0].getSize().x / 2, startPos.y + ROOM_SIZE / 2 - coldHoleTexture[0].getSize().y / 2);

		// create positions of creature can be generated 
		creaturePos.push_back(Position(startPos.x + 300, startPos.y + 1500));
		creaturePos.push_back(Position(startPos.x + 1200, startPos.y + 150));
		creaturePos.push_back(Position(startPos.x + 800, startPos.y + 800));
		creaturePos.push_back(Position(startPos.x + 1700, startPos.y + 1700));
	}

	// Intent:	create style 6 of room
	// Pre:		None
	// Post:    None
	void room6Style()
	{
		// create and set walls
		objectTexture[8].loadFromFile("./image/Wall/centerMidWall.png");
		objectSprite[8].setTexture(objectTexture[8]);
		objectSprite[8].setPosition(startPos.x, startPos.y);

		objectTexture[9].loadFromFile("./image/Wall/centerMidWall.png");
		objectSprite[9].setTexture(objectTexture[9]);
		objectSprite[9].setPosition(startPos.x + ROOM_SIZE - 660, startPos.y);

		objectTexture[10].loadFromFile("./image/Wall/centerMidWall.png");
		objectSprite[10].setTexture(objectTexture[10]);
		objectSprite[10].setPosition(startPos.x, startPos.y + ROOM_SIZE - 660);

		objectTexture[11].loadFromFile("./image/Wall/centerMidWall.png");
		objectSprite[11].setTexture(objectTexture[11]);
		objectSprite[11].setPosition(startPos.x + ROOM_SIZE - 660, startPos.y + ROOM_SIZE - 660);

		// create positions of creature can be generated 
		creaturePos.push_back(Position(startPos.x + 1000, startPos.y + 200));
		creaturePos.push_back(Position(startPos.x + 100, startPos.y + 700));
		creaturePos.push_back(Position(startPos.x + 1500, startPos.y + 800));
		creaturePos.push_back(Position(startPos.x + 950, startPos.y + 950));
		creaturePos.push_back(Position(startPos.x + 1200, startPos.y + 1600));
	}

	// Intent:	create style 7 of room
	// Pre:		None
	// Post:    None
	void room7Style()
	{
		// create and set walls
		objectTexture[8].loadFromFile("./image/Wall/centerBlockWall.png");
		objectSprite[8].setTexture(objectTexture[8]);
		objectSprite[8].setPosition(startPos.x + 100, startPos.y + 100);

		objectTexture[9].loadFromFile("./image/Wall/centerBlockWall.png");
		objectSprite[9].setTexture(objectTexture[9]);
		objectSprite[9].setPosition(startPos.x + 1700, startPos.y + 100);

		objectTexture[10].loadFromFile("./image/Wall/centerBlockWall.png");
		objectSprite[10].setTexture(objectTexture[10]);
		objectSprite[10].setPosition(startPos.x + 100, startPos.y + 1700);

		objectTexture[11].loadFromFile("./image/Wall/centerBlockWall.png");
		objectSprite[11].setTexture(objectTexture[11]);
		objectSprite[11].setPosition(startPos.x + 1700, startPos.y + 1700);

		objectTexture[12].loadFromFile("./image/Wall/centerBlockWall.png");
		objectSprite[12].setTexture(objectTexture[12]);
		objectSprite[12].setPosition(startPos.x + 900, startPos.y + 900);

		objectTexture[13].loadFromFile("./image/Wall/centerBlockWall.png");
		objectSprite[13].setTexture(objectTexture[13]);
		objectSprite[13].setPosition(startPos.x + 1100, startPos.y + 1100);

		startTimes.assign(2, clock());  // create timers
		deltas.push_back(0.3);  // create deltas 

		// create and set balls
		ballTexture.assign(2, Texture());
		ballSprite.assign(2, Sprite());
		ballTexture[0].loadFromFile("./image/Trap/ball.png");
		ballSprite[0].setTexture(ballTexture[0]);
		ballSprite[0].setOrigin(ballTexture[0].getSize().x / 2, ballTexture[0].getSize().y / 2);
		ballSprite[0].setPosition(startPos.x + 200, startPos.y + ROOM_SIZE / 2 - 25);
		ballTexture[1].loadFromFile("./image/Trap/ball.png");
		ballSprite[1].setTexture(ballTexture[1]);
		ballSprite[1].setOrigin(ballTexture[1].getSize().x / 2, ballTexture[1].getSize().y / 2);
		ballSprite[1].setPosition(startPos.x + ROOM_SIZE / 2 - 200, startPos.y + ROOM_SIZE / 2 - 25);

		// create positions of creature can be generated 
		creaturePos.push_back(Position(startPos.x + 450, startPos.y + 450));
		creaturePos.push_back(Position(startPos.x + 1500, startPos.y + 450));
		creaturePos.push_back(Position(startPos.x + 500, startPos.y + 1450));
		creaturePos.push_back(Position(startPos.x + 950, startPos.y + 1150));
	}

	// Intent:	create style 8 of room
	// Pre:		None
	// Post:    None
	void room8Style()
	{
		// create and set walls
		objectTexture[8].loadFromFile("./image/Wall/centerBlockWall.png");
		objectSprite[8].setTexture(objectTexture[8]);
		objectSprite[8].setPosition(startPos.x + 500, startPos.y + 500);

		objectTexture[9].loadFromFile("./image/Wall/centerRowWall.png");
		objectSprite[9].setTexture(objectTexture[9]);
		objectSprite[9].setPosition(startPos.x + 500, startPos.y + 1300);

		objectTexture[10].loadFromFile("./image/Wall/centerColWall.png");
		objectSprite[10].setTexture(objectTexture[10]);
		objectSprite[10].setPosition(startPos.x + 1300, startPos.y + 500);

		objectTexture[11].loadFromFile("./image/Wall/centerRowWall.png");
		objectSprite[11].setTexture(objectTexture[11]);
		objectSprite[11].setPosition(startPos.x + 950, startPos.y + 1300);

		objectTexture[12].loadFromFile("./image/Wall/centerColWall.png");
		objectSprite[12].setTexture(objectTexture[12]);
		objectSprite[12].setPosition(startPos.x + 1300, startPos.y + 950);

		// create positions of creature can be generated 
		creaturePos.push_back(Position(startPos.x + 1000, startPos.y + 450));
		creaturePos.push_back(Position(startPos.x + 550, startPos.y + 1000));
		creaturePos.push_back(Position(startPos.x + 1700, startPos.y + 1700));
	}

	// Intent:	create style 9 of room
	// Pre:		None
	// Post:    None
	void room9Style()
	{
		// create and set walls
		objectTexture[8].loadFromFile("./image/Wall/centerColWall.png");
		objectSprite[8].setTexture(objectTexture[8]);
		objectSprite[8].setPosition(startPos.x + 500, startPos.y + 500);

		objectTexture[9].loadFromFile("./image/Wall/centerColWall.png");
		objectSprite[9].setTexture(objectTexture[9]);
		objectSprite[9].setPosition(startPos.x + 500, startPos.y + 1100);

		objectTexture[10].loadFromFile("./image/Wall/centerColWall.png");
		objectSprite[10].setTexture(objectTexture[10]);
		objectSprite[10].setPosition(startPos.x + 1300, startPos.y + 500);

		objectTexture[11].loadFromFile("./image/Wall/centerColWall.png");
		objectSprite[11].setTexture(objectTexture[11]);
		objectSprite[11].setPosition(startPos.x + 1300, startPos.y + 1100);

		// create positions of creature can be generated 
		creaturePos.push_back(Position(startPos.x + 150, startPos.y + 1000));
		creaturePos.push_back(Position(startPos.x + 1000, startPos.y + 1300));
		creaturePos.push_back(Position(startPos.x + 1600, startPos.y + 650));
	}

	// Intent:	create style 10 of room
	// Pre:		None
	// Post:    None
	void room10Style()
	{
		// create and set walls
		objectTexture[8].loadFromFile("./image/Wall/centerBigWall.png");
		objectSprite[8].setTexture(objectTexture[8]);
		objectSprite[8].setPosition(startPos.x + 450, startPos.y + 450);

		// create positions of creature can be generated 
		creaturePos.push_back(Position(startPos.x + 120, startPos.y + 120));
		creaturePos.push_back(Position(startPos.x + 120, startPos.y + 1300));
		creaturePos.push_back(Position(startPos.x + 1650, startPos.y + 120));
	}

	// Intent:	create style 11 of room
	// Pre:		None
	// Post:    None
	void room11Style()
	{
		// create positions of creature can be generated 
		creaturePos.push_back(Position(startPos.x + 1000, startPos.y + 1000));
	}
};
