/***********************************************************************
 * File: Turtle.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for Turtle main program header
***********************************************************************/
#pragma once
#include "Creature.h"
#include "Hero.h"

class Turtle : public Creature
{
private:
	Texture* cAttackTexture; // texture of turtle attack
	clock_t attackStartTime = clock(); // time of hermit crab attack
	clock_t attackWaitTime; // waiting time before attacking
	Hero* gHero; // hero
	bool isAttacking = false; // turtle whether attacking 
	float speedX = 2; // delta x of turtle attack 
	float speedY = 2; // delta y of turtle attack 

public:
	// Intent:	setting constructor
	// Pre:		the window of game, obstacles of game, level of turtle
	// Post:	None
	Turtle(RenderWindow* window, Hero* hero, vector<Sprite*>* _collisionSprites, int level = 1)
	{
		// set datas and image of turtle
		mainWindow = window;
		collisionSprites = _collisionSprites;
		cID = CreatureID::Turtle;
		face = "front";
		cTexture = new Texture;
		cSprite = new Sprite;
		cAttackTexture = new Texture;
		gHero = hero;
		cTexture->loadFromFile("./image/Creature/Turtle/turtle_" + face + ".png");
		cAttackTexture->loadFromFile("./image/Creature/Turtle/Turtle_Attack.png");
		cSprite->setTexture(*cTexture);

		// set textures of turtle
		for (int i = 0; i < 4; i++) cTextures.push_back(new Texture());

		cTextures[0]->loadFromFile("./image/Creature/Turtle/turtle_front.png");
		cTextures[1]->loadFromFile("./image/Creature/Turtle/turtle_right.png");
		cTextures[2]->loadFromFile("./image/Creature/Turtle/turtle_back.png");
		cTextures[3]->loadFromFile("./image/Creature/Turtle/turtle_left.png");

		// set values of turtle
		state = 0;
		currentHp = maxHp = level * 15;
		atk = level * 3 + level - 1;
		coin = 5 + (level - 1) * 1.5;
		exp = 5 + (level - 1) * 2;
	}

	// Intent:	update turtle
	// Pre:		position of hero, correction speed
	// Post:	None
	void update(Position heroPos, float correctionSpeed) override
	{
		if (!isAttacking) 
		{
			// let the turtle face the position of the hero
			if (abs(getPosition().x - heroPos.x) < abs(getPosition().y - heroPos.y))
			{
				if (getPosition().y < heroPos.y)
				{
					face = "front";
					cSprite->setTexture(*cTextures[0]);
				}
				else if (getPosition().y > heroPos.y)
				{
					face = "back";
					cSprite->setTexture(*cTextures[2]);
				}
			}
			else
			{
				if (getPosition().x < heroPos.x)
				{
					face = "right";
					cSprite->setTexture(*cTextures[1]);
				}
				else if (getPosition().x > heroPos.x)
				{
					face = "left";
					cSprite->setTexture(*cTextures[3]);
				}
			}

			// set sprite of turtle to sprite to simulate, increase size
			Sprite turtleVision = *cSprite;
			turtleVision.setPosition(turtleVision.getPosition().x - 150, turtleVision.getPosition().y - 150);
			turtleVision.setScale(3, 3);

			// if hero overly on prite to simulate
			if (gHero->getSprite().getGlobalBounds().intersects(turtleVision.getGlobalBounds()))
			{
				isAttacking = true; // set turtle is attacking
				attackStartTime = clock();
				attackWaitTime = clock();
				cSprite->setTexture(*cAttackTexture);

				// calculate delta of turtle attack 
				float deltaX = gHero->getPosition().x - getPosition().x;
				float deltaY = gHero->getPosition().y - getPosition().y;
				float step = deltaX > deltaY ? abs(deltaX) : abs(deltaY);
				speedX = 8 * (deltaX / step) + correctionSpeed;
				speedY = 8 * (deltaY / step) + correctionSpeed;
			}
		}
		else
		{
			// if wait time up
			if (clock() - attackWaitTime >= 1000)
			{
				move(speedX, speedY); // turtle move

				for (int i = 0; i < collisionSprites->size(); i++)
				{
					// if sprite to simulate will be overly on obstacles
					if (cSprite->getGlobalBounds().intersects((*collisionSprites)[i]->getGlobalBounds()))
					{
						// check the side where the turtle hits the obstacle
						CollisionSide side = getCollisionSide(cSprite->getGlobalBounds(), (*collisionSprites)[i]->getGlobalBounds());

						// if turtle be collided, change delta x or delta y 
						switch (side)
						{
						case CollisionSide::Right:
						case CollisionSide::Left:
							speedX = -speedX;
							break;
						case CollisionSide::Top:
						case CollisionSide::Bottom:
							speedY = -speedY;
							break;
						}
					}
				}

				// if time up, end attacking
				if (clock() - attackStartTime >= 10000) isAttacking = false; 
			}			
		}			
	}

	// Intent:  turtle move 
	// Pre:		delta x, delta y
	// Post:	None
	void move(float deltaX, float deltaY) override
	{
		// set sprite of hero to sprite to simulate, decrease size and move
		Sprite sprite = *cSprite;
		sprite.setPosition(sprite.getPosition().x + 10, sprite.getPosition().y + 10);
		sprite.setScale(0.8, 0.8);
		sprite.move(deltaX, deltaY);

		bool canMove = true;

		for (int i = 0; i < collisionSprites->size(); i++)
		{
			// if turtle will be overly on obstacles 
			if ((*collisionSprites)[i]->getGlobalBounds().intersects(sprite.getGlobalBounds()))
			{
				canMove = false;
				break;
			}
		}

		if (canMove) cSprite->move(deltaX, deltaY);
	}

	// Intent:  move turtle
	// Pre:		delta x, delta y
	// Post:	None
	void followMove(float deltaX, float deltaY) override
	{
		cSprite->move(deltaX, deltaY);
	}

	// Intent:  draw turtle to the window of game 
	// Pre:		None
	// Post:	None
	void drawInWindow() override
	{
		mainWindow->draw(*cSprite);
	}

	// Intent:  set texture of turtle
	// Pre:		face of turtle, state of turtle
	// Post:	None
	void setTexture(string face, string state = "")
	{
		cTexture->loadFromFile("./image/Creature/Turtle/turtle_" + face + ".png");
		this->face = face; // set face
	}

	// Intent:  get the center position of turtle
	// Pre:		None
	// Post:	the center position of turtle
	Position getPosition()
	{
		return Position(cSprite->getPosition().x + cTexture->getSize().x / 2, cSprite->getPosition().y + cTexture->getSize().y / 2);
	}

protected:
	// collision side
	enum class CollisionSide
	{
		None,
		Left,
		Right,
		Top,
		Bottom
	};

	// Intent:  get collision side of turtle
	// Pre:		bounds of turtle, bounds of obstacle
	// Post:	collision side
	CollisionSide getCollisionSide(const FloatRect& turtleBounds, const FloatRect& obstacleBounds)
	{
		// get turtle and obstacle collision side
		if (turtleBounds.left + turtleBounds.width > obstacleBounds.left)
			return CollisionSide::Left;

		if (turtleBounds.left < obstacleBounds.left + obstacleBounds.width)
			return CollisionSide::Right;

		if (turtleBounds.top + turtleBounds.height > obstacleBounds.top)
			return CollisionSide::Top;

		if (turtleBounds.top < obstacleBounds.top + obstacleBounds.height)
			return CollisionSide::Bottom;

		return CollisionSide::None;
	}
};


