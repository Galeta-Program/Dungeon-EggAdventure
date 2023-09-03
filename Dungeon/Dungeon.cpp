/***********************************************************************
 * File: Dungeon.cpp
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for Dungeon
***********************************************************************/
#include "Dungeon.h"

// Intent:	setting constructor
// Pre:		the window of game, save file name, font of game
// Post:	None
Dungeon::Dungeon(RenderWindow* window, string _fileName, Font* _font)
{
	mainWindow = window;
	fileName = _fileName;
	font = _font;

	// Initialize rooms pattern and max width number of rooms
	roomPattern.assign(10, vector<vector<int>>());
	roomTotalSize.assign(10, 0);

	merchantShop = new MerchantShop(mainWindow, gHero, &heroBackpack); // create a merchant shop
	ironsmithShop = new IronsmithShop(mainWindow, gHero, &heroBackpack); // create a ironsmith shop
	saveFile.open("./resources/data/" + fileName + ".txt"); // open save file
	readSave(); // call readSave() to read all data of save
	merchantShop->setHero(gHero);
	ironsmithShop->setHero(gHero);
	gHero->setPosition((mainWindow->getSize().x - gHero->getSize().width) / 2, (mainWindow->getSize().y - gHero->getSize().height) / 2);

	// set image of market save point
	savePointTexture.loadFromFile("./image/savepoint.png");
	savePointSprite.setTexture(savePointTexture);
	savePointSprite.setPosition((mainWindow->getSize().x - savePointTexture.getSize().x) / 2, (mainWindow->getSize().y - savePointTexture.getSize().y) / 2);

	// set image of dungeon entrance 
	entranceTexture.loadFromFile("./image/Entrance.png");
	entranceSprite.setTexture(entranceTexture);
	entranceSprite.setPosition(mainWindow->getSize().x / 2 - entranceTexture.getSize().x / 2, 0);

	// set image of character menu
	characterMenuTexture.loadFromFile("./image/character_menu.png");
	characterMenuSprite.setTexture(characterMenuTexture);
	characterMenuSprite.setScale(static_cast<float>(mainWindow->getSize().x / characterMenuSprite.getLocalBounds().width), static_cast<float>(mainWindow->getSize().y / characterMenuSprite.getLocalBounds().height));

	// set text of character menu information1
	characterMenuInfo1.setCharacterSize(60);
	characterMenuInfo1.setFillColor(Color::Black);
	characterMenuInfo1.setFont(*font);
	characterMenuInfo1.setPosition(mainWindow->getSize().x * 0.67, 100);

	// set text of character menu information2
	characterMenuInfo2.setCharacterSize(50);
	characterMenuInfo2.setFillColor(Color::Black);
	characterMenuInfo2.setFont(*font);
	characterMenuInfo2.setPosition(mainWindow->getSize().x * 0.55, mainWindow->getSize().y * 0.6);

	// set text of character menu information3
	characterMenuInfo3.setCharacterSize(50);
	characterMenuInfo3.setFillColor(Color::Black);
	characterMenuInfo3.setFont(*font);
	characterMenuInfo3.setPosition(mainWindow->getSize().x * 0.75, mainWindow->getSize().y * 0.6);

	// set text of saved
	savedText.setCharacterSize(30);
	savedText.setFillColor(Color::Green);
	savedText.setFont(*font);
	savedText.setString("Saved");
	savedText.setPosition(savePointSprite.getPosition().x + 80, savePointSprite.getPosition().y);

	// set image of inventory selection box
	chooseTexture.loadFromFile("./image/itembox_choose.png");
	chooseSprite.setTexture(chooseTexture);

	// load bgms and sounds
	storeBGMBuffer.loadFromFile("./sound/storeBGM.wav");
	walkingSoundBuffer.loadFromFile("./sound/walking.wav");
	buySoundBuffer.loadFromFile("./sound/buy.wav");
	drinkSoundBuffer.loadFromFile("./sound/drink.wav");
	backHomeSoundBuffer.loadFromFile("./sound/backHome.wav");
	swordSoundBuffer.loadFromFile("./sound/sword.wav");
	bowSoundBuffer.loadFromFile("./sound/bow.wav");
	magicSoundBuffer.loadFromFile("./sound/magic.wav");

	for (int i = 0; i < 3; i++) // the bgms of dungeon have three 
	{
		dungeonBGMBuffers.push_back(SoundBuffer());
		dungeonBGMBuffers.back().loadFromFile("./sound/dungeonBGM" + to_string(i + 1) + ".wav");
	}

	// set bgms and sounds to players
	walkingSound.setBuffer(walkingSoundBuffer);
	walkingSound.setVolume(soundVolume);
	dungeonBGM.setVolume(bgmVolume);
	attackSound.setVolume(soundVolume);
	actionSound.setVolume(soundVolume);

	// set player of market bgm
	storeBGM.setBuffer(storeBGMBuffer);
	storeBGM.setLoop(true);
	storeBGM.setVolume(bgmVolume);
	storeBGM.play();

	// if the number of levels that the maximum width number of rooms will increase next time is 0, initialize number
	if (maxLevel == 0)
	{
		maxLevel = 10;
		minLevel = 0;
		roomMaxSize = 2;
	}

	// if the maximum width number of room for ten levels is no data, 
	// call createLevelPattern() to create the pattern of ten levels 
	if (roomTotalSize[0] == 0) createLevelPattern();

	// set hero informations
	for (int i = 0; i < HERO_INFO_NUM; i++)
	{
		heroInfoTexture.push_back(new Texture());
		heroInfoText.push_back(Text());
		heroInfoSprite.push_back(Sprite());

		// set text of hero information
		heroInfoText.back().setFont(*font);
		heroInfoText.back().setCharacterSize(50);
		heroInfoText.back().setFillColor(Color::Black);

		switch (i) // set icons of information
		{
		case 0:
			heroInfoTexture.back()->loadFromFile("./image/HPicon.png");
			heroInfoText.back().setString(to_string(gHero->getCurrentHP()) + "/" + to_string(gHero->getMaxHP()));
			break;
		case 1:
			heroInfoTexture.back()->loadFromFile("./image/MPicon.png");
			heroInfoText.back().setString(to_string(gHero->getCurrentMP()) + "/" + to_string(gHero->getMaxMP()));
			break;
		case 2:
			heroInfoTexture.back()->loadFromFile("./image/ATKicon.png");
			heroInfoText.back().setString(to_string(gHero->getATK()));
			break;
		case 3:
			heroInfoTexture.back()->loadFromFile("./image/COINicon.png");
			heroInfoText.back().setString(to_string(gHero->getCoin()));
			break;
		case 4:
			heroInfoTexture.back()->loadFromFile("./image/LEVELicon.png");
			heroInfoText.back().setString(to_string(totalLevel + nowLevel));
			break;
		}

		heroInfoSprite.back().setTexture(*heroInfoTexture.back());
		heroInfoSprite.back().setPosition(25, 25 + (heroInfoTexture.back()->getSize().y + 25) * i);
		heroInfoText.back().setPosition(50 + heroInfoTexture.back()->getSize().x, 25 + (heroInfoTexture.back()->getSize().y + 25) * i);
	}

	Item* item;

	// create and set itrms of hero backpack 
	for (int i = 0; i < 10; i++)
	{
		item = new Item(mainWindow, (*gHero->getBackpack())[1][i], 0, (*gHero->getBackpack())[0][i]);
		heroBackpack.push_back(*item);
	}

	// create and set itrms of hero equipment
	for (int i = 0; i < 3; i++)
	{
		item = new Item(mainWindow, 0, 0, (*gHero->getEquipment())[i] * (8 + i));
		item->setPosition(mainWindow->getSize().x * 0.53, mainWindow->getSize().y / 6 + (20 + item->getSize().height) * i);
		heroEquipment.push_back(*item);
	}

	// create and set itrms of hero inventory
	for (int i = 0; i < 3; i++)
	{
		item = new Item(mainWindow, (*gHero->getInventory())[1][i], 0, (*gHero->getInventory())[0][i]);
		item->setPosition(mainWindow->getSize().x * 0.62 + (70 + item->getSize().width) * i, mainWindow->getSize().y - item->getSize().height - 10);
		heroInventory.push_back(*item);
	}

	save();
}

// Intent:	get the number of levels where the hero is now, with ten levels as a unit
// Pre:		None
// Post:	the number of levels where the hero is now, with ten levels as a unit
int Dungeon::getNowLevel()
{
	return nowLevel;
}

// Intent:	deal with event of dungeon
// Pre:		event of game
// Post:	None
void Dungeon::systemEvent(Event* event)
{
	// character movement speed, which is enhanced when wearing boots
	float heroSpeedRate = gHero->getSpeed() + (*gHero->getEquipment())[2] * gHero->getBoots()->getSpeedUpRate();

	// ***adjusting the game speed due to inventory items causing lag
	correctionSpeed = 0.8 * ((*gHero->getInventory())[0][0] == 0 ? 0 : 1) + 0.8 * ((*gHero->getInventory())[0][1] == 0 ? 0 : 1) + 0.8 * ((*gHero->getInventory())[0][2] == 0 ? 0 : 1);

	// if pressed key A and hero state isn't frozen, go left
	if (Keyboard::isKeyPressed(Keyboard::A) && gHero->getState() != State::Frozen)
	{
		// call tryMove() to check if the next position to the left is movable, but if nowLevel isn't 0, don't move the hero. 
		// Instead, move other objects to keep the hero in the center of the screen
		if (tryMove(-(heroSpeedRate + correctionSpeed), 0) && nowLevel == 0) gHero->move(-(heroSpeedRate + correctionSpeed), 0);
	}

	// if pressed key D and hero state isn't frozen, go right
	if (Keyboard::isKeyPressed(Keyboard::D) && gHero->getState() != State::Frozen)
	{
		// call tryMove() to check if the next position to the right is movable, but if nowLevel isn't 0, don't move the hero. 
		// Instead, move other objects to keep the hero in the center of the screen
		if (tryMove((heroSpeedRate + correctionSpeed), 0) && nowLevel == 0) gHero->move((heroSpeedRate + correctionSpeed), 0);
	}

	// if pressed key W and hero state isn't frozen, go up
	if (Keyboard::isKeyPressed(Keyboard::W) && gHero->getState() != State::Frozen)
	{
		// call tryMove() to check if the next position to the up is movable, but if nowLevel isn't 0, don't move the hero. 
		// Instead, move other objects to keep the hero in the center of the screen
		if (tryMove(0, -(heroSpeedRate + correctionSpeed)) && nowLevel == 0) gHero->move(0, -(heroSpeedRate + correctionSpeed));
	}

	// if pressed key S and hero state isn't frozen, go down
	if (Keyboard::isKeyPressed(Keyboard::S) && gHero->getState() != State::Frozen)
	{
		// call tryMove() to check if the next position to the down is movable, but if nowLevel isn't 0, don't move the hero. 
		// Instead, move other objects to keep the hero in the center of the screen
		if (tryMove(0, (heroSpeedRate + correctionSpeed)) && nowLevel == 0) gHero->move(0, (heroSpeedRate + correctionSpeed));
	}

	// if pressed key E and the menu of merchant shop and the menu of ironsmith shop isn't open
	if (Keyboard::isKeyPressed(Keyboard::E) && !merchantShop->isMenuOpen && !ironsmithShop->isMenuOpen && keyECanClick)
	{
		keyECanClick = false;
		keyETime = clock(); // reset time
		isBackpackOpen = !isBackpackOpen; // set hero backpack on or off
	}

	// calculate the hero backpack can on or off time
	if (!keyECanClick && clock() - keyETime >= 500)
	{
		keyECanClick = true;
	}

	// if pressed key ESC, close the menu of merchant shop and the menu of ironsmith shop and hero backpack
	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		if (merchantShop->isMenuOpen || ironsmithShop->isMenuOpen || isBackpackOpen)
		{
			merchantShop->isMenuOpen = false; // close the menu of merchant shop
			ironsmithShop->isMenuOpen = false; // close the menu of ironsmith shop
			isBackpackOpen = false; // close hero backpack
			pauseTime = clock(); // reset time
		}
	}

	// if pressed key Num1, choose first of inventory index
	if (Keyboard::isKeyPressed(Keyboard::Num1))
	{
		inventoryIndex = 0;
	}

	// if pressed key Num1, choose second of inventory index
	if (Keyboard::isKeyPressed(Keyboard::Num2))
	{
		inventoryIndex = 1;
	}

	// if pressed key Num1, choose third of inventory index
	if (Keyboard::isKeyPressed(Keyboard::Num3))
	{
		inventoryIndex = 2;
	}

	// if event isn't mouse wheel scrolled, mouse wheel can be scrolled again
	if (event->type != Event::MouseWheelScrolled)
	{
		isCanScroll = true;
	}

	// if mouse wheel scrolled, change index of inventory
	if (event->type == Event::MouseWheelScrolled && event->mouseWheelScroll.wheel == Mouse::VerticalWheel)
	{
		if (event->mouseWheelScroll.delta > 0 && isCanScroll) // If the mouse wheel scrolls up, index of inventory decrease
		{
			isCanScroll = false;
			inventoryIndex--;

			if (inventoryIndex < 0) inventoryIndex = 2;
		}
		else if (event->mouseWheelScroll.delta < 0 && isCanScroll) // If the mouse wheel scrolls down, index of inventory increase
		{
			isCanScroll = false;
			inventoryIndex++;

			if (inventoryIndex > 2) inventoryIndex = 0;
		}
	}

	// if the mouse is moved, change the hero's orientation towards the mouse cursor
	if (event->type == Event::MouseMoved)
	{
		// If the difference between the mouse cursor y position and the character center y is 
		// greater than the difference between the mouse cursor x position and the character center x
		if (abs(event->mouseMove.x - gHero->getPosition().x) < abs(event->mouseMove.y - gHero->getPosition().y))
		{
			if (event->mouseMove.y > gHero->getSprite().getPosition().y + gHero->getSize().height / 2)
				gHero->setFace("front");
			else if (event->mouseMove.y < gHero->getSprite().getPosition().y + gHero->getSize().height / 2)
				gHero->setFace("back");
		}
		else  // If the difference between the mouse cursor y position and the character center y is 
		{     // less than the difference between the mouse cursor x position and the character center x

			if (event->mouseMove.x > gHero->getSprite().getPosition().x + gHero->getSize().width / 2)
				gHero->setFace("right");
			else if (event->mouseMove.x < gHero->getSprite().getPosition().x + gHero->getSize().width / 2)
				gHero->setFace("left");
		}
	}

	//  attack or use items, if is pressed left mouse, three menus isn't open, and hero state isn't frozen
	if (Mouse::isButtonPressed(Mouse::Left) && buttonCanClick && !ironsmithShop->isMenuOpen &&
		!merchantShop->isMenuOpen && !isBackpackOpen && gHero->getState() != State::Frozen)
	{
		buttonCanClick = false;

		// attack or use items
		switch ((*gHero->getInventory())[0][inventoryIndex])
		{
		case ItemsID::Null:
			gHero->attack(correctionSpeed);
			break;

		case ItemsID::Sword:

			// if attack with sword is successful, play sword sound
			if (gHero->attack(correctionSpeed)) 
			{
				attackSound.setBuffer(swordSoundBuffer); // set sword sound to player of attack sound 

				if (attackSound.getStatus() == Sound::Stopped) attackSound.play();
			}
			break;

		case ItemsID::MagicWand:

			// if attack with magic wand is successful, play magic wand sound
			if (gHero->attack(correctionSpeed))
			{
				attackSound.setBuffer(magicSoundBuffer); // set magic wand sound to player of attack sound 

				if (attackSound.getStatus() == Sound::Stopped) attackSound.play();
			}
			break;

		case ItemsID::Bow:

			// if attack with bow is successful, play bow sound
			if (gHero->attack(correctionSpeed))
			{
				attackSound.setBuffer(bowSoundBuffer); // set bow sound to player of attack sound 

				if (attackSound.getStatus() == Sound::Stopped) attackSound.play();
			}

			// update items of hero inventory
			for (int i = 0; i < (*gHero->getInventory())[0].size(); i++)
			{
				heroInventory[i].setItem((*gHero->getInventory())[0][i]);
				heroInventory[i].setAmount((*gHero->getInventory())[1][i]);
			}
			break;

		case ItemsID::HPWater:
			gHero->setCurrentHP(gHero->getCurrentHP() + 10); // hero current hp add 10 points
			(*gHero->getInventory())[1][inventoryIndex]--; // decrease the number of items
			actionSound.setBuffer(drinkSoundBuffer); // set drink sound to player of action sound
			actionSound.play();

			// if hero current hp greater than max hp, set hero current hp is the value of max hp
			if (gHero->getCurrentHP() > gHero->getMaxHP()) gHero->setCurrentHP(gHero->getMaxHP());

			// if the number of item is 0, set id of item to 0
			if ((*gHero->getInventory())[1][inventoryIndex] <= 0) (*gHero->getInventory())[0][inventoryIndex] = 0;

			// update the item of hero inventory index
			heroInventory[inventoryIndex].setItem((*gHero->getInventory())[0][inventoryIndex]);
			heroInventory[inventoryIndex].setAmount((*gHero->getInventory())[1][inventoryIndex]);
			break;

		case ItemsID::MPWater:
			gHero->setCurrentMP(gHero->getCurrentMP() + 10); // hero current mp add 10 points
			(*gHero->getInventory())[1][inventoryIndex]--; // decrease the number of items
			actionSound.setBuffer(drinkSoundBuffer); // set drink sound to player of action sound
			actionSound.play();

			// if hero current mp greater than max mp, set hero current mp is the value of max mp
			if (gHero->getCurrentMP() > gHero->getMaxMP()) gHero->setCurrentMP(gHero->getMaxMP());

			// if the number of item is 0, set id of item to 0
			if ((*gHero->getInventory())[1][inventoryIndex] <= 0) (*gHero->getInventory())[0][inventoryIndex] = 0;

			// update the item of hero inventory index
			heroInventory[inventoryIndex].setItem((*gHero->getInventory())[0][inventoryIndex]);
			heroInventory[inventoryIndex].setAmount((*gHero->getInventory())[1][inventoryIndex]);
			break;

		case ItemsID::HomecomingMagic: 
			actionSound.setBuffer(backHomeSoundBuffer); // set back home sound to player of action sound
			actionSound.play();
			homecomingMagicSaveLevel = nowLevel; // save the value of nowLevel
			nowLevel = 0; // set nowLevel to 0
			dungeonBGM.pause(); // pause the bgm of dungeon
			storeBGM.play(); // play the bgm of store
			(*gHero->getInventory())[1][inventoryIndex]--; // decrease the number of items

			// if the number of item is 0, set id of item to 0
			if ((*gHero->getInventory())[1][inventoryIndex] <= 0) (*gHero->getInventory())[0][inventoryIndex] = 0;

			// update the item of hero inventory index
			heroInventory[inventoryIndex].setItem((*gHero->getInventory())[0][inventoryIndex]);
			heroInventory[inventoryIndex].setAmount((*gHero->getInventory())[1][inventoryIndex]);
			break;

		}
	} 
	else if (event->type == Event::MouseButtonReleased && event->mouseButton.button == Mouse::Left) // if left mouse released
	{
		buttonCanClick = true; // set button can be clicked again
	}

	// if nowLevel is 0 and hero id near the merchant shop 
	if (nowLevel == 0 && gHero->getSprite().getGlobalBounds().intersects(merchantShop->getSprite().getGlobalBounds()))
	{
		if (Mouse::isButtonPressed(Mouse::Right) && buttonCanClick)
		{
			// if pressed right mouse on the merchant shop, open the menu of merchant shop
			if (merchantShop->getSprite().getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y) &&
				!ironsmithShop->isMenuOpen && !merchantShop->isMenuOpen && !isBackpackOpen)
			{
				buttonCanClick = true;
				merchantShop->isMenuOpen = true; // open the menu of merchant shop
			}

			// if click the itme of merchant shop, and successful buy a item
			if (merchantShop->isClick(event))
			{
				actionSound.setBuffer(buySoundBuffer); // set buy sound to player of action sound 
				actionSound.play();
			}

			if (merchantShop->isMenuOpen) buttonCanClick = false;
		}
		else if (event->type == Event::MouseButtonReleased && event->mouseButton.button == Mouse::Right) // if right mouse released
		{
			buttonCanClick = true; // set button can be clicked again
		}
	}
	else merchantShop->isMenuOpen = false;

	// if nowLevel is 0 and hero id near the ironsmith shop
	if (nowLevel == 0 && gHero->getSprite().getGlobalBounds().intersects(ironsmithShop->getSprite().getGlobalBounds()))
	{
		if (Mouse::isButtonPressed(Mouse::Right) && buttonCanClick)
		{
			// if pressed right mouse on the merchant shop, open the menu of ironsmith shop
			if (ironsmithShop->getSprite().getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y) &&
				!ironsmithShop->isMenuOpen && !merchantShop->isMenuOpen && !isBackpackOpen)
			{
				buttonCanClick = true;
				ironsmithShop->isMenuOpen = true; // open the menu of ironsmith shop
			}

			// if click the itme of ironsmith shop, and successful buy a item
			if (ironsmithShop->isClick(event))
			{
				actionSound.setBuffer(buySoundBuffer); // set buy sound to player of action sound 
				actionSound.play();
			}

			if (ironsmithShop->isMenuOpen) buttonCanClick = false;
		}
		else if (event->type == Event::MouseButtonReleased && event->mouseButton.button == Mouse::Right) // if right mouse released
		{
			buttonCanClick = true; // set button can be clicked again
		}
	}
	else ironsmithShop->isMenuOpen = false;

	// if nowLevel is 0 and hero id near the save point
	if (nowLevel == 0 && gHero->getSprite().getGlobalBounds().intersects(savePointSprite.getGlobalBounds()))
	{
		if (Mouse::isButtonPressed(Mouse::Right) && buttonCanClick)
		{
			// if pressed right mouse on the save point, save game
			if (savePointSprite.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y) &&
				!ironsmithShop->isMenuOpen && !merchantShop->isMenuOpen && !isBackpackOpen)
			{
				buttonCanClick = false;
				save(); // call save() to save game data
				savedTextShow = true; // show text of saved 
				savedTextTime = clock(); // reset time
			}
		}
		else if (event->type == Event::MouseButtonReleased && event->mouseButton.button == Mouse::Right) // if right mouse released
		{
			buttonCanClick = true; // set button can be clicked again
		}
	}

	// if nowLevel is 0 and hero id near the entrance of dungeon
	if (nowLevel == 0 && gHero->getSprite().getGlobalBounds().intersects(entranceSprite.getGlobalBounds()))
	{
		if (Mouse::isButtonPressed(Mouse::Right) && buttonCanClick)
		{
			// if pressed right mouse on the entrance of dungeon, load a level
			if (entranceSprite.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y) && !ironsmithShop->isMenuOpen && !merchantShop->isMenuOpen && !isBackpackOpen)
			{
				if (homecomingMagicSaveLevel == -1) nowLevel++; // if isn't the value of save level is -1, nowlevel++(nowlLevel = 1) 
				else // if isn't the value of save level isn't -1
				{
					nowLevel = homecomingMagicSaveLevel; // set nowlevel is the value of save level
					homecomingMagicSaveLevel = -1; // set the value of save level is -1
				}

				createLevel(); // call createLevel() to generate a level of dungeon
				buttonCanClick = false;
			}
		}
		else if (event->type == Event::MouseButtonReleased && event->mouseButton.button == Mouse::Right) // if right mouse released
		{
			buttonCanClick = true;  // set button can be clicked again
		}
	}
	
	if (Mouse::isButtonPressed(Mouse::Right) && buttonCanClick && !merchantShop->isMenuOpen && !ironsmithShop->isMenuOpen)
	{
		// if right mouse pressed the items of hero backpack 
		for (int i = 0; i < heroBackpack.size(); i++)
		{
			// if click a item
			if (heroBackpack[i].getSprite().getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y) && isBackpackOpen)
			{
				if (heroBackpack[i].getID() == ItemsID::Helmet && heroEquipment[0].getID() == 0) // if the item is helmet
				{
					// set a helmet to hero equipment, decrease a helment from hero backpack
					(*gHero->getEquipment())[0] = 1;
					(*gHero->getBackpack())[1][i]--;
					heroEquipment[0].setItem(ItemsID::Helmet);
					heroEquipment[0].setAmount(1);
					heroBackpack[i].setAmount((*gHero->getBackpack())[1][i]);
					gHero->setMaxHP(gHero->getMaxHP() + gHero->getHelmet()->getHP()); // update the max hp of hero

					// if the amount of item is equal to 0, set the id of item to 0
					if (heroBackpack[i].getAmount() <= 0)
					{
						heroBackpack[i].setItem(0);
						(*gHero->getBackpack())[0][i] = 0;
					}
				}
				else if (heroBackpack[i].getID() == ItemsID::Breastplate && heroEquipment[1].getID() == 0) // if the item is breastplate
				{
					// set a breastplate to hero equipment, decrease a helment from hero backpack
					(*gHero->getEquipment())[1] = 1;
					(*gHero->getBackpack())[1][i]--;
					heroEquipment[1].setItem(ItemsID::Breastplate);
					heroEquipment[1].setAmount(1);
					heroBackpack[i].setAmount((*gHero->getBackpack())[1][i]);
					gHero->setMaxHP(gHero->getMaxHP() + gHero->getBreastplate()->getHP()); // update the max hp of hero

					// if the amount of item is equal to 0, set the id of item to 0
					if (heroBackpack[i].getAmount() <= 0)
					{
						heroBackpack[i].setItem(0);
						(*gHero->getBackpack())[0][i] = 0;
					}
				}
				else if (heroBackpack[i].getID() == ItemsID::Boots && heroEquipment[2].getID() == 0) // if the item is boots
				{
					// set a boots to hero equipment, decrease a boots from hero backpack
					(*gHero->getEquipment())[2] = 1;
					(*gHero->getBackpack())[1][i]--;
					heroEquipment[2].setItem(ItemsID::Boots);
					heroEquipment[2].setAmount(1);
					heroBackpack[i].setAmount((*gHero->getBackpack())[1][i]);
					gHero->setMaxHP(gHero->getMaxHP() + gHero->getBoots()->getHP()); // update the max hp of hero

					// if the amount of item is equal to 0, set the id of item to 0
					if (heroBackpack[i].getAmount() <= 0)
					{
						heroBackpack[i].setItem(0);
						(*gHero->getBackpack())[0][i] = 0;
					}
				}
				else if (heroBackpack[i].getID() != 0) // if the item isn't equipment and none
				{
					// find whether there is an item with the same id in the inventory
					int index = findItem(gHero->getInventory(), heroBackpack[i].getID());

					if (index != -1) // if find the same
					{
						// move items to inventory for stacking
						(*gHero->getInventory())[1][index] += (*gHero->getBackpack())[1][i];
						(*gHero->getBackpack())[1][i] = 0;
						(*gHero->getBackpack())[0][i] = 0;
						heroInventory[index].setAmount((*gHero->getInventory())[1][index]);
						heroBackpack[i].setAmount((*gHero->getBackpack())[1][i]);
						heroBackpack[i].setItem(0);
					}
					else // if didn't find the same
					{
						// find a empty slot in the inventory
						index = getSpace(gHero->getInventory());

						if (index != -1) // if find
						{
							// move items to the empty slot of inventory
							(*gHero->getInventory())[0][index] = (*gHero->getBackpack())[0][i];
							(*gHero->getInventory())[1][index] += (*gHero->getBackpack())[1][i];
							(*gHero->getBackpack())[1][i] = 0;
							(*gHero->getBackpack())[0][i] = 0;
							heroInventory[index].setItem((*gHero->getInventory())[0][index]);
							heroInventory[index].setAmount((*gHero->getInventory())[1][index]);
							heroBackpack[i].setAmount((*gHero->getBackpack())[1][i]);
							heroBackpack[i].setItem(0);
						}
					}
				}
			}
		}

		// if right mouse pressed the items of hero inventory
		for (int i = 0; i < heroInventory.size(); i++)
		{
			// if click a item
			if (heroInventory[i].getSprite().getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y) && isBackpackOpen)
			{
				if (heroInventory[i].getID() != 0)
				{
					// find whether there is an item with the same id in the backpack
					int index = findItem(gHero->getBackpack(), heroInventory[i].getID());

					if (index != -1) // if find the same
					{
						// move items to backpack for stacking
						(*gHero->getBackpack())[1][index] += (*gHero->getInventory())[1][i];
						(*gHero->getInventory())[1][i] = 0;
						(*gHero->getInventory())[0][i] = 0;
						heroBackpack[index].setAmount((*gHero->getBackpack())[1][index]);
						heroInventory[i].setAmount((*gHero->getInventory())[1][i]);
						heroInventory[i].setItem(0);
					}
					else // if didn't find the same
					{
						// find a empty slot in the backpack
						index = getSpace(gHero->getBackpack());

						if (index != -1) // if find
						{
							// move items to the empty slot of backpack
							(*gHero->getBackpack())[0][index] = (*gHero->getInventory())[0][i];
							(*gHero->getBackpack())[1][index] += (*gHero->getInventory())[1][i];
							(*gHero->getInventory())[1][i] = 0;
							(*gHero->getInventory())[0][i] = 0;
							heroBackpack[index].setItem((*gHero->getBackpack())[0][index]);
							heroBackpack[index].setAmount((*gHero->getBackpack())[1][index]);
							heroInventory[i].setAmount(0);
							heroInventory[i].setItem(0);
						}
					}
				}
			}
		}

		// if right mouse pressed the items of hero equipment
		for (int i = 0; i < heroEquipment.size(); i++)
		{
			// if click a item
			if (heroEquipment[i].getSprite().getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y) && isBackpackOpen)
			{
				if (heroEquipment[i].getID() != 0)
				{
					// find whether there is an item with the same id in the backpack
					int index = findItem(gHero->getBackpack(), heroEquipment[i].getID());

					if (index != -1) // if find the same
					{
						// move items to backpack for stacking
						(*gHero->getBackpack())[1][index] += 1;
						(*gHero->getEquipment())[i] = 0;
						heroBackpack[index].setAmount((*gHero->getBackpack())[1][index]);
						heroEquipment[i].setAmount(0);
						heroEquipment[i].setItem(0);

						// update the max hp of hero and the current hp of hero
						if (i == 0) gHero->setMaxHP(gHero->getMaxHP() - gHero->getHelmet()->getHP());

						if (i == 1) gHero->setMaxHP(gHero->getMaxHP() - gHero->getBreastplate()->getHP());

						if (i == 2) gHero->setMaxHP(gHero->getMaxHP() - gHero->getBoots()->getHP());

						if (gHero->getCurrentHP() > gHero->getMaxHP()) gHero->setCurrentHP(gHero->getMaxHP());
					}
					else // if didn't find the same
					{
						// find a empty slot in the backpack
						index = getSpace(gHero->getBackpack());

						if (index != -1) // if find
						{
							// move items to the empty slot of backpack
							(*gHero->getBackpack())[0][index] = heroEquipment[i].getID();
							(*gHero->getBackpack())[1][index] += 1;
							(*gHero->getEquipment())[i] = 0;
							heroBackpack[index].setItem((*gHero->getBackpack())[0][index]);
							heroBackpack[index].setAmount((*gHero->getBackpack())[1][index]);
							heroEquipment[i].setAmount(0);
							heroEquipment[i].setItem(0);

							// update the max hp of hero and the current hp of hero
							if (i == 0) gHero->setMaxHP(gHero->getMaxHP() - gHero->getHelmet()->getHP());

							if (i == 1) gHero->setMaxHP(gHero->getMaxHP() - gHero->getBreastplate()->getHP());

							if (i == 2) gHero->setMaxHP(gHero->getMaxHP() - gHero->getBoots()->getHP());

							if (gHero->getCurrentHP() > gHero->getMaxHP()) gHero->setCurrentHP(gHero->getMaxHP());
						}
					}
				}
			}
		}
	}
	else if (event->type == Event::MouseButtonReleased && event->mouseButton.button == Mouse::Right) // if right mouse released
	{
		buttonCanClick = true; // set button can be clicked again
	}

	// update creatures
	for (int i = 0; i < creatures.size(); i++)
	{
		if (creatures[i]->getCreatureID() != CreatureID::Dragon) // if the creeature isn't dragon
		{
			// if the creature is on the screen of game, update the creature
			if (FloatRect(0, 0, mainWindow->getSize().x, mainWindow->getSize().y).intersects(creatures[i]->getSprite()->getGlobalBounds()))
			{
				creatures[i]->update(Position(gHero->getPosition().x, gHero->getPosition().y), correctionSpeed);
			}
		}
		else creatures[i]->update(Position(gHero->getPosition().x, gHero->getPosition().y), correctionSpeed);
	}

	// if the current hp of hero less than or equal to 0
	if (gHero->getCurrentHP() <= 0)
	{
		deadAnimation(); // play animation of dead

		// delete all rooms
		for (int i = 0; i < rooms.size(); i++)
			for (int j = 0; j < rooms[i].size(); j++)
				delete rooms[i][j];

		// delete all creatures
		for (int i = 0; i < creatures.size(); i++)
			delete creatures[i];

		// initial dungeon data
		nowLevel = 0;
		gHero->setCurrentHP(gHero->getMaxHP());
		gHero->setCurrentMP(gHero->getMaxMP());
		randExitPos.clear();
		collisionSprites.clear();
		rooms.clear();
		creatures.clear();
		dungeonBGM.pause();
		storeBGM.play();
	}

	// check events of rooms  
	for (int i = 0; i < rooms.size(); i++)
	{
		for (int j = 0; j < rooms[i].size(); j++)
		{
			// if is click the door of going next level
			if (rooms[i][j]->update(clock(), correctionSpeed, event))
			{
				nowLevel++; // increase nowLevel

				if (nowLevel > 10) // if nowLevel greater than 10
				{
					totalLevel += nowLevel - 1; // nowLevel add to totalLevel
					nowLevel = 0; // set nowLevel to 0

					// delete all rooms
					for (int i = 0; i < rooms.size(); i++)
						for (int j = 0; j < rooms[i].size(); j++)
							delete rooms[i][j];

					// delete all creatures
					for (int i = 0; i < creatures.size(); i++)
						delete creatures[i];

					// initial dungeon data
					rooms.clear();
					randExitPos.clear();
					collisionSprites.clear();
					creatures.clear();
					createLevelPattern();
					dungeonBGM.pause();
					storeBGM.play();
					return;
				}
				createLevel(); // call createLevel() to generate a level of dungeon
			}

			// If the conditions for opening the door have been met, set rooms exit is open
			if (updateCondition()) rooms[i][j]->isExitOpen = true;

			rooms[i][j]->setText(to_string(conditionNume) + "/" + to_string(conditionDeno)); // set text of condition
		}
	}

	// if bgms of dungeon is stopped, random play next bgm
	if (nowLevel != 0 && dungeonBGM.getStatus() == Sound::Stopped)
	{
		int bgmIndex = rand() % dungeonBGMBuffers.size(); // random choose bgm
		dungeonBGM.setBuffer(dungeonBGMBuffers[bgmIndex]);
		dungeonBGM.play();
	}

	gHero->stateEvent(); // update hero state

	attackEvent(); // update attack event
}

// Intent:	test move to next position of hero, if nowLevel isn't 0, don't move the hero. Instead, move other objects 
//          to keep the hero in the center of the screen		
// Pre:		x of position, y of position
// Post:	hero whether movable
bool Dungeon::tryMove(int x, int y)
{
	Sprite sprite = gHero->getSprite(); // sprite to simulate
	sprite.setPosition(sprite.getPosition().x + x + 80, sprite.getPosition().y + y + 80); // move sprite to simulate
	sprite.setScale(0.35, 0.35); // decrease size of sprite to simulate

	if (walkingSound.getStatus() == Sound::Stopped) walkingSound.play(); // play walking sound

	if (nowLevel == 0)
	{
		if (sprite.getGlobalBounds().intersects(merchantShop->getSprite().getGlobalBounds())) return false;
		else if (sprite.getGlobalBounds().intersects(ironsmithShop->getSprite().getGlobalBounds())) return false;
		else if (sprite.getGlobalBounds().intersects(entranceSprite.getGlobalBounds()))	return false;
	}
	else // if nowLevel isn't 0
	{
		gHero->move(0, 0); // fake move

		// check hero whether overlap the sprite of rooms
		for (int i = 0; i < rooms.size(); i++)
		{
			for (int j = 0; j < rooms[i].size(); j++)
			{
				for (int k = 0; k < rooms[i][j]->getSpirte()->size(); k++)
				{
					if (sprite.getGlobalBounds().intersects((*rooms[i][j]->getSpirte())[k].getGlobalBounds())) return false;
				}
			}
		}

		// in case the player walks into the body of the dragon
		for (int i = 0; i < creatures.size(); i++)
		{
			if (creatures[i]->getCreatureID() == CreatureID::Dragon)
			{
				// create the collision block of dargon
				Sprite dragonBlock = *creatures[i]->getSprite();
				dragonBlock.move(150, 150);
				dragonBlock.setScale(0.6, 0.6);

				if (sprite.getGlobalBounds().intersects(dragonBlock.getGlobalBounds())) return false;
			}
		}

		// move all rooms
		for (int i = 0; i < roomTotalSize[nowLevel - 1] + 2; i++)
		{
			for (int j = 0; j < roomTotalSize[nowLevel - 1] + 2; j++)
			{
				rooms[i][j]->move(-x, -y);
			}
		}

		// move all creatures
		for (int i = 0; i < creatures.size(); i++)
		{
			creatures[i]->followMove(-x, -y);
		}

		// move bullets of hero
		for (int i = 0; i < gHero->getBullets()->size(); i++)
		{
			(*(*gHero->getBullets())[i]).move(-x, -y);
		}
	}

	return true;
}

// Intent:	draw the objects to the window of game	
// Pre:		None
// Post:	None
void Dungeon::drawInWindow()
{
	// update hero texture
	if (!gHero->attackMoveAppear && !gHero->walkMoveAppear)
		gHero->setTexture(gHero->getFace(), (*gHero->getEquipment())[0], (*gHero->getEquipment())[1], (*gHero->getEquipment())[2]);

	if (nowLevel == 0)
	{
		// draw the objects to the window of game
		mainWindow->draw(entranceSprite);
		mainWindow->draw(savePointSprite);
		mainWindow->draw(gHero->getSprite());

		for (int i = 0; i < gHero->getBullets()->size(); i++) (*gHero->getBullets())[i]->drawInWindow();
				
		drawInfo();
		merchantShop->drawInWindow();
		ironsmithShop->drawInWindow();
		drawBackpack();
		merchantShop->menuDrawInWindow();
		ironsmithShop->menuDrawInWindow();
	}
	else // if nowLevel is 1 to 10
	{
		// draw the objects to the window of game
		for (int i = 0; i < rooms.size(); i++)
		{
			for (int j = 0; j < rooms[i].size(); j++)
			{
				rooms[i][j]->drawInWindow();
			}
		}

		mainWindow->draw(gHero->getSprite());

		if (gHero->getState() == State::Frozen)	mainWindow->draw(*gHero->getFrozenSprite());

		for (int i = 0; i < creatures.size(); i++)
		{
			// if the creature is on the screen of game, draw the creature to the window of game
			if (FloatRect(0, 0, mainWindow->getSize().x, mainWindow->getSize().y).intersects(creatures[i]->getSprite()->getGlobalBounds()))
			{
				creatures[i]->drawInWindow();
			}
		}

		for (int i = 0; i < gHero->getBullets()->size(); i++) (*gHero->getBullets())[i]->drawInWindow();

		drawInfo();
		drawBackpack();
	}

	if (savedTextShow)
	{
		mainWindow->draw(savedText);

		// the text of saved rise
		if (clock() - savedTextTime % 500) savedText.move(0, -(0.5 + correctionSpeed * 0.5));

		if (clock() - savedTextTime >= 1000)
		{
			savedText.setPosition(savePointSprite.getPosition().x + 80, savePointSprite.getPosition().y); // set text of saved to original position
			savedTextShow = false; // hide the text of saved
		}
	}
}

// Intent:	set and draw the hero informations
// Pre:		None
// Post:	None
void Dungeon::drawInfo()
{
	// set texts of information and draw to the window of game
	for (int i = 0; i < HERO_INFO_NUM; i++)
	{
		switch (i)
		{
		case 0:
			heroInfoText[i].setString(to_string(gHero->getCurrentHP()) + "/" + to_string(gHero->getMaxHP()));
			break;
		case 1:
			heroInfoText[i].setString(to_string(gHero->getCurrentMP()) + "/" + to_string(gHero->getMaxMP()));
			break;
		case 2:
			heroInfoText[i].setString(to_string(gHero->getATK()) + (gHero->getExtraATK() != 0 ? "+" + to_string(gHero->getExtraATK()) : ""));
			break;
		case 3:
			heroInfoText[i].setString(to_string(gHero->getCoin()));
			break;
		case 4:
			heroInfoText[i].setString(to_string(totalLevel + nowLevel));
			break;
		}
		mainWindow->draw(heroInfoSprite[i]);
		mainWindow->draw(heroInfoText[i]);
	}
}

// Intent:	get the menu of merchant shop whether open
// Pre:		None
// Post:	the menu of merchant shop whether open
bool Dungeon::isMerchantShopMenuOpen()
{
	return merchantShop->isMenuOpen;
}

// Intent:	get the menu of ironsmith shop whether open
// Pre:		None
// Post:	the menu of ironsmith shop whether open
bool Dungeon::isIronsmithShopMenuOpen()
{
	return ironsmithShop->isMenuOpen;
}

// Intent:	get the menu of hero backpack whether open
// Pre:		None
// Post:	the menu of hero backpack whether open
bool Dungeon::isHeroBackpackOpen()
{
	return isBackpackOpen;
}

// Intent:	get the menu of pause can open timer 
// Pre:		None
// Post:	the menu of pause can open timer 
clock_t Dungeon::getPauseTime()
{
	return pauseTime;
}

// Intent:	set volume of all bgms
// Pre:		volume of bgm 
// Post:	None
void Dungeon::setBGMVolume(int volume)
{
	bgmVolume = volume;
	storeBGM.setVolume(bgmVolume);
	dungeonBGM.setVolume(bgmVolume);
}

// Intent:	set volume of all sounds
// Pre:		volume of sound
// Post:	None
void Dungeon::setSoundVolume(int volume)
{
	soundVolume = volume;
	attackSound.setVolume(soundVolume);
	walkingSound.setVolume(soundVolume);
	actionSound.setVolume(soundVolume);
}

// Intent:	play the animation of hero dead
// Pre:		None
// Post:	None
void Dungeon::deadAnimation()
{
	Sprite* heroDeadAnimateSprite = new Sprite(); // sprite of dead animation
	vector<Texture*> heroDeadTexture; // textures of dead animation
	clock_t deadStartTime = clock();
	clock_t deadEndTime = clock();
	heroDeadAnimateSprite->setPosition(gHero->getSprite().getPosition().x, gHero->getSprite().getPosition().y);

	for (int i = 1; i <= 7; i++) // load textures of dead animation 
	{
		heroDeadTexture.push_back(new Texture());
		heroDeadTexture.back()->loadFromFile("./image/EGG/DeadAnimation/Dead" + to_string(i) + ".png");
	}

	int textureIndex = 0; // index of dead texture 

	while (textureIndex < 10)
	{
		if (deadEndTime - deadStartTime >= 300)
		{
			if (textureIndex < 7)
			{
				// clear window and draw sprite to window of game
				mainWindow->clear(Color::Color(25, 25, 25));
				heroDeadAnimateSprite->setTexture(*heroDeadTexture[textureIndex]); // change dead texture
				mainWindow->draw(*heroDeadAnimateSprite);
				mainWindow->display();
			}

			textureIndex++; // increase index of dead texture 
			deadStartTime = clock(); // reset time
		}
		deadEndTime = clock(); // reset time
	}

	// delete objects of dead animation
	for (int i = 0; i < 7; i++) delete heroDeadTexture[i];

	delete heroDeadAnimateSprite;
}

// Intent:	set and draw the menu of hero backpack
// Pre:		None
// Post:	None
void Dungeon::drawBackpack()
{
	if (isBackpackOpen)
	{
		mainWindow->draw(characterMenuSprite); // draw background of backpack menu 

		// set and draw first information of character menu
		characterMenuInfo1.setString("LV." + to_string(gHero->getRank()) + "  |  " + to_string(gHero->getCurrentExp()) + "/" + to_string(gHero->getMaxExp()));
		mainWindow->draw(characterMenuInfo1);

		// set and draw second and third information of character menu
		string hpString = "HP  " + to_string(gHero->getCurrentHP()) + "/" + to_string(gHero->getMaxHP());
		string mpString = "MP  " + to_string(gHero->getCurrentMP()) + "/" + to_string(gHero->getMaxMP());
		string atkString = "ATK  " + to_string(gHero->getATK()) + (gHero->getExtraATK() != 0 ? "+" + to_string(gHero->getExtraATK()) : "");
		string coinString = "Coin  " + to_string(gHero->getCoin());
		string damageReductString = "DamageReduct  " + to_string((*gHero->getEquipment())[1] * gHero->getBreastplate()->getDamageReduct());
		string speedUpRateString = "SpeedUpRate   " + to_string((*gHero->getEquipment())[2] * gHero->getBoots()->getSpeedUpRate());
		characterMenuInfo2.setString(hpString + "\n" + atkString + "\n" + damageReductString.substr(0, damageReductString.find('.') + 3) + "\n" + speedUpRateString.substr(0, speedUpRateString.find('.') + 3));
		mainWindow->draw(characterMenuInfo2);
		characterMenuInfo3.setString(mpString + "\n" + coinString);
		mainWindow->draw(characterMenuInfo3);

		// set position of backpack items and draw backpack items
		for (int i = 0; i < heroBackpack.size(); i++)
		{
			heroBackpack[i].setPosition(125 + (70 + heroBackpack[i].getSize().width) * (i % 5), mainWindow->getSize().y / 6 + (heroBackpack[i].getSize().height + 30) * (i / 5));
			heroBackpack[i].drawInWindow();
		}

		// draw hero equipment items
		for (int i = 0; i < heroEquipment.size(); i++) heroEquipment[i].drawInWindow();
	}
	else
	{
		gHero->setHandWeaponID(heroInventory[inventoryIndex].getID()); // set hero hand weapon id is the item id of inventory index

		// set and draw inventory selection box
		chooseSprite.setPosition(mainWindow->getSize().x * 0.38 + (70 + heroInventory[inventoryIndex].getSize().width) * inventoryIndex - 10, mainWindow->getSize().y - heroInventory[inventoryIndex].getSize().height - 20);
		mainWindow->draw(chooseSprite);
	}

	// set position of inventory items and draw inventory items
	for (int i = 0; i < heroInventory.size(); i++)
	{
		if (isBackpackOpen) heroInventory[i].setPosition(mainWindow->getSize().x * 0.62 + (70 + heroInventory[i].getSize().width) * i, mainWindow->getSize().y - heroInventory[i].getSize().height - 10);
		else  heroInventory[i].setPosition(mainWindow->getSize().x * 0.38 + (70 + heroInventory[i].getSize().width) * i, mainWindow->getSize().y - heroInventory[i].getSize().height - 10);

		heroInventory[i].drawInWindow();
	}
}

// Intent:  create a level of dungeon
// Pre:		None
// Post:	None
void Dungeon::createLevel()
{
	// create and set a screen of loading
	mainWindow->clear(Color::Black);
	Text text("Loading...", *font, 50);
	text.setFillColor(Color::White);
	text.setPosition((mainWindow->getSize().x - text.getString().getSize() * 20) / 2, mainWindow->getSize().y / 2 - 20);
	mainWindow->draw(text);
	mainWindow->display();

	// delete all rooms
	for (int i = 0; i < rooms.size(); i++)
		for (int j = 0; j < rooms[i].size(); j++)
			delete rooms[i][j];

	// delete all creatures
	for (int i = 0; i < creatures.size(); i++)
		delete creatures[i];

	// initial
	rooms.clear();
	collisionSprites.clear();
	creatures.clear();
	roomStartPos.x = roomStartPos.y = roomExitPos.x = roomExitPos.y = 0;

	// find the position of start room and the position of exit room
	for (int i = 0; i < roomPattern[nowLevel - 1].size(); i++)
	{
		for (int j = 0; j < roomPattern[nowLevel - 1][i].size(); j++)
		{
			if (roomPattern[nowLevel - 1][i][j] == 1)
			{
				roomStartPos.x = j;
				roomStartPos.y = i;
			}
			else if (roomPattern[nowLevel - 1][i][j] == 2)
			{
				roomExitPos.x = j;
				roomExitPos.y = i;
			}
		}
	}

	// create the rooms of the level of dungeon
	for (int i = 0; i < roomTotalSize[nowLevel - 1] + 2; i++)
	{
		rooms.push_back(vector<Room*>());

		for (int j = 0; j < roomTotalSize[nowLevel - 1] + 2; j++)
		{
			rooms[i].push_back(new Room(mainWindow, gHero));
			rooms[i][j]->setRoomStyle(roomPattern[nowLevel - 1][i][j]); // set style of room, get value from roomPattern
			rooms[i][j]->move(j * ROOM_SIZE, i * ROOM_SIZE);

			// save all obstacles from all rooms to collisionSprites
			for (int k = 0; k < rooms[i][j]->getSpirte()->size(); k++)
			{
				collisionSprites.push_back(&(*rooms[i][j]->getSpirte())[k]);
			}
		}
	}

	// create creatures of the level of dungeon
	for (int i = 0; i < roomTotalSize[nowLevel - 1] + 2; i++)
	{
		for (int j = 0; j < roomTotalSize[nowLevel - 1] + 2; j++)
		{
			rooms[i][j]->move(-roomStartPos.x * ROOM_SIZE, -roomStartPos.y * ROOM_SIZE); // move to start room position

			if (rooms[i][j]->getCreaturePos()->size() > 0)
			{
				int cPosNum = rand() % (rooms[i][j]->getCreaturePos()->size() + 1); // random the number of creature on the room
				vector<Position> cPos = *(rooms[i][j]->getCreaturePos());  // get position that creatures can generated 

				 // the style11 of room need creature a dargon, so the number of creature is max
				if (rooms[i][j]->getRoomStyle() == 11) cPosNum = (rooms[i][j]->getCreaturePos()->size());

				// generated the random creatures of this room
				while (cPos.size() > 0 && cPosNum > 0)
				{
					int index = rand() % cPos.size();
					int cType = rand() % 3;

					if (rooms[i][j]->getRoomStyle() == 11) cType = CreatureID::Dragon; // the dargon only can generated on the style11 of room

					switch (cType)
					{
					case CreatureID::Slime:
						creatures.push_back(new class Slime(mainWindow, &collisionSprites, 1 + (totalLevel + nowLevel) / 5));
						break;
					case CreatureID::HermitCrab:
						creatures.push_back(new class HermitCrab(mainWindow, gHero, &collisionSprites, 1 + (totalLevel + nowLevel) / 6));
						break;
					case CreatureID::Turtle:
						creatures.push_back(new class Turtle(mainWindow, gHero, &collisionSprites, 1 + (totalLevel + nowLevel) / 10));
						break;
					case CreatureID::Dragon:
						creatures.push_back(new class Dragon(mainWindow, gHero, &collisionSprites, 1 + (totalLevel + nowLevel) / 11));
						break;
					}
					creatures[creatures.size() - 1]->setPosition(cPos[index].x, cPos[index].y);
					cPos.erase(cPos.begin() + index); // delete already used position
					cPosNum--;
				}
			}
		}
	}

	// play random a bgm of dungeon
	int bgmIndex = rand() % dungeonBGMBuffers.size();
	storeBGM.pause();
	dungeonBGM.pause();
	dungeonBGM.setBuffer(dungeonBGMBuffers[bgmIndex]);
	dungeonBGM.play();

	// set the position of hero to the center of screen
	gHero->setPosition((mainWindow->getSize().x - gHero->getSize().width) / 2, (mainWindow->getSize().y - gHero->getSize().height) / 2);
	randCondition(); // call randCondition() to create a condition of open the exit door

	// if the condition is complete puzzle, the exit room create a puzzle game
	if (conditionType == Condition::CompletePuzzle) rooms[roomExitPos.y][roomExitPos.x]->createPuzzle();
}

// Intent:  create ten pattern of level
// Pre:		None
// Post:	None
void Dungeon::createLevelPattern()
{
	// if current total level greater than maxLevel
	if (totalLevel >= maxLevel)
	{
		roomMaxSize++; // increase the max width number of room 
		minLevel = maxLevel;
		maxLevel += fibonacci(roomMaxSize) * 10; // used the number of fibonacci to increase maxLevel
	}

	roomPattern.assign(10, vector<vector<int>>()); // initial ten pattern of rooms

	for (int i = 0; i < 10; i++)
	{
		randExitPos.clear();
		roomTotalSize[i] = roomMaxSize;
		roomPattern[i].assign(roomTotalSize[i] + 2, vector<int>(roomTotalSize[i] + 2, 0)); // initial a pattern of room

		// rnadom place the position of start room 
		int rx = rand() % roomTotalSize[i] + 1;
		int ry = rand() % roomTotalSize[i] + 1;
		roomPattern[i][ry][rx] = 1;

		// adjust the probability of generated rooms
		float denominator = maxLevel - minLevel;
		float numerator = ((totalLevel + nowLevel - minLevel) <= 0) ? 0.2 : (totalLevel + nowLevel - minLevel);
		roomNum = 0; // set the number of rooms at the current level to 0

		if (i == 9) isBossRoomCreate = false; // if the number of level is 10, and will create a boss room(style 11)

		randCreateLevel(i, rx, ry, numerator / denominator + 0.3); // call randCreateLevel() to start create the pattern of rooms at this level

		// place the position of exit room 
		while (randExitPos.size() > 0)
		{
			int pos = rand() % randExitPos.size();

			if (roomPattern[i][randExitPos[pos].y][randExitPos[pos].x] == 0)
			{
				roomPattern[i][randExitPos[pos].y][randExitPos[pos].x] = 2;
				break;
			}
			else randExitPos.erase(randExitPos.begin() + pos);
		}
	}
}

// Intent:  create a pattern of room
// Pre:		the number of level, current x of position, current y of position, generation probability 
// Post:	None
void Dungeon::randCreateLevel(int levelNum, int x, int y, float prob)
{
	prob -= roomMaxSize * 0.0008; // decrease generation probability

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			// if up, down, left, and right of current position in range
			if (!(i == 0 && j == 0) && !(abs(i) == 1 && abs(j) == 1) && x + j > 0 && x + j < roomTotalSize[levelNum] + 1 && y + i > 0 && y + i < roomTotalSize[levelNum] + 1)
			{
				// if the number of room is 0, will force generated room, other will call randomSelect() to determine whether to generate
				bool isCreate = roomNum == 0 ? true : randomSelect(prob); 

				// if the number of room is in range, the position of generated is 0, and can create
				if (roomNum + 1 < roomTotalSize[levelNum] * roomTotalSize[levelNum] - 1 && roomPattern[levelNum][y + i][x + j] == 0 && isCreate)
				{
					if (!isBossRoomCreate) // if boss room(style 11) isn't yet generated
					{
						roomPattern[levelNum][y + i][x + j] = 11;
						isBossRoomCreate = true;
					}
					else roomPattern[levelNum][y + i][x + j] = rand() % 8 + 3; // generate random room style 

					roomNum++;
					randCreateLevel(levelNum, x + j, y + i, prob); // call randCreateLevel() to continue generate pattern of room
				}
				else randExitPos.push_back(Position(x + j, y + i)); // add the positions of the exit room can create
			}
		}
	}
}

// Intent: Randomly select true or false based on a given probability
// Pre:    prob - probability value between 0 and 1 (inclusive)
// Post:   Returns a random result (true or false) based on the probability
bool Dungeon::randomSelect(float prob)
{
	if (prob > 1) return true;
	else if (prob < 0) return false;

	// Create a vector with a number of true elements based on the probability
	vector<bool> randoms((int)(prob * 100), true);

	// Add false elements to the vector to complement the true elements
	for (int i = (int)(prob * 100); i < 100 - (int)(prob * 100); i++)
		randoms.push_back(false);

	// Shuffle the vector to randomize the order of true and false elements
	random_device rd;
	mt19937 generator(rd());
	shuffle(randoms.begin(), randoms.end(), generator);

	// Select a random index from the vector and return the corresponding value
	int index = rand() % randoms.size();
	return randoms[index];
}

// Intent: update the condition of open exit door
// Pre:    None
// Post:   the condition whether complete
bool Dungeon::updateCondition()
{
	switch (conditionType)
	{
	case Condition::KillAllCreature:
		// Calculate the remaining number of creatures to kill
		conditionNume = conditionDeno - creatures.size();
		break;
	case Condition::CompletePuzzle:
		// Check if the puzzle in the exit room is completed
		if (rooms[roomExitPos.y][roomExitPos.x]->isPuzzleComp())
			conditionNume = 1;
		break;
	}

	// Check if the condition is met
	if (conditionNume == conditionDeno)
		return true;
	else
		return false;
}

// Intent: random the condition for opening the exit door
// Pre:    None
// Post:   None
void Dungeon::randCondition()
{
	conditionNume = conditionDeno = 0;

	// Randomly select a condition type
	conditionType = rand() % 1;

	// Set the condition type of CompletePuzzle based on the result of randomSelect function
	conditionType = randomSelect(0.1) * Condition::CompletePuzzle;

	// Set the condition denominator based on the condition type
	switch (conditionType)
	{
	case Condition::KillAllCreature:
		conditionDeno = creatures.size();
		break;
	case Condition::CompletePuzzle:
		conditionDeno = 1;
		break;
	}
}

// Intent: perform attack events between the hero and creatures
// Pre:    None
// Post:   None
void Dungeon::attackEvent()
{
	// create a temporary sprite based on the hero's sprite and adjust its position and scale
	Sprite sprite = gHero->getSprite();
	sprite.setPosition(sprite.getPosition().x + 100, sprite.getPosition().y + 100);
	sprite.setScale(0.2, 0.2);

	for (int i = 0; i < creatures.size(); i++)
	{
		// check if the creature is not a dragon and intersects with the temporary sprite
		if (creatures[i]->getCreatureID() != CreatureID::Dragon && sprite.getGlobalBounds().intersects(creatures[i]->getSprite()->getGlobalBounds()))
		{
			// check if the creature is attackable and deal damage to the hero
			if (creatures[i]->attackable(clock()))
				gHero->damage(creatures[i]->getATK());
		}

		for (int j = 0; j < gHero->getBullets()->size(); j++)
		{
			// check if the creature is not a dragon and intersects with the bullet sprite
			if (creatures[i]->getCreatureID() != CreatureID::Dragon && (*gHero->getBullets())[j]->getSprite()->getGlobalBounds().intersects(creatures[i]->getSprite()->getGlobalBounds()))
			{
				// delete the bullet and deal damage to the creature
				delete (*gHero->getBullets())[j];
				gHero->getBullets()->erase(gHero->getBullets()->begin() + j);
				creatures[i]->damage(gHero->getATK() + gHero->getExtraATK());

				// check if the creature is defeated
				if (creatures[i]->getCurrentHp() <= 0)
				{
					// increase the hero's coin and gain experience
					gHero->setCoin(gHero->getCoin() + creatures[i]->getCoin());
					gHero->gainEXP(creatures[i]->getEXP());

					// delete the creature
					delete creatures[i];
					creatures.erase(creatures.begin() + i);
				}
			}
			else
			{
				// set the dargon harm box
				Sprite dragonBlock = *creatures[i]->getSprite();

				if (creatures[i]->getFace() == "front" || creatures[i]->getFace() == "back")
				{
					dragonBlock.move(200, 50);
					dragonBlock.setScale(0.4, 0.8);
				}
				else
				{
					dragonBlock.move(50, 200);
					dragonBlock.setScale(0.8, 0.4);
				}

				if ((*gHero->getBullets())[j]->getSprite()->getGlobalBounds().intersects(dragonBlock.getGlobalBounds()))
				{
					// delete the bullet and deal damage to the creature
					delete (*gHero->getBullets())[j];
					gHero->getBullets()->erase(gHero->getBullets()->begin() + j);
					creatures[i]->damage(gHero->getATK() + gHero->getExtraATK());

					// check if the creature is defeated
					if (creatures[i]->getCurrentHp() <= 0)
					{
						// increase the hero's coin and gain experience
						gHero->setCoin(gHero->getCoin() + creatures[i]->getCoin());
						gHero->gainEXP(creatures[i]->getEXP());

						// delete the creature
						delete creatures[i];
						creatures.erase(creatures.begin() + i);
					}
				}
			}
		}
	}
}

// Intent: Save the game progress and data to save file
// Pre:    None
// Post:   None
void Dungeon::save()
{
	// Set the hero's current HP and MP to their maximum values
	gHero->setCurrentHP(gHero->getMaxHP());
	gHero->setCurrentMP(gHero->getMaxMP());

	// Open the save file for writing and truncate its content
	saveFile.open("./resources/data/" + fileName + ".txt", std::ios::out | std::ios::trunc);

	// Write hero data to the save file
	saveFile << gHero->getRank() << " " << gHero->getCurrentHP() << " " << gHero->getMaxHP() << " " << gHero->getCurrentMP() << " " << gHero->getMaxMP()
		<< " " << gHero->getATK() << " " << gHero->getCoin() << " " << totalLevel << " " << (*gHero->getEquipment())[0] << " " << (*gHero->getEquipment())[1]
		<< " " << (*gHero->getEquipment())[2] << " " << gHero->getHandWeaponID() << " " << gHero->getCurrentExp() << " " << gHero->getMaxExp() << " " << nowLevel << endl;

	// Write backpack data to the save file
	for (int i = 0; i < (*gHero->getBackpack())[0].size(); i++)
		saveFile << (*gHero->getBackpack())[0][i] << " ";

	// Write inventory data to the save file
	for (int i = 0; i < (*gHero->getInventory())[0].size(); i++)
	{
		if (i < (*gHero->getInventory())[0].size() - 1)
			saveFile << (*gHero->getInventory())[0][i] << " ";
		else
			saveFile << (*gHero->getInventory())[0][i] << endl;
	}

	// Write backpack data to the save file
	for (int i = 0; i < (*gHero->getBackpack())[1].size(); i++)
		saveFile << (*gHero->getBackpack())[1][i] << " ";

	// Write inventory data to the save file
	for (int i = 0; i < (*gHero->getInventory())[1].size(); i++)
	{
		if (i < (*gHero->getInventory())[1].size() - 1)
			saveFile << (*gHero->getInventory())[1][i] << " ";
		else
			saveFile << (*gHero->getInventory())[1][i] << endl;
	}

	// Write merchant shop item data to the save file
	for (int i = 0; i < merchantShop->getItems()->size(); i++)
	{
		if (i < merchantShop->getItems()->size() - 1)
			saveFile << (*merchantShop->getItems())[i].getID() << " ";
		else
			saveFile << (*merchantShop->getItems())[i].getID() << endl;
	}

	// Write merchant shop item price data to the save file
	for (int i = 0; i < merchantShop->getItems()->size(); i++)
	{
		if (i < merchantShop->getItems()->size() - 1)
			saveFile << (*merchantShop->getItems())[i].getPrice() << " ";
		else
			saveFile << (*merchantShop->getItems())[i].getPrice() << endl;
	}

	// Write merchant shop item amount data to the save file
	for (int i = 0; i < merchantShop->getItems()->size(); i++)
	{
		if (i < merchantShop->getItems()->size() - 1)
			saveFile << (*merchantShop->getItems())[i].getAmount() << " ";
		else
			saveFile << (*merchantShop->getItems())[i].getAmount() << endl;
	}

	// Write ironsmith shop item data to the save file
	for (int i = 0; i < ironsmithShop->getItems()->size(); i++)
	{
		if (i < ironsmithShop->getItems()->size() - 1)
			saveFile << (*ironsmithShop->getItems())[i].getID() << " ";
		else
			saveFile << (*ironsmithShop->getItems())[i].getID() << endl;
	}

	// Write ironsmith shop item price data to the save file
	for (int i = 0; i < ironsmithShop->getItems()->size(); i++)
	{
		if (i < ironsmithShop->getItems()->size() - 1)
			saveFile << (*ironsmithShop->getItems())[i].getPrice() << " ";
		else
			saveFile << (*ironsmithShop->getItems())[i].getPrice() << endl;
	}

	// Write ironsmith shop item amount data to the save file
	for (int i = 0; i < ironsmithShop->getItems()->size(); i++)
	{
		if (i < ironsmithShop->getItems()->size() - 1)
			saveFile << (*ironsmithShop->getItems())[i].getAmount() << " ";
		else
			saveFile << (*ironsmithShop->getItems())[i].getAmount() << endl;
	}

	// Write dungeon configuration data to the save file
	saveFile << minLevel << " " << maxLevel << " " << roomMaxSize << endl;

	// Write room pattern data to the save file
	for (int i = 0; i < 10; i++)
	{
		saveFile << roomTotalSize[i] << endl;

		for (int j = 0; j < roomTotalSize[i] + 2; j++)
		{
			for (int k = 0; k < roomTotalSize[i] + 2; k++)
			{
				if (k < roomTotalSize[i] + 1)
					saveFile << roomPattern[i][j][k] << " ";
				else
					saveFile << roomPattern[i][j][k] << endl;
			}
		}
	}

	// Close the save file
	saveFile.close();
}

// Intent: Read and load game progress and data from save file
// Pre:    None
// Post:   None
void Dungeon::readSave()
{
	stringstream ss;
	string input;
	std::getline(saveFile, input);
	ss << input;

	int heroData[15];  // rank currentHp maxHp currentMp maxMp atk coin levels helmet armor boots weaponID currentExp maxExp nowlevel
	vector<vector<int>> heroBackpack(2, vector<int>(10, 0));
	vector<vector<int>> heroInventory(2, vector<int>(3, 0));

	// Read hero data from the save file
	for (int i = 0; i < 15; i++) ss >> heroData[i];

	ss.clear();
	std::getline(saveFile, input);
	ss << input;

	// Read the id of hero backpack items from the save file
	for (int i = 0; i < 10; i++) ss >> heroBackpack[0][i];

	// Read the id of hero inventory items from the save file
	for (int i = 0; i < 3; i++)
	{
		ss >> heroInventory[0][i];

		if (heroInventory[0][i] == heroData[11]) inventoryIndex = i;
	}

	ss.clear();
	std::getline(saveFile, input);
	ss << input;

	// Read the number of hero backpack items from the save file
	for (int i = 0; i < 10; i++) ss >> heroBackpack[1][i];

	// Read the number of hero inventory items from the save file
	for (int i = 0; i < 3; i++) ss >> heroInventory[1][i];

	vector<bool> heroEquipment{ (bool)heroData[8], (bool)heroData[9], (bool)heroData[10] };
	totalLevel = heroData[7];
	nowLevel = heroData[14];
	string heroIcon;

	// if hero hand item id is weapon id  
	if (heroData[11] >= 0 && heroData[11] <= 4) heroIcon = "./image/EGG/id_" + to_string(heroData[11]) + "/front_" + to_string(heroData[8]) + to_string(heroData[9]) + to_string(heroData[10]) + to_string(heroData[11]) + ".png";
	else heroIcon = "./image/EGG/id_" + to_string(0) + "/front_" + to_string(heroData[8]) + to_string(heroData[9]) + to_string(heroData[10]) + to_string(0) + ".png";

	// Create a new Hero object with the loaded data
	gHero = new Hero(mainWindow, heroIcon, 0, 0, heroData[0], heroData[1], heroData[2], heroData[3], heroData[4], heroData[5], heroData[6], heroData[12], heroData[13], heroData[11]);
	gHero->setBackpack(heroBackpack);
	gHero->setEquipment(heroEquipment);
	gHero->setInventory(heroInventory);
	gHero->setCollision(&collisionSprites);

	int shopIdData[20] = { 0 };
	int shopPriceData[20] = { 0 };
	int shopAmountData[20] = { 0 };
	ss.clear();
	std::getline(saveFile, input);
	ss << input;

	// Read merchant shop the id of items from the save file
	for (int i = 0; i < 20; i++) ss >> shopIdData[i];

	ss.clear();
	std::getline(saveFile, input);
	ss << input;

	// Read merchant shop the price of items from the save file
	for (int i = 0; i < 20; i++) ss >> shopPriceData[i];

	ss.clear();
	std::getline(saveFile, input);
	ss << input;

	// Read merchant shop the amount of items from the save file
	for (int i = 0; i < 20; i++) ss >> shopAmountData[i];

	// set merchant shop the items
	for (int i = 0; i < 20; i++)
	{
		if (shopIdData[0] == 0) break;

		merchantShop->setItem(i, shopIdData[i], shopPriceData[i], shopAmountData[i]);
	}

	ss.clear();
	std::getline(saveFile, input);
	ss << input;

	// Read ironsmith shop the id of items from the save file
	for (int i = 0; i < 20; i++) ss >> shopIdData[i];

	ss.clear();
	std::getline(saveFile, input);
	ss << input;

	// Read ironsmith shop the price of items from the save file
	for (int i = 0; i < 20; i++) ss >> shopPriceData[i];

	ss.clear();
	std::getline(saveFile, input);
	ss << input;

	// Read ironsmith shop the aount of items from the save file
	for (int i = 0; i < 20; i++) ss >> shopAmountData[i];

	// set ironsmith shop the items
	for (int i = 0; i < 20; i++)
	{
		if (shopIdData[0] == 0) break;

		switch (shopIdData[i])
		{
		case ItemsID::SwordLevelUp:
			gHero->getSword()->setLevel(abs(shopAmountData[i]));
			break;
		case ItemsID::BowLevelUp:
			gHero->getBow()->setLevel(abs(shopAmountData[i]));
			break;
		case ItemsID::MagicWandLevelUp:
			gHero->getMagicWand()->setLevel(abs(shopAmountData[i]));
			break;
		case ItemsID::HelmetLevelUp:
			gHero->getHelmet()->setLevel(abs(shopAmountData[i]));
			break;
		case ItemsID::BreastplateLevelUp:
			gHero->getBreastplate()->setLevel(abs(shopAmountData[i]));
			break;
		case ItemsID::BootsLevelUp:
			gHero->getBoots()->setLevel(abs(shopAmountData[i]));
			break;
		}

		ironsmithShop->setItem(i, shopIdData[i], shopPriceData[i], shopAmountData[i]);
	}

	ss.clear();
	std::getline(saveFile, input);
	ss << input;
	// Read level dungron from the save file
	ss >> minLevel >> maxLevel >> roomMaxSize;

	// Read rooms of level from the save file
	for (int i = 0; i < 10; i++)
	{
		ss.clear();
		std::getline(saveFile, input);
		ss << input;
		ss >> roomTotalSize[i];
		roomPattern[i].assign(roomTotalSize[i] + 2, vector<int>(roomTotalSize[i] + 2, 0));

		for (int j = 0; j < roomTotalSize[i] + 2; j++)
		{
			ss.clear();
			std::getline(saveFile, input);
			ss << input;

			 // Read pattern of room
			for (int k = 0; k < roomTotalSize[i] + 2; k++)
			{
				ss >> roomPattern[i][j][k];
			}
		}
	}

	saveFile.close(); // Close the save file
}

// Intent: Calculate the nth Fibonacci number recursively
// Pre: a number
// Post: Returns the num-th Fibonacci number
int Dungeon::fibonacci(int num)
{
	// if num is 0 or 1, return num
	if (num <= 1) return num;

	// call fibonacci function with num - 1 and num - 2, and return the sum of the two recursive calls
	else return fibonacci(num - 1) + fibonacci(num - 2);
}

// Intent: find the first available space
// Pre: two-dimensional vector
// Post: the index of the first available space in the vector, or -1 if no space is available
int Dungeon::getSpace(vector<vector<int>>* arr)
{
	// iterate through the first row of the vector of vectors
	for (int i = 0; i < (*arr)[0].size(); i++)
	{
		// if the value at the current index is 0, it represents an available space
		if ((*arr)[0][i] == 0) return i; // return the index of the available space
	}

	// if no available space is found, return -1
	return -1;
}

// Intent: find the index of the same item id
// Pre: two-dimensional vector, id of wnat to find item
// Post: the index of the first occurrence of the same item id in the vector, or -1 if the item is not found
int Dungeon::findItem(vector<vector<int>>* arr, int id)
{
	// iterate through the first row of the vector of vectors
	for (int i = 0; i < (*arr)[0].size(); i++)
	{
		// if the value at the current index is the same as the input id, return the index
		if ((*arr)[0][i] == id) return i; // return the index of the same itme id
	}

	// if the item id is not found, return -1
	return -1;
}

	