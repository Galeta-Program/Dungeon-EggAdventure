/***********************************************************************
 * File: HermitCrab.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for Hermit Crab main program header
***********************************************************************/
#pragma once
#include "Creature.h"
#include "Hero.h"
#include "Bullet.h"

class HermitCrab : public Creature
{
private:	
	clock_t walkStartTime = clock(); // time of hermit crab walk
	vector<Bullet*> bullets; // bullets of hermit crab
	Hero* gHero; // hero

public:
	// Intent:	setting constructor
	// Pre:		the window of game, obstacles of game, level of hermit crab
	// Post:	None
	HermitCrab(RenderWindow* window, Hero *hero, vector<Sprite*>* _collisionSprites, int level = 1)
	{
		// set datas and image of hermit crab
		mainWindow = window;
		collisionSprites = _collisionSprites;
		attackInterval = (2000 - (level - 1) * 100) > 800 ? (2000 - (level - 1) * 100) : 800;
		cID = CreatureID::HermitCrab;
		face = "front";
		cTexture = new Texture;
		cSprite = new Sprite;
		gHero = hero;
		speed = 0.5;
		cTexture->loadFromFile("./image/Creature/HermitCrab/hermitCrab_" + face + ".png");
		cSprite->setTexture(*cTexture);

		// set textures of hermit crab
		for (int i = 0; i < 4; i++) cTextures.push_back(new Texture());

		cTextures[0]->loadFromFile("./image/Creature/HermitCrab/hermitCrab_front.png");
		cTextures[1]->loadFromFile("./image/Creature/HermitCrab/hermitCrab_right.png");
		cTextures[2]->loadFromFile("./image/Creature/HermitCrab/hermitCrab_back.png");
		cTextures[3]->loadFromFile("./image/Creature/HermitCrab/hermitCrab_left.png");

		// set textures of hermit crab walking
		for (int i = 0; i < 8; i++) cWalkTexture.push_back(new Texture());

		for (int i = 0, j = 0; i < cWalkTexture.size(); i += 4, j++)
		{
			cWalkTexture[i]->loadFromFile("./image/Creature/HermitCrab/hermitCrab_front_Walk" + to_string(j + 1) + ".png");
			cWalkTexture[i + 1]->loadFromFile("./image/Creature/HermitCrab/hermitCrab_right_Walk" + to_string(j + 1) + ".png");
			cWalkTexture[i + 2]->loadFromFile("./image/Creature/HermitCrab/hermitCrab_back_Walk" + to_string(j + 1) + ".png");
			cWalkTexture[i + 3]->loadFromFile("./image/Creature/HermitCrab/hermitCrab_left_Walk" + to_string(j + 1) + ".png");
		}

		// set values of hermit crab
		state = 0;
		currentHp = maxHp = level * 5;
		atk = level * 2 + level - 1;
		coin = 1 + (level - 1) * 1.5;
		exp = 5 + (level - 1) * 2;
	}

	// Intent:	update hermit crab
	// Pre:		position of hero, correction speed
	// Post:	None
	void update(Position heroPos, float correctionSpeed) override
	{
		// set sprite of hermit crab to sprite to simulate, increase size
		Sprite sprite = *cSprite;
		sprite.setPosition(sprite.getPosition().x - 250, sprite.getPosition().y - 250);
		sprite.scale(6, 6);

		// if hero overly on sprite to simulate
		if (sprite.getGlobalBounds().intersects(gHero->getSprite().getGlobalBounds()))
		{
			// set texture of hermit crab
			if (face == "front") cSprite->setTexture(*cTextures[0]);
			else if (face == "right") cSprite->setTexture(*cTextures[1]);
			else if (face == "back") cSprite->setTexture(*cTextures[2]);
			else if (face == "left") cSprite->setTexture(*cTextures[3]);

			// let the hermit crab face the position of the hero
			if (abs(getPosition().x - heroPos.x) < abs(getPosition().y - heroPos.y))
			{
				if (getPosition().y < heroPos.y)
					face = "front";
				else if (getPosition().y > heroPos.y)
					face = "back";
			}
			else
			{
				if (getPosition().x < heroPos.x)
					face = "right";
				else if (getPosition().x > heroPos.x)
					face = "left";
			}
		}
		else
		{
			clock_t walkEndTime = clock();

			// texture of hermit crab walking that change over time
			if (walkEndTime - walkStartTime >= 150)
			{
				switch (state)
				{
				case 0:
					if (face == "front") cSprite->setTexture(*cTextures[0]);
					else if (face == "right") cSprite->setTexture(*cTextures[1]);
					else if (face == "back") cSprite->setTexture(*cTextures[2]);
					else if (face == "left") cSprite->setTexture(*cTextures[3]);
					state++;
					break;
				case 1:
					if (face == "front") cSprite->setTexture(*cWalkTexture[0]);
					else if (face == "right") cSprite->setTexture(*cWalkTexture[1]);
					else if (face == "back") cSprite->setTexture(*cWalkTexture[2]);
					else if (face == "left") cSprite->setTexture(*cWalkTexture[3]);
					state++;
					break;
				case 2:
					if (face == "front") cSprite->setTexture(*cWalkTexture[4]);
					else if (face == "right") cSprite->setTexture(*cWalkTexture[5]);
					else if (face == "back") cSprite->setTexture(*cWalkTexture[6]);
					else if (face == "left") cSprite->setTexture(*cWalkTexture[7]);
					state = 0;
					break;
				}

				walkStartTime = clock();
			}
			trackHero(heroPos, correctionSpeed); // let the hermit crab walk in the direction of the hero
			attackStartTime = clock();
		}	

		bulletUpdate(correctionSpeed); // update bullets of hermit crab
	}

	// Intent:	update bullets of hermit crab
	// Pre:		correction speed
	// Post:	None
	void bulletUpdate(float correctionSpeed)
	{
		// if hermit crab is attackable, create a bullet and set position of target current position of hero 
		if (attackable(clock()))  
			bullets.push_back(new Bullet(mainWindow, collisionSprites, getPosition(), Size(cTexture->getSize().x, cTexture->getSize().y), gHero->getPosition(), face, face, "hermitCrab", 610, 1.5 + correctionSpeed));

		for (int i = 0; i < bullets.size(); i++)
		{
			bullets[i]->moveBullet(); // move bullets

			// set sprite of hero to sprite to simulate, and decrease size
			Sprite sprite = gHero->getSprite();
			sprite.setPosition(sprite.getPosition().x + 100, sprite.getPosition().y + 100);
			sprite.setScale(0.3, 0.3);

			// if bullet is overly on hero
			if (sprite.getGlobalBounds().intersects(bullets[i]->getSprite()->getGlobalBounds()))
			{
				gHero->damage(getATK()); // hero get damage

				// delete bullet
				delete bullets[i];
				bullets.erase(bullets.begin() + i);
			}
		}

		for (int i = 0; i < bullets.size(); i++)
		{
			// calculate current distance
			int nowDistance = sqrt(pow(bullets[i]->getStartPos()->x - bullets[i]->getSprite()->getPosition().x, 2) + pow(bullets[i]->getStartPos()->y - bullets[i]->getSprite()->getPosition().y, 2));

			// if the distance is greater than buttle max distance
			if (bullets[i]->getDistance() <= nowDistance)
			{
				// delete bullet
				delete bullets[i];
				bullets.erase(bullets.begin() + i);
			}
		}

		for (int i = 0; i < bullets.size(); i++)
		{
			clock_t endTime = clock();

			// if bullet can exist time up
			if (endTime - *(bullets[i]->getStartTime()) >= 6000)
			{
				// delete bullet
				delete bullets[i];
				bullets.erase(bullets.begin() + i);
			}
		}
	}

	// Intent:  hermit crab move 
	// Pre:		delta x, delta y
	// Post:	None
	void move(float deltaX, float deltaY) override
	{
		// set sprite of hero to sprite to simulate, move
		Sprite sprite = *cSprite;
		sprite.move(deltaX, deltaY);
		bool canMove = true;

		for (int i = 0; i < collisionSprites->size(); i++)
		{
			// if hermit crab will be overly on obstacles 
			if ((*collisionSprites)[i]->getGlobalBounds().intersects(sprite.getGlobalBounds()))
			{
				canMove = false;
				break;
			}
		}

		if (canMove)
		{
			// set hermit crab' face 
			if (deltaX > 0) face = "right";
			else if (deltaX < 0) face = "left";
			else if (deltaY > 0) face = "front";
			else if (deltaY < 0) face = "back";

			cSprite->move(deltaX, deltaY);
		}
	}

	// Intent:  move hermit crab and bullets 
	// Pre:		delta x, delta y
	// Post:	None
	void followMove(float deltaX, float deltaY) override
	{
		// move all objects
		cSprite->move(deltaX, deltaY);

		for (int i = 0; i < bullets.size(); i++)
		{
			bullets[i]->followMove(deltaX, deltaY);
		}
	}

	// Intent:  draw all objects of hermit crab to the window of game 
	// Pre:		None
	// Post:	None
    void drawInWindow() override
	{
		// draw all objects to the window
		mainWindow->draw(*cSprite);

		for (int i = 0; i < bullets.size(); i++) bullets[i]->drawInWindow();
	}

	// Intent:  get the center position of hermit crab
	// Pre:		None
	// Post:	the center position of hermit crab
	Position getPosition()
	{
		return Position(cSprite->getPosition().x + cTexture->getSize().x / 2, cSprite->getPosition().y + cTexture->getSize().y / 2);
	}
};


