/***********************************************************************
 * File: Slime.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for Slime main program header
***********************************************************************/
#pragma once
#include "Creature.h"

class Slime : public Creature
{
private:
	clock_t jumpStartTime = clock(); // time of slime jumping

public:
	// Intent:	setting constructor
	// Pre:		the window of game, obstacles of game, level of slime
	// Post:	None
	Slime(RenderWindow* window, vector<Sprite*>* _collisionSprites, int level = 1)
	{
		// set datas and image of slime
		mainWindow = window;
		collisionSprites = _collisionSprites;
		cID = CreatureID::Slime;
		cTexture = new Texture;
		cSprite = new Sprite;
	    cTexture->loadFromFile("./image/Creature/Slime/slime.png");
		cWalkTexture.push_back(new Texture());
		cWalkTexture.push_back(new Texture());

		// set texture of slime
		for (int i = 0; i < cWalkTexture.size(); i++)
			cWalkTexture[i]->loadFromFile("./image/Creature/Slime/slime_Walk" + to_string(i + 1) + ".png");
		
		// set values of slime
		cSprite->setTexture(*cTexture);
		state = 2;
		currentHp = maxHp = level * 10 + level - 1;
		atk = level * 1;
		coin = 1 + (level - 1) * 1.5;
		exp = 5 + (level - 1) * 2;
	}

	// Intent:	update slime
	// Pre:		position of hero, correction speed
	// Post:	None
	void update(Position heroPos, float correctionSpeed) override
	{
		clock_t jumpEndTime = clock();

		// texture of slime jumping that change over time
		if (jumpEndTime - jumpStartTime >= 150)
		{
			switch (state)
			{
			case 0:
				cSprite->setTexture(*cTexture);
				state++;
				break;
			case 1:
				cSprite->setTexture(*cWalkTexture[0]);
				state++;
				break;
			case 2:
				cSprite->setTexture(*cWalkTexture[1]);
				state = 0;
				break;
			}

			jumpStartTime = clock();
		}		

		trackHero(heroPos, correctionSpeed * 0.6); // let the slime jump in the direction of the hero
	}
};
