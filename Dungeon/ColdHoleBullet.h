/***********************************************************************
 * File: ColdHoleBullet.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for bullet of cold hole main program header
***********************************************************************/
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> 
#include "Hero.h"
#include "Position.h"
#include "Size.h"

using namespace sf;
using namespace std;

class ColdHoleBullet
{
private:
	RenderWindow* mainWindow; // the window of game
	int speed; // speed of bullet
	Position startPos; // start position of bullet
	Sprite bulletSprite; // sprtie of bullet
	Texture bulletTexture; // texture of bullet
	Hero* gHero; // hero
	clock_t startTime; // time the bullet was fired

public:
	// Intent:	setting constructor, the bullet shouldn't move to the target position
	// Pre:		the window of game, hero, position of cold hole, size of cold hole, speed of bullet
	// Post:	None
	ColdHoleBullet(RenderWindow* window,  Hero* hero, Position holePos, Size holeSize, int bulletSpeed = 1)
	{
		// set datas and image of bullet
		mainWindow = window;
		gHero = hero;
		bulletTexture.loadFromFile("./image/Trap/coldHole_bullet.png");
		bulletSprite.setTexture(bulletTexture);
		bulletSprite.setPosition(holePos.x + holeSize.width / 2, holePos.y + holeSize.height / 2);
		speed = bulletSpeed;
		startTime = clock();
	}

	// Intent:	draw this bullet to the window of game
	// Pre:		None
	// Post:	None
	void drawInWindow()
	{
		mainWindow->draw(bulletSprite);
	}

	// Intent:	calculate the next position of the bullet
	// Pre:		None
	// Post:	None
	void moveBullet()
	{
		int dx = gHero->getPosition().x - bulletSprite.getPosition().x; // The x gap between Hero and bullet position
		int dy = gHero->getPosition().y - bulletSprite.getPosition().y; // The y gap between Hero and bullet position
		int randFace = -1; // Used to save bullet position to move x or y

		if (abs(dx) == abs(dy)) randFace = rand() % 2; // If the difference in x is the same as the difference in y, choose one at random

		if (abs(dx) > abs(dy) || randFace == 0) // If the difference in x is relatively large, move the x-axis
		{
			if (gHero->getPosition().x < bulletSprite.getPosition().x) this->move(-speed, 0); // If Hero is on the left bullet position moves to the left
			else this->move(speed, 0); // If Hero is on the right bullet position moves to the right
		}
		else if (abs(dx) < abs(dy) || randFace == 1)  // If the gap in y is relatively large, move the y axis
		{
			if (gHero->getPosition().y < bulletSprite.getPosition().y) this->move(0, -speed); // If Hero is above bullet position move up
			else this->move(0, speed); // If Hero is below bullet position, move down
		}
	}

	// Intent:  bullet move 
	// Pre:		delta x, delta y 
	// Post:	None
	void move(int deltaX, int deltaY)
	{
		bulletSprite.move(deltaX, deltaY);
	}

	// Intent:  get sprite of bullet
	// Pre:		None
	// Post:	sprite of bullet
	Sprite* getSprite()
	{
		return &bulletSprite;
	}

	// Intent:  get start position of bullet
	// Pre:		None
	// Post:	start position of bullet
	Position* getStartPos()
	{
		return &startPos;
	}

	// Intent:  get start time of bullet
	// Pre:		None
	// Post:	start time of bullet
	clock_t* getStartTime()
	{
		return &startTime;
	}
};