/***********************************************************************
 * File: Dragon.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for Dragon main program header
***********************************************************************/
#pragma once
#include "Creature.h"
#include "Hero.h"
#include "Bullet.h"

class Dragon : public Creature
{
private:
	vector<Texture*>  cAttackTextures; // dragon atacking textures
	clock_t actionStartTime = clock(); // used to calculate change action time
	clock_t gatherStartTime; // timer for gather attack of dargon
	clock_t rotateStartTime = clock(); // timer for rotation attack of dargon
	vector<Bullet*> bullets; // bullets of dargon
	Position originalPos; // used to save the original position of dargon 
	Hero* gHero; // hero
	int attackMode; // current attack mode of dargon
	int fireBallAtk; // atk of fire ball
	int tailAtk; // atk of tail attack
	int bigFireBallAtk; // atk of big fire ball
	int gatherInterval; // gaather time of dargon
	int angle = 0; // angle of dargon rotation
	vector<Sprite*> tailBulletSprites; // sprites of tail bullet
	vector<Texture*> tailBulletTextures; // textures of tail bullet
	vector<clock_t> tailAttackStartTime; // timers of calcaulate tail attacks update

public:
	// Intent:	setting constructor
	// Pre:		the window of game, hero, sprites of obstacle, level of dargon
	// Post:	None
	Dragon(RenderWindow* window, Hero* hero, vector<Sprite*>* _collisionSprites, int level = 1)
	{
		mainWindow = window;
		collisionSprites = _collisionSprites;
		attackInterval = (5000 - level * 250 > 1500) ? 5000 - level * 250 : 1500; // set attack interval of dargon
		gatherInterval = (8000 - level * 300 > 2000) ? 8000 - level * 250 : 2000; // set gather interval of dargon
		cID = CreatureID::Dragon; // set creature id

		// set data of dargon
		face = "front";		
		gHero = hero;
		speed = 0.5;
		cTexture = new Texture;
		cSprite = new Sprite;
		cTexture->loadFromFile("./image/Creature/Dragon/dragon_" + face + ".png");
		cSprite->setTexture(*cTexture);

		// set images of dargon
		for (int i = 0; i < 4; i++) cTextures.push_back(new Texture());

		cTextures[0]->loadFromFile("./image/Creature/Dragon/dragon_front.png");
		cTextures[1]->loadFromFile("./image/Creature/Dragon/dragon_right.png");
		cTextures[2]->loadFromFile("./image/Creature/Dragon/dragon_back.png");
		cTextures[3]->loadFromFile("./image/Creature/Dragon/dragon_left.png");

		// set images of dargon walking 
		for (int i = 0; i < 8; i++) cWalkTexture.push_back(new Texture());

		for (int i = 0, j = 0; i < cWalkTexture.size(); i += 4, j++)
		{
			cWalkTexture[i]->loadFromFile("./image/Creature/Dragon/dragon_front_Walk" + to_string(j + 1) + ".png");
			cWalkTexture[i + 1]->loadFromFile("./image/Creature/Dragon/dragon_right_Walk" + to_string(j + 1) + ".png");
			cWalkTexture[i + 2]->loadFromFile("./image/Creature/Dragon/dragon_back_Walk" + to_string(j + 1) + ".png");
			cWalkTexture[i + 3]->loadFromFile("./image/Creature/Dragon/dragon_left_Walk" + to_string(j + 1) + ".png");
		}

		// set images of dargon gather 
		for (int i = 0; i < 12; i++) cAttackTextures.push_back(new Texture());

		for (int i = 0, j = 0; i < cAttackTextures.size(); i += 4, j++)
		{
			cAttackTextures[i]->loadFromFile("./image/Creature/Dragon/dragon_front_Gather" + to_string(j + 1) + ".png");
			cAttackTextures[i + 1]->loadFromFile("./image/Creature/Dragon/dragon_right_Gather" + to_string(j + 1) + ".png");
			cAttackTextures[i + 2]->loadFromFile("./image/Creature/Dragon/dragon_back_Gather" + to_string(j + 1) + ".png");
			cAttackTextures[i + 3]->loadFromFile("./image/Creature/Dragon/dragon_left_Gather" + to_string(j + 1) + ".png");
		}
			
		// set values of dargon 
		state = 0;
		currentHp = maxHp = level * 300 - (level - 1) * 20;
		atk = level * 2 + level - 1;
		fireBallAtk = level * 2 + level - 1;
		tailAtk = level * 4 + level - 1;
		bigFireBallAtk = level * 5 + (level - 1) * 2;
		coin = 20 + (level - 1) * 8;
		exp = 15 + (level - 1) * 6;
	}

	// Intent:	update dargon 
	// Pre:		position of hero, correction speed
	// Post:	None
	void update(Position heroPos, float correctionSpeed) override
	{
		// set sprite of dargon to sprite to simulate, and increase size
		Sprite sprite = *cSprite;
		sprite.setPosition(sprite.getPosition().x - 400, sprite.getPosition().y - 400);
		sprite.scale(2, 2);

		// if hero overly on prite to simulate
		if (sprite.getGlobalBounds().intersects(gHero->getSprite().getGlobalBounds()))
		{
			if (attackMode != 1)
			{
				// set texture of dargon 
				if (face == "front") cSprite->setTexture(*cTextures[0]);
				else if (face == "right") cSprite->setTexture(*cTextures[1]);
				else if (face == "back") cSprite->setTexture(*cTextures[2]);
				else if (face == "left") cSprite->setTexture(*cTextures[3]);
			}
			else // if attackMode == 1
			{
				clock_t actionEndTime = clock();

				// texture of dargon attacking that change over time
				if (actionEndTime - actionStartTime >= 300)
				{
					switch (state)
					{
					case 0:
						if (face == "front") cSprite->setTexture(*cAttackTextures[0]);
						else if (face == "right") cSprite->setTexture(*cAttackTextures[1]);
						else if (face == "back") cSprite->setTexture(*cAttackTextures[2]);
						else if (face == "left") cSprite->setTexture(*cAttackTextures[3]);
						state++;
						break;
					case 1:
						if (face == "front") cSprite->setTexture(*cAttackTextures[4]);
						else if (face == "right") cSprite->setTexture(*cAttackTextures[5]);
						else if (face == "back") cSprite->setTexture(*cAttackTextures[6]);
						else if (face == "left") cSprite->setTexture(*cAttackTextures[7]);
						state++;
						break;
					case 2:
						if (face == "front") cSprite->setTexture(*cAttackTextures[8]);
						else if (face == "right") cSprite->setTexture(*cAttackTextures[9]);
						else if (face == "back") cSprite->setTexture(*cAttackTextures[10]);
						else if (face == "left") cSprite->setTexture(*cAttackTextures[11]);
						state = 0;
						break;
					}

					actionStartTime = clock();
				}
			}
			
			// let the dragon face the position of the hero
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
			clock_t actionEndTime = clock();

			// texture of dargon walking that change over time
			if (actionEndTime - actionStartTime >= 100)
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

				actionStartTime = clock();
			}

			trackHero(heroPos, correctionSpeed); // let the dragon walk in the direction of the hero
			attackStartTime = clock();
		}

		bulletUpdate(correctionSpeed); // update bullets of dargon
	}

	// Intent:	update bullets of dargon 
	// Pre:		correction speed
	// Post:	None
	void bulletUpdate(float correctionSpeed)
	{
		if (attackable(clock())) // if dargon is attackable
		{
			if (attackMode == -1) // dargon don't attacking
			{
				attackMode = rand() % 3; // randomly a attack mode

				if (attackMode == 1) gatherStartTime = clock(); // attack mode is 1, the dargon start the gather action
				else if (attackMode == 2) 
				{
					originalPos = Position(cSprite->getPosition().x, cSprite->getPosition().y); // save current position of dargon
					cSprite->setOrigin(cSprite->getLocalBounds().width / 2, cSprite->getLocalBounds().height / 2); // set the center position
					cSprite->setPosition(originalPos.x + cTexture->getSize().x / 2, originalPos.y + cTexture->getSize().y / 2); // adjest current position
					angle = 0;
				}
 			}

			if (attackMode == 0)
			{
				string bulletFace1 = "", bulletFace2 = "", bulletFace3 = "";

				// set bullets of the three direction
				if (face == "front")
				{
					bulletFace1 = face + "-right";
					bulletFace2 = face;
					bulletFace3 = face + "-left";
				}
				else if (face == "left")
				{
					bulletFace1 = "front-" + face;
					bulletFace2 = face;
					bulletFace3 = "back-" + face;
				}
				else if (face == "back")
				{
					bulletFace1 = face + "-right";
					bulletFace2 = face;
					bulletFace3 = face + "-left";
				}
				else if (face == "right")
				{
					bulletFace1 = "front-" + face;
					bulletFace2 = face;
					bulletFace3 = "back-" + face;
				}

				// create three fire ball bullets of the direction
				bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(cSprite->getPosition().x, cSprite->getPosition().y), Size(cTexture->getSize().x, cTexture->getSize().y), face, bulletFace1, "fireball", 610, 2 + correctionSpeed));
				bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(cSprite->getPosition().x, cSprite->getPosition().y), Size(cTexture->getSize().x, cTexture->getSize().y), face, bulletFace2, "fireball", 610, 2 + correctionSpeed));
				bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(cSprite->getPosition().x, cSprite->getPosition().y), Size(cTexture->getSize().x, cTexture->getSize().y), face, bulletFace3, "fireball", 610, 2 + correctionSpeed));
				attackMode = -1;
			}
		}

		if (attackMode == 1)
		{
			// if the gather time ends, create a big fire ball
			if (clock() - gatherStartTime >= gatherInterval)
			{
				bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(cSprite->getPosition().x, cSprite->getPosition().y), Size(cTexture->getSize().x, cTexture->getSize().y), face, face, "bigFireball", 800, 3 + correctionSpeed));
				attackMode = -1;
			}
		}
		else if (attackMode == 2)
		{
			// if time up, dargon rotate by a angle
			if (clock() - rotateStartTime >= 20)
			{
				angle += 20;
				cSprite->setRotation(angle);
				rotateStartTime = clock();
			}
			
			// if already rotated one full circle
			if (angle >= 360)
			{
				// set dargon to original setting
				cSprite->setRotation(0);
				cSprite->setOrigin(0, 0);
				cSprite->setPosition(originalPos.x, originalPos.y);

				attackMode = -1;

				// create a bullet of tail attack
				tailAttackStartTime.push_back(clock());
				tailBulletTextures.push_back(new Texture());
				tailBulletTextures.back()->loadFromFile("./image/Bullet/tail_bullet.png");
				tailBulletSprites.push_back(new Sprite());
				tailBulletSprites.back()->setTexture(*tailBulletTextures.back());
				tailBulletSprites.back()->setPosition(getPosition().x - tailBulletTextures.back()->getSize().x / 2, getPosition().y - tailBulletTextures.back()->getSize().y / 2);
			}
		}

		// set sprite of hero to sprite to simulate, and decrease size
		Sprite sprite = gHero->getSprite();
		sprite.setPosition(sprite.getPosition().x + 80, sprite.getPosition().y + 80);
		sprite.setScale(0.35, 0.35);

		for (int i = 0; i < bullets.size(); i++)
		{
			bullets[i]->moveBullet(); // move bullets

			// if bullet is overly on hero
			if (sprite.getGlobalBounds().intersects(bullets[i]->getSprite()->getGlobalBounds()))
			{
				// hero get damage
				if (bullets[i]->getBulletType() == "fireball") gHero->damage(fireBallAtk);
				else if (bullets[i]->getBulletType() == "bigFireball") gHero->damage(bigFireBallAtk);

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
				// if type of bullet is bigFireball, shoot fireballs in eight directions
				if (bullets[i]->getBulletType() == "bigFireball")
				{
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "front", "fireball", 800, 2 + correctionSpeed));
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "front-left", "fireball", 800, 2 + correctionSpeed));
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "left", "fireball", 800, 2 + correctionSpeed));
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "back-left", "fireball", 800, 2 + correctionSpeed));
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "back", "fireball", 800, 2 + correctionSpeed));
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "back-right", "fireball", 800, 2 + correctionSpeed));
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "right", "fireball", 800, 2 + correctionSpeed));
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "front-right", "fireball", 800, 2 + correctionSpeed));
				}

				// delete bullet
				delete bullets[i];
				bullets.erase(bullets.begin() + i);
			}
		}

		for (int i = 0; i < bullets.size(); i++)
		{
			clock_t endTime = clock();

			// if bullet can exist time up
			if (endTime - *(bullets[i]->getStartTime()) >= 5000)
			{
				// if type of bullet is bigFireball, shoot fireballs in eight directions
				if (bullets[i]->getBulletType() == "bigFireball")
				{
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "front", "fireball", 800, 2 + correctionSpeed));
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "front-left", "fireball", 800, 2 + correctionSpeed));
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "left", "fireball", 800, 2 + correctionSpeed));
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "back-left", "fireball", 800, 2 + correctionSpeed));
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "back", "fireball", 800, 2 + correctionSpeed));
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "back-right", "fireball", 800, 2 + correctionSpeed));
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "right", "fireball", 800, 2 + correctionSpeed));
					bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(bullets[i]->getSprite()->getPosition().x, bullets[i]->getSprite()->getPosition().y), Size(cTexture->getSize().x, bullets[i]->getTexture()->getSize().y), face, "front-right", "fireball", 800, 2 + correctionSpeed));
				}

				// delete bullet
				delete bullets[i];
				bullets.erase(bullets.begin() + i);
			}
		}

		for (int i = 0; i < tailBulletSprites.size(); i++)
		{
			// if is time up, increase the size of tail attack bullet
			if (clock() - tailAttackStartTime[i] >= 50)
			{
				tailBulletSprites[i]->move(-15, -15);
				tailBulletSprites[i]->setScale(tailBulletSprites[i]->getScale().x + 0.1, tailBulletSprites[i]->getScale().y + 0.1);
				tailAttackStartTime[i] = clock();
			}
			
			// if the size of tail attack bullet is greater than range, delete bullet
			if (tailBulletSprites[i]->getScale().x >= 3.5 || tailBulletSprites[i]->getScale().y >= 3.5)
			{
				// delete bullet
				delete tailBulletSprites[i];
				delete tailBulletTextures[i];
				tailBulletSprites.erase(tailBulletSprites.begin() + i);
				tailBulletTextures.erase(tailBulletTextures.begin() + i);
				tailAttackStartTime.erase(tailAttackStartTime.begin() + i);
			}
		}

		for (int i = 0; i < tailBulletSprites.size(); i++)
		{
			// if bullet is overly on hero
			if (sprite.getGlobalBounds().intersects(tailBulletSprites[i]->getGlobalBounds()))
			{
				gHero->damage(tailAtk); // hero get damage

				// delete bullet
				delete tailBulletSprites[i];
				delete tailBulletTextures[i];
				tailBulletSprites.erase(tailBulletSprites.begin() + i);
				tailBulletTextures.erase(tailBulletTextures.begin() + i);
				tailAttackStartTime.erase(tailAttackStartTime.begin() + i);
				break;
			}
		}
	}

	// Intent:  dargon move 
	// Pre:		delta x, delta y
	// Post:	None
	void move(float deltaX, float deltaY) override
	{
		// set sprite of hero to sprite to simulate, and decrease size
		Sprite sprite = *cSprite;
		sprite.move(deltaX, deltaY); // try move
		bool canMove = true;
		sprite.move(100, 100);
		sprite.setScale(0.6, 0.6);

		for (int i = 0; i < collisionSprites->size(); i++)
		{
			// if dargon will be overly on obstacles 
			if ((*collisionSprites)[i]->getGlobalBounds().intersects(sprite.getGlobalBounds()))
			{
				canMove = false;
				break;
			}
		}

		if (canMove)
		{
			// set dargon' face 
			if (deltaX > 0) face = "right";
			else if (deltaX < 0) face = "left";
			else if (deltaY > 0) face = "front";
			else if (deltaY < 0) face = "back";

			cSprite->move(deltaX, deltaY);
		}
	}

	// Intent:  move dargon and bullets 
	// Pre:		delta x, delta y
	// Post:	None
	void followMove(float deltaX, float deltaY) override
	{
		// move all objects
		cSprite->move(deltaX, deltaY);

		for (int i = 0; i < bullets.size(); i++) bullets[i]->followMove(deltaX, deltaY);
		
		for (int i = 0; i < tailBulletSprites.size(); i++) tailBulletSprites[i]->move(deltaX, deltaY);
	}

	// Intent:  draw all objects of dargon to the window of game 
	// Pre:		None
	// Post:	None
	void drawInWindow() override
	{
		// draw all objects to the window
		for (int i = 0; i < tailBulletSprites.size(); i++) mainWindow->draw(*tailBulletSprites[i]);

		mainWindow->draw(*cSprite);

		for (int i = 0; i < bullets.size(); i++) bullets[i]->drawInWindow();	
	}

	// Intent:  get the center position of dargon
	// Pre:		None
	// Post:	the center position of dargon
	Position getPosition()
	{
		return Position(cSprite->getPosition().x + cTexture->getSize().x / 2, cSprite->getPosition().y + cTexture->getSize().y / 2);
	}

	// Intent:  get face of dargon
	// Pre:		None
	// Post:	face of dargon
	string getFace() override
	{
		return face;
	}
};
