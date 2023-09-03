/***********************************************************************
 * File:  Creature.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for Creature main program header
***********************************************************************/
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> 
#include "Position.h"

using namespace sf;
using namespace std;

class Creature
{
protected:
	RenderWindow* mainWindow; // the window of game
	Sprite* cSprite; // sprtie of creature 
	Texture* cTexture; // texture of creature
	vector<Texture*>  cTextures; // textures of creature 
	vector<Texture *> cWalkTexture;  // textures of creature walking
	string face; // face of creature
	int cID = -1; // id of creature
	int state = 0; // state of texture
	int level = 1; // level of creature
	float speed = 1; // speed of creature
	int currentHp; // current hp of creature
	int maxHp; // max hp of creature
	int atk; // atk of creature
	int coin; // coin of creature
	int exp; // exp of creature
	int attackInterval = 1000; // attack interval
	clock_t attackStartTime = clock(); // timer of creature attack
	vector<Sprite*>* collisionSprites; // obstacles of game

public:
	
	// Intent:	draw creature on the window of game
	// Pre:		None
	// Post:	None
	virtual void drawInWindow()
	{
		mainWindow->draw(*cSprite);
	}

	// Intent:	set position of creature
	// Pre:		x of position, y of position
	// Post:	None
	void setPosition(int x, int y)
	{
		cSprite->setPosition(x, y);
	}

	// Intent:	get position of creature
	// Pre:		None
	// Post:	position of creature
	Position getPosition()
	{
		return Position(cSprite->getPosition().x + cTexture->getSize().x / 2, cSprite->getPosition().y + cTexture->getSize().y / 2);
	}

	// Intent:	get sprite of creature
	// Pre:		None
	// Post:	sprite of creature
	Sprite* getSprite()
	{
		return cSprite;
	}

	// Intent:	creature move 
	// Pre:		delta x, delta y
	// Post:	None
	virtual void move(float deltaX, float deltaY)
	{
		// set sprite of creature to sprite to simulate, move
		Sprite sprite = *cSprite;
		sprite.move(deltaX, deltaY);

		bool canMove = true;

		for (int i = 0; i < collisionSprites->size(); i++)
		{
			// if creature will be overly on obstacles 
			if ((*collisionSprites)[i]->getGlobalBounds().intersects(sprite.getGlobalBounds()))
			{
				canMove = false;
				break;
			}
		}

		if (canMove) cSprite->move(deltaX, deltaY);
	}

	// Intent:	move creature
	// Pre:		delta x, delta y
	// Post:	None
	virtual void followMove(float deltaX, float deltaY)
	{
		// move creature
		cSprite->move(deltaX, deltaY);
	}

	// Intent:	the creature walk in the direction of the hero
	// Pre:		position of hero, correction speed
	// Post:	None
	void trackHero(Position heroPos, float correctionSpeed)
	{
		int dx = heroPos.x - getPosition().x; // The x gap between Hero and Creature
		int dy = heroPos.y - getPosition().y; // The y gap between Hero and Creature
		int randFace = -1; // Used to save Creature to move x or y

		if (abs(dx) == abs(dy)) randFace = rand() % 2; // If the difference in x is the same as the difference in y, choose one at random

		if (abs(dx) > abs(dy) || randFace == 0) // If the difference in x is relatively large, move the x-axis
		{
			if (heroPos.x < getPosition().x) this->move(-(speed + correctionSpeed), 0); // If Hero is on the left Creature moves to the left
			else this->move((speed + correctionSpeed), 0); // If Hero is on the right Creature moves to the right
		}
		else if (abs(dx) < abs(dy) || randFace == 1)  // If the gap in y is relatively large, move the y axis
		{
			if (heroPos.y < getPosition().y) this->move(0, -(speed + correctionSpeed)); // If Hero is above Creature move up
			else this->move(0, (speed + correctionSpeed)); // If Hero is below Creature, move down
		}
	}

	// Intent:	get id of creature
	// Pre:		None
	// Post:	id of creature
	int getCreatureID()
	{
		return cID;
	}

	// Intent:	get current hp of creature
	// Pre:		None
	// Post:	current hp of creature
	int getCurrentHp()
	{
		return currentHp;
	}

	// Intent:	get max hp of creature
	// Pre:		None
	// Post:	max hp of creature
	int getMaxHp()
	{
		return maxHp;
	}

	// Intent:	get atk of creature
	// Pre:		None
	// Post:	atk of creature
	int getATK()
	{
		return atk;
	}

	// Intent:	creature get damage
	// Pre:		points
	// Post:	None
	void damage(int points)
	{
		currentHp -= points;
	}

	// Intent:	get coin of creature
	// Pre:		None
	// Post:	coin of creature
	int getCoin()
	{
		return coin;
	}

	// Intent:	get exp of creature
	// Pre:		None
	// Post:	exp of creature
	int getEXP()
	{
		return exp;
	}

	// Intent:	get face of creature
	// Pre:		None
	// Post:	face of creature
	virtual string getFace()
	{
		return face;
	}

	// Intent:	check creature whether attackable
 	// Pre:		current time
	// Post:	creature whether attackable
	bool attackable(clock_t attackEndTime)
	{
		// check time
		if (attackEndTime - attackStartTime >= attackInterval)
		{
			attackStartTime = clock();
			return true;
		}

		return false;
	}

	// Intent:	update creature
	// Pre:		position of hero, correction speed
	// Post:	None
	virtual void update(Position heroPos, float correctionSpeed) { };
};
