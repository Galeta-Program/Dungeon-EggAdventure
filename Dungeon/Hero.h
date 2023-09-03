/***********************************************************************
 * File: Hero.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for Hero main program header
***********************************************************************/
#pragma once
#include <string>
#include "Position.h"
#include "Size.h"
#include "Weapon.h"
#include "Enum.h"
#include "Helmet.h"
#include "Breastplate.h"
#include "Boots.h"
#include "Bullet.h" 
#include "Slime.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

class Hero 
{
private: 
	RenderWindow* mainWindow; // the window of game
	string face = "front"; // hero's face

	Sprite sSprite; // sprite of hero
	Texture sTexture; // texture of hero
	Sprite frozenSprite; // sprite of frozen
	Texture frozenTexture; // texture of frozen

	int handWeaponID = 0; // the id of the item in hand
	int	currentHp; // current hp of hero
	int	maxHp;  // max hp of hero
	int	currentMp; // current mp of hero
	int	maxMp;  // msx mp of hero
	int	atk; // atk of hero 
	int extraAtk; // extra atk of hero
	int	coin;  // coin of hero
	int	sRank = 1; // rank(level) of hero
	int	sMaxExp;  // level update experience
	int	sCurrentExp; // current exp hero has	
	int state = State::None; // state of hero  
	int stateTime; // duration of state
	float speed = 1.5; // speed of hero

	vector<vector<int>> backpack; // backpack of hero
	vector<vector<int>> inventory; // inventory of hero
	vector<bool> equipment; // equipment of hero

	class Helmet helmet; // helmet of hero
	class Breastplate breastplate; // breastplate of hero
	class Boots boots; // boots of hero

	Weapon *sword = new Weapon(ItemsID::Sword); // sword of hero
	Weapon *magicWand = new Weapon(ItemsID::MagicWand); // magic wand of hero
	Weapon *bow = new Weapon(ItemsID::Bow); // bow of hero

	vector<Bullet*> bullets; // bullets of hero
	vector<Sprite*>* collisionSprites; // obstacles of game
	
	clock_t stateStartTime; // timer of state
	clock_t attackMoveStartTime; // timer of attack
	clock_t walkStartTime = clock(); // timer of walk

public:
	// Intent:	setting constructor
	// Pre:		the window of game, hero initial texture, postion of hero, rank of hero, current hp of hero, max hp of hero,
	//			current mp of hero, max mp of hero, atk of hero, coin of hero, current exp of hero, max exp of hero, the id of the item in hand
	// Post:	None
	Hero(RenderWindow* window, string fileName, int x, int y, int rank = 1, int currentHp = 10, int maxHp = 10,
		int currentMp = 10, int maxMp = 10, int atk = 1, int coin = 10, int currentExp = 0, int maxExp = 15, int handWeaponID = 0)
	{
		mainWindow = window;

		// set image of hero 
		sTexture.loadFromFile("./image/EGG/id_1/right_Attack_0001.png");
		sSprite.setTexture(sTexture);
		sTexture.loadFromFile(fileName);
		sSprite.setPosition(x, y);

		// initial
		backpack.assign(2, vector<int>(10, 0));
		inventory.assign(2, vector<int>(3, 0));
		equipment.assign(3, 0);

		// set hero data
		this->sRank = rank;
		this->sCurrentExp = currentExp;
		this->sMaxExp = maxExp;
		this->maxHp = maxHp;
		this->currentHp = currentHp;
		this->maxMp = maxMp;
		this->currentMp = currentMp;
		this->atk = atk;
		this->coin = coin;
		this->handWeaponID = handWeaponID;
		setExtraATK();
		
		// set image of frozen
		frozenTexture.loadFromFile("./image/frozen.png");
		frozenSprite.setTexture(frozenTexture);
	};

	bool attackMoveAppear = false; // whether show attack action
	bool walkMoveAppear = false; // whether show walk action

	// Intent:	set sprite of obstacles
	// Pre:		sprite of obstacles
	// Post:	None
	void setCollision(vector<Sprite*>* _collisionSprite)
	{
		collisionSprites = _collisionSprite;
	}

	// Intent:	change image of current hero 
	// Pre:		face of hero, whether wearing helmet, whether wearing breastplate, whether wearing boots, state of hero action
	// Post:	None
	void setTexture(string face, int helmet, int breastplate, int boots, string state = "")
	{
		int weaponId = 0;

		if (handWeaponID >= 0 && handWeaponID <= 4) weaponId = handWeaponID; // if the id of the item in hand is weapon ids

		if (state != "") state += "_";

		// load image of hero
		sTexture.loadFromFile("./image/EGG/id_" + to_string(weaponId) + "/" + face + "_" + state + to_string(helmet) + to_string(breastplate) + to_string(boots) + to_string(weaponId) + ".png");
		this->face = face; // set face
	}

	// Intent:	set hero backpack
	// Pre:		backpack data
	// Post:	None
	void setBackpack(vector<vector<int>> _backpack)
	{
		backpack = _backpack;
	}

	// Intent:	set hero inventory
	// Pre:		inventory data
	// Post:	None
	void setInventory(vector<vector<int>> _inventory)
	{
		inventory = _inventory;
	}

	// Intent:	set hero equipment
	// Pre:		equipment data
	// Post:	None
	void setEquipment(vector<bool> _equipment)
	{
		equipment = _equipment;
	}

	// Intent:	set position of hero
	// Pre:		x of position, y of position
	// Post:	None
	void setPosition(int x, int y) 
	{
		sSprite.setPosition(x, y);
	}

	// Intent:	get size of hero
	// Pre:		None
	// Post:	size of hero image
	Size getSize()
	{
		return Size(sTexture.getSize().x, sTexture.getSize().y);
	}

	// Intent:	get sprite of hero
	// Pre:		None
	// Post:	sprite of hero
	Sprite getSprite()
	{
		return sSprite;
	}

	// Intent:	get sprite of frozen
	// Pre:		None
	// Post:	sprite of frozen
	Sprite* getFrozenSprite()
	{
		return &frozenSprite;
	}

	// Intent:	deal with event of hero  bullets
	// Pre:		None
	// Post:	None
	void bulletEvent()
	{
		for (int i = 0; i < bullets.size(); i++)
		{
			bullets[i]->moveBullet(); // move bullet

			// calculate current distance
			int nowDistance = sqrt(pow(bullets[i]->getStartPos()->x - bullets[i]->getSprite()->getPosition().x, 2) + pow(bullets[i]->getStartPos()->y - bullets[i]->getSprite()->getPosition().y, 2));

			// if the distance is greater than buttle max distance
			if (nowDistance >= bullets[i]->getDistance())
			{
				// delete bullet
				delete bullets[i];
				bullets.erase(bullets.begin() +  i);
				return;
			}
		}

		// if bullet of hero overly on obstacles, delete bullet
		for (int i = 0; i < bullets.size(); i++)
		{
			for (int j = 0; j < collisionSprites->size(); j++)
			{
				if (bullets[i]->getSprite()->getGlobalBounds().intersects((*collisionSprites)[i]->getGlobalBounds()))
				{
					//  delete bullet
					delete bullets[i];
					bullets.erase(bullets.begin() + i);
					return;
				}
			}
		}		
	}

	// Intent:	deal with all event of hero
	// Pre:		None
	// Post:	None
	void stateEvent()
	{		
		// if time up, recover from abnormal state
		if (state != State::None && clock() - stateStartTime >= stateTime)
		{
			state = State::None;
		}

		// if time up and hero isn't attacking
		if (clock() - attackMoveStartTime >= 100)
		{
			attackMoveAppear = false;
		}

		// if time up and hero isn't walking
		if (clock() - walkStartTime >= 400)
		{
			walkStartTime = clock();
			walkMoveAppear = false;
		}

		bulletEvent(); // event of bullets
	}

	// Intent:	hero move
	// Pre:		delta x, delta y
	// Post:	None
	void move(int x, int y)
	{
		// if next step of hero on screen
		if (sSprite.getPosition().x + x >= -50 && sSprite.getPosition().x + x <= mainWindow->getSize().x - sSprite.getLocalBounds().width + 50)
		{
			if (sSprite.getPosition().y + y >= -50 && sSprite.getPosition().y + y <= mainWindow->getSize().y - sSprite.getLocalBounds().height + 50)
			{
				sSprite.move(x, y); // move sprite of hero

				// texture of hero walking that change over time
				if (clock() - walkStartTime <= 100)
				{
					setTexture(face, equipment[0], equipment[1], equipment[2], "Walk1");
					walkMoveAppear = true;
				}
				else if (clock() - walkStartTime > 100 && clock() - walkStartTime <= 200)
				{
					setTexture(face, equipment[0], equipment[1], equipment[2]);
					walkMoveAppear = true;
				}
				else if (clock() - walkStartTime > 200 && clock() - walkStartTime <= 300)
				{
					setTexture(face, equipment[0], equipment[1], equipment[2], "Walk2");
					walkMoveAppear = true;
				}
				else if (clock() - walkStartTime > 300 && clock() - walkStartTime <= 350)
				{
					walkMoveAppear = true;
					walkStartTime = clock();
				}
			}
		}	
	}

	// Intent:	hero attack
	// Pre:		correction speed
	// Post:	hero attack whether success
	bool attack(float correctionSpeed)
	{
		int index = -1;
		bool attackSuccess = false;

		switch (handWeaponID)
		{
		case ItemsID::Null:
			// create a bullet of attack with hand
			bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(this->getSprite().getPosition().x, this->getSprite().getPosition().y), this->getSize(), face, face, "hand", 10, 3 + correctionSpeed));
			attackSuccess = true;
			break;

		case ItemsID::Sword:
			// create a bullet of attack with sword
			bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(this->getSprite().getPosition().x, this->getSprite().getPosition().y), this->getSize(), face, face, "sword", 50, 3 + correctionSpeed));
			attackSuccess = true;
			break;

		case ItemsID::Bow:
			// check inventory of hero whether have arrows
			for (int i = 0; i < inventory[0].size(); i++)
			{
				if (inventory[0][i] == ItemsID::Arrow) index = i;
			}

			if (index != -1)
			{
				// create a bullet of attack with bow 
				bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(this->getSprite().getPosition().x, this->getSprite().getPosition().y), this->getSize(), face, face, "bow", 400, 5 + correctionSpeed));
				inventory[1][index]--; // decrease the number of arrow
				attackSuccess = true;

				// if the number of arrow is equal to 0, set id to 0
				if (inventory[1][index] <= 0) inventory[0][index] = 0; 	
			}
			break;

		case ItemsID::MagicWand:
			// if current mp of hero is enough
			if (this->getCurrentMP() - (3 + magicWand->getLevel() - 1) >= 0)
			{
				// create a bullet of attack with magic wand
				bullets.push_back(new Bullet(mainWindow, collisionSprites, Position(this->getSprite().getPosition().x, this->getSprite().getPosition().y), this->getSize(), face, face, "magicwand", 300, 4 + correctionSpeed));
				this->setCurrentMP(this->getCurrentMP() - (3 + magicWand->getLevel() - 1)); // decrease current mp of hero
				attackSuccess = true;
			}
			break;
		}

		// set attack texture
		attackMoveStartTime = clock();
		attackMoveAppear = true;
		setTexture(face, equipment[0], equipment[1], equipment[2], "Attack");

		return attackSuccess;
	}

	// Intent:	hero get damage
	// Pre:		points
	// Post:	None
	void damage(int points)
	{
		// if hero is wearing breastplate, can reduct damage
		if (equipment[1]) 
			points -= points * breastplate.getDamageReduct();

		currentHp -= points;
	}

	// Intent:	get bullets of hero
	// Pre:		None
	// Post:	bullets of hero
	vector<Bullet*>* getBullets()
	{
		return &bullets;
	}

	// Intent:	get backpack of hero
	// Pre:		None
	// Post:	backpack of hero
    vector<vector<int>>* getBackpack()
	{
		return &backpack;
	}

	// Intent:	get inventory of hero
	// Pre:		None
	// Post:	inventory of hero
	vector<vector<int>>* getInventory()
	{
		return &inventory;
	}

	// Intent:	get equipment of hero
	// Pre:		None
	// Post:	equipment of hero
	vector<bool>* getEquipment()
	{
		return &equipment;
	}

	// Intent:	get center position of hero
	// Pre:		None
	// Post:	center position of hero
	Position getPosition()
	{
		return Position(sSprite.getPosition().x + sTexture.getSize().x / 2, sSprite.getPosition().y + sTexture.getSize().y / 2);
	}

	// Intent:	get rank of hero
	// Pre:		None
	// Post:	rank of hero
	int getRank()
	{
		return sRank;
	}

	// Intent:	get current exp of hero
	// Pre:		None
	// Post:	current exp of hero
	int getCurrentExp()
	{
		return sCurrentExp;
	}

	// Intent:	get max exp of hero
	// Pre:		None
	// Post:	max exp of hero
	int getMaxExp()
	{
		return sMaxExp;
	}

	// Intent:	get max hp of hero
	// Pre:		None
	// Post:	max hp of hero
	int getMaxHP()
	{
		return maxHp;
	}

	// Intent:	get current hp of hero
	// Pre:		None
	// Post:	current hp of hero
	int getCurrentHP()
	{
		return currentHp;
	}

	// Intent:	get current mp of hero
	// Pre:		None
	// Post:	current mp of hero
	int getCurrentMP()
	{
		return currentMp;
	}

	// Intent:	get max mp of hero
	// Pre:		None
	// Post:	max mp of hero
	int getMaxMP()
	{
		return maxMp;
	}

	// Intent:	get atk of hero
	// Pre:		None
	// Post:	atk of hero
	int getATK()
	{
		return atk;
	}

	// Intent:	get coin of hero
	// Pre:		None
	// Post:	coin of hero
	int getCoin()
	{
		return coin;
	}

	// Intent:	get speed of hero
	// Pre:		None
	// Post:	speed of hero
	float getSpeed()
	{
		return speed;
	}

	// Intent:	get the id of the item in hand
	// Pre:		None
	// Post:	the id of the item in hand
	int getHandWeaponID()
	{
		return handWeaponID;
	}

	// Intent:	get face of hero
	// Pre:		None
	// Post:	face of hero
	string getFace()
	{
		return face;
	}

	// Intent:	get extra atk of hero
	// Pre:		None
	// Post:	extra atk of hero
	int getExtraATK()
	{
		return extraAtk;
	}

	// Intent:	get sword of hero
	// Pre:		None
	// Post:	sword of hero
	Weapon* getSword()
	{
		return sword;
	}

	// Intent:	get magic wand of hero
	// Pre:		None
	// Post:	magic wand of hero
	Weapon* getMagicWand()
	{
		return magicWand;
	}

	// Intent:	get bow of hero
	// Pre:		None
	// Post:	bow of hero
	Weapon* getBow()
	{
		return bow;
	}

	// Intent:	get helmet of hero
	// Pre:		None
	// Post:	helmet of hero
	class Helmet* getHelmet()
	{
		return &helmet;
	}

	// Intent:	get breastplate of hero
	// Pre:		None
	// Post:	breastplate of hero
	class Breastplate* getBreastplate()
	{
		return &breastplate;
	}

	// Intent:	get boots of hero
	// Pre:		None
	// Post:	boots of hero
	class Boots* getBoots()
	{
		return &boots;
	}

	// Intent:	get abnormal state of hero
	// Pre:		None
	// Post:	abnormal state of hero
	int getState()
	{
		return state;
	}

	// Intent:	set current hp of hero
	// Pre:		hp
	// Post:	None
	void setCurrentHP(int _hp)
	{
		this->currentHp = _hp;
	}

	// Intent:	set current mp of hero
	// Pre:		mp
	// Post:	None
	void setCurrentMP(int _mp)
	{
		this->currentMp = _mp;
	}

	// Intent:	set max hp of hero
	// Pre:		hp
	// Post:	None
	void setMaxHP(int _hp)
	{
		this->maxHp = _hp;
	}

	// Intent:	set max mp of hero
	// Pre:		mp
	// Post:	None
	void setMaxMP(int _mp)
	{
		this->maxMp = _mp;
	}

	// Intent:	set atk of hero
	// Pre:		atk
	// Post:	None
	void setATK(int _atk)
	{
		this->atk = _atk;
	}

	// Intent:	set coin of hero
	// Pre:		the number of coin
	// Post:	None
	void setCoin(int _coin)
	{
		this->coin = _coin;
	}

	// Intent:	set face of hero
	// Pre:		direction
	// Post:	None
	void setFace(string _face)
	{
		this->face = _face;
	}

	// Intent:	set hand weapon id of hero
	// Pre:		id of item
	// Post:	None
	void setHandWeaponID(int _id)
	{
		handWeaponID = _id;
		setExtraATK();
	}

	// Intent:	set state of hero
	// Pre:		type of state, duration
	// Post:	None
	void setState(int _state, int duration)
	{
		state = _state;

		// set timer
		stateTime = duration;
		stateStartTime = clock(); 

		switch (state)
		{
		case State::Frozen:
			// set position of frozen image
			frozenSprite.setPosition(getPosition().x - frozenTexture.getSize().x / 2, getPosition().y - frozenTexture.getSize().y / 2);
			break;
		}
	}

	// Intent:	set extra atk of hero
	// Pre:		None
	// Post:	None
	void setExtraATK()
	{
		// if the id of the item in hand is weapons, set extraAtk to the atk of weapon
		switch (handWeaponID)
		{
		case ItemsID::Sword:
			extraAtk = sword->getATK();
			break;
		case ItemsID::MagicWand:
			extraAtk = magicWand->getATK();
			break;
		case ItemsID::Bow:
			extraAtk = bow->getATK();
			break;
		default:
			extraAtk = 0;
			break;
		}
	}

	// Intent:	hero get exp
	// Pre:		points
	// Post:	None
	void gainEXP(int points)
	{
		sCurrentExp += points; // increase current exp of hero

		// if current exp of hero greater than max exp of hero, increase the values of hero 
		while (sCurrentExp >= sMaxExp)
		{
			sRank += 1;
			atk += (sRank % 5 == 0 ? 1 : 0);
			maxHp += 2;
			currentHp += 2;
			maxMp += 2;
			currentMp += 2;
			sCurrentExp -= sMaxExp;
			sMaxExp += 15;					
		}	
	}
};
