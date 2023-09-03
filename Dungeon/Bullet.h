/***********************************************************************
 * File: Bullet.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for Bullet main program header
***********************************************************************/
#pragma once
#include <SFML/Graphics.hpp>
#include <vector> 
#include <cmath>
#include "Position.h"

using namespace sf;
using namespace std;

class Bullet
{
private:
	RenderWindow* mainWindow;  // the window of game
	int atk; // atk of bullet
	int speed; // speed of bullet
	Position startPos; // start position of bullet
	string face; // face of object that shoots bullet
	string bulletFace; // direction of bullet 
	int distance; // the max distance of bullet
	Sprite bulletSprite; // sprite of bullet
	Texture bulletTexture; // texture of bullet
	vector<Sprite*>* collisionSprites;  // obstacles of game
	clock_t startTime; // time the bullet was fired
	string  bulletType; // type of bullet
	Position targetPos = Position{-1, -1}; // target position of bullet
	bool isMoveToTarget; // whether the bullet should move to the target position
	
public:
	// Intent:	setting constructor, the bullet shouldn't move to the target position
	// Pre:		the window of game, obstacles of game, position of object that shoots bullet, size of object that shoots bullet,
	//          face of object that shoots bullet, direction of bullet, type of bullet, max distance of bullet, speed of bullet
	// Post:	None
	Bullet(RenderWindow* window, vector<Sprite*>* _collisionSprites, Position sPos, Size sSize, string _face, string _bulletFace, string _bulletType, int _distance, int bulletSpeed = 1)
	{
		// set values of bullet, and image
		mainWindow = window;
		face = _face;
		distance = _distance;
		bulletFace = _bulletFace;
		collisionSprites  = _collisionSprites;
		bulletType = _bulletType;
		bulletTexture.loadFromFile("./image/Bullet/" +  bulletType + "_" + bulletFace + "_bullet.png");
		bulletSprite.setTexture(bulletTexture);
		speed = bulletSpeed;
		startTime = clock();
		isMoveToTarget = false;

		// set the start position of bullet
		if (face == "front")
		{
			startPos.x = sPos.x + (sSize.width - bulletTexture.getSize().x) / 2;
			startPos.y = sPos.y + sSize.height - bulletTexture.getSize().y;
			bulletSprite.setPosition(startPos.x, startPos.y);
		}
		else if (face == "back")
		{		
			startPos.x = sPos.x + (sSize.width - bulletTexture.getSize().x) / 2;
			startPos.y = sPos.y;
			bulletSprite.setPosition(startPos.x, startPos.y);
		}
		else if (face == "right")
		{
			startPos.x = sPos.x + sSize.width - bulletTexture.getSize().x;
			startPos.y = sPos.y + (sSize.height - bulletTexture.getSize().y) / 2;
			bulletSprite.setPosition(startPos.x, startPos.y);
		}
		else if(face == "left")
		{
			startPos.x = sPos.x;
			startPos.y = sPos.y + (sSize.height - bulletTexture.getSize().y) / 2;
			bulletSprite.setPosition(startPos.x, startPos.y);
		}
	}

	// Intent:	setting constructor, the bullet should move to the target position
	// Pre:		the window of game, obstacles of game, position of object that shoots bullet, size of object that shoots bullet, tartget position 
	//          face of object that shoots bullet, direction of bullet, type of bullet, max distance of bullet, speed of bullet
	// Post:	None
	Bullet(RenderWindow* window, vector<Sprite*>* _collisionSprites, Position sPos, Size sSize, Position _targetPos, string _face, string _bulletFace, string _bulletType, int _distance, int bulletSpeed = 1)
		: Bullet(window, _collisionSprites, sPos, sSize, _face, _bulletFace, _bulletType, _distance, bulletSpeed)
	{
		//set values of bullet
		targetPos = _targetPos;
		isMoveToTarget = true;
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
		// the bullet shouldn't move to the target position
		if (!isMoveToTarget)
		{
			// call tryMove() to check bullet can move, than move bullet
			if (bulletFace == "front")
			{
				if (tryMove(0, speed)) bulletSprite.move(0, speed);
			}
			else if (bulletFace == "front-left")
			{
				if (tryMove(-speed, speed)) bulletSprite.move(-speed, speed);
			}
			else if (bulletFace == "left")
			{
				if (tryMove(-speed, 0)) bulletSprite.move(-speed, 0);
			}
			else if (bulletFace == "back-left")
			{
				if (tryMove(-speed, -speed)) bulletSprite.move(-speed, -speed);
			}
			else if (bulletFace == "back")
			{
				if (tryMove(0, -speed)) bulletSprite.move(0, -speed);
			}
			else if (bulletFace == "back-right")
			{
				if (tryMove(speed, -speed)) bulletSprite.move(speed, -speed);
			}
			else if (bulletFace == "right")
			{
				if (tryMove(speed, 0)) bulletSprite.move(speed, 0);
			}
			else if (bulletFace == "front-right")
			{
				if (tryMove(speed, speed)) bulletSprite.move(speed, speed);
			}
		}
		else // the bullet should move to the target position
		{
			int dx = targetPos.x - bulletSprite.getPosition().x; // The x gap between target position and bullet position
			int dy = targetPos.y - bulletSprite.getPosition().y; // The y gap between target position and bullet position
			int randFace = -1; // Used to save bullet position to move x or y

			// If the difference in x is the same as the difference in y, choose one at random
			if (abs(dx) == abs(dy)) randFace = rand() % 2; 

			if (abs(dx) > abs(dy) || randFace == 0) // If the difference in x is relatively large, move the x-axis
			{
				if (targetPos.x < bulletSprite.getPosition().x)
				{
					// If target position is on the left bullet position moves to the left
					if (tryMove(-speed, 0)) this->move(-speed, 0); 
				}
				else
				{
					// If target position is on the right bullet position moves to the right
					if (tryMove(speed, 0)) this->move(speed, 0); 
				}
			}
			else if (abs(dx) < abs(dy) || randFace == 1)  // If the gap in y is relatively large, move the y axis
			{
				if (targetPos.y < bulletSprite.getPosition().y)
				{
					// If target position is above bullet position move up
					if (tryMove(0, -speed)) this->move(0, -speed); 
				}
				else
				{
					// If target position is below bullet position, move down
					if (tryMove(0, speed)) this->move(0, speed); 
				}

			}
		}
	}

	// Intent:	try move bullet
	// Pre:		delta x, delta y
	// Post:	None
	bool tryMove(int deltaX, int deltaY)
	{
		// set sprite of bullet to sprite to simulate
		Sprite sprite = bulletSprite;
		sprite.move(deltaX, deltaY);
		bool canMove = true;

		// check if bullet is overly on obstacles
		for (int i = 0; i < collisionSprites->size(); i++)
		{
			if ((*collisionSprites)[i]->getGlobalBounds().intersects(sprite.getGlobalBounds())) return false;		
		}

		return true;
	}

	// Intent:	move bullet, if have target position, move target position too
	// Pre:		delta x, delta y
	// Post:	None
	void followMove(float deltaX, float deltaY)
	{
		bulletSprite.move(deltaX, deltaY); // move bullet

		// if have target position
		if (isMoveToTarget)
		{
			// move target position
			targetPos.x += deltaX;
			targetPos.y += deltaY;
		}		
	}

	// Intent:	get sprite of bullet 
	// Pre:		None
	// Post:	sprite of bullet 
	Sprite* getSprite()
	{
		return &bulletSprite;
	}

	// Intent:	get texture of bullet 
	// Pre:		None
	// Post:	texture of bullet 
	Texture* getTexture()
	{
		return &bulletTexture;
	}

	// Intent:	get start position of bullet
	// Pre:		None
	// Post:	start position of bullet
	Position* getStartPos()
	{
		return &startPos;
	}

	// Intent:	get the max distance of bullet
	// Pre:		None
	// Post:	the max distance of bullet
	int getDistance()
	{
		return distance;
	}

	// Intent:  bullet move 
	// Pre:		delta x, delta y 
	// Post:	None
	void move(int x, int y)
	{
		// move bullet
		bulletSprite.move(x, y);
	}

	// Intent:  get start time of bullet
	// Pre:		None
	// Post:	start time of bullet
	clock_t* getStartTime()
	{
		return &startTime;
	}

	// Intent:  get type of bullet
	// Pre:		None
	// Post:	type of bullet
	string getBulletType()
	{
		return  bulletType;
	}
};
