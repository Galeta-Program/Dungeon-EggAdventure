/***********************************************************************
 * File: MerchantShop.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for Merchant Shop main program header
***********************************************************************/
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Item.h"
#include "Hero.h"

using namespace std;
using namespace sf;

class MerchantShop
{
private:
	RenderWindow* mainWindow;  // the window of game
	Sprite shopSprite; // sprite of shop
	Texture shopTexture;  // texture of shop
	Sprite shopMenuSprite; // sprite of shop menu
	Texture shopMenuTexture;  // texture of shop menu
	vector<Item> items; // items
	vector<Item>* heroBackpack; // hero backpack
	Hero* gHero; // hero
	const int maxProductNum = 20; // max number of product

public:
	// Intent:	setting constructor
	// Pre:		the window of game, hero, hero backpack
	// Post:	None
	MerchantShop(RenderWindow* window, Hero *hero, vector<Item>* _heroBackpack)
	{
		// set datas and image of merchant shop
		mainWindow = window;
		gHero = hero;
		heroBackpack = _heroBackpack;
		shopTexture.loadFromFile("./image/merchant_shop.png");
		shopSprite.setTexture(shopTexture);
		shopSprite.setPosition(mainWindow->getSize().x / 4 - shopTexture.getSize().x / 2, mainWindow->getSize().y / 4 - shopTexture.getSize().y / 2);
		shopMenuTexture.loadFromFile("./image/merchant_shop_menu.png");
		shopMenuSprite.setTexture(shopMenuTexture);
		shopMenuSprite.setScale(static_cast<float>(mainWindow->getSize().x / shopMenuSprite.getLocalBounds().width), static_cast<float>(mainWindow->getSize().y / shopMenuSprite.getLocalBounds().height));	

		// add empty items
		Item *item;	

		for (int i = 0; i < maxProductNum; i++)
		{		
			item = new Item(mainWindow, 0, 0);
			item->setPosition(70 + (70 + item->getSize().width) * (i % 5),mainWindow->getSize().y / 6 + (item->getSize().height + 30) * (i / 5));
			items.push_back(*item);
		}

		// add products
		addItem(ItemsID::HPWater, 10, 0);
		addItem(ItemsID::MPWater, 10, 0);
		addItem(ItemsID::HomecomingMagic, 300, 0);
		addItem(ItemsID::Arrow, 5, 0);
	}

	bool isMenuOpen = false; // menu of shop whether open

	// Intent:	set hero
	// Pre:		hero
	// Post:	None
	void setHero(Hero* hero)
	{
		gHero = hero;
	}

	// Intent:	draw shop to the window of game
	// Pre:		None
	// Post:	None
	void drawInWindow()
	{
		// draw shop to the window
		mainWindow->draw(shopSprite);
	}

	// Intent:	draw menu of shop to the window of game
	// Pre:		None
	// Post:	None
	void menuDrawInWindow()
	{
		if (isMenuOpen) // if menu of shop is opened
		{
			// draw all objects of menu of shop 
			mainWindow->draw(shopMenuSprite);

			for (int i = 0; i < items.size(); i++)
			{
				items[i].drawInWindow();
			}

			// move and draw hero backpack on window
			for (int i = 0; i < heroBackpack->size(); i++)
			{
				(*heroBackpack)[i].setPosition(mainWindow->getSize().x / 2 + 100 + (70 + (*heroBackpack)[i].getSize().width) * (i % 5), mainWindow->getSize().y / 6 + ((*heroBackpack)[i].getSize().height + 30) * (i / 5));
				(*heroBackpack)[i].drawInWindow();
			}
		}
	}

	// Intent:	get sprite of shop
	// Pre:		None
	// Post:	sprite of shop
	Sprite getSprite()
	{
		return this->shopSprite;
	}

	// Intent:	get items of shop
	// Pre:		None
	// Post:	items of shop
	vector<Item>* getItems()
	{
		return &items;
	}

	// Intent:	add items
	// Pre:		id of item, price of item, amount of item
	// Post:	None
	void addItem(int _id, int _price, int _amount)
	{
		for (int i = 0; i < maxProductNum; i++)
		{
			// if already has the same id of item, only set price and amount
			if (_id > 0 && _amount >= 0 && items[i].getID() == _id)
			{
				items[i].setAmount(_amount);
				items[i].setPrice(_price);
				break;
			}
			else if (items[i].getID() == 0) // if there has a empty slot 
			{
				// set data of item
				items[i].setItem(_id);
				items[i].setAmount(_amount);
				items[i].setPrice(_price);
				break;
			}
		}
	}

	// Intent:	set items
	// Pre:		index of shop items, id of item, price of item, amount of item
	// Post:	None
	void setItem(int index, int _id, int _price, int _amount)
	{
		if (_id != 0)
		{
			// set data of item
			items[index].setItem(_id);
			items[index].setAmount(_amount);
			items[index].setPrice(_price);
		}	
	}

	// Intent:	deal with event of shop 
	// Pre:		event of game
	// Post:	whether click the items
	bool isClick(Event *event)
	{
		for (int i = 0; i < items.size(); i++)
		{
	        // if click the items of shop
			if (items[i].getSprite().getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y) && isMenuOpen)
			{
				// if the items isn't empty and coin of hero is enough
				if (items[i].getID() != 0 && gHero->getCoin() - items[i].getPrice() >= 0)
				{
					// find whether there is an item with the same id in the backpack
					int index = findItem(gHero->getBackpack(), items[i].getID());

					if (index != -1) // if find the same
					{
						(*gHero->getBackpack())[1][index]++; // increase amount of the items
						gHero->setCoin(gHero->getCoin() - items[i].getPrice()); // decrease coin of hero
						items[i].setAmount(items[i].getAmount() - 1); // decrease of the items amount
						(*heroBackpack)[index].setAmount((*gHero->getBackpack())[1][index]);

						// if the amount of items is 0, set the slot is empty
						if (items[i].getAmount() == 0)
						{
							items[i].setItem(0);
							items[i].setPrice(0);
						}

						return true;
					}
					else // if didn't find the same
					{
						// find a empty slot in the inventory
						index = getSpace(gHero->getBackpack());

						if (index != -1) // if find
						{
							// get the items to hero backpack
							(*gHero->getBackpack())[0][index] = items[i].getID();
							(*gHero->getBackpack())[1][index] = 1;
							gHero->setCoin(gHero->getCoin() - items[i].getPrice()); // decrease coin of hero
							(*heroBackpack)[index].setItem(items[i].getID());
							(*heroBackpack)[index].setAmount(1);
							items[i].setAmount(items[i].getAmount() - 1); // decrease of the items amount

							// if the amount of items is 0, set the slot is empty
							if (items[i].getAmount() == 0)
							{
								items[i].setItem(0);
								items[i].setPrice(0);
							}

							return true;
						}
					}				
				}
			}
		}

		return false;
	}

protected:

	// Intent: find the first available space
	// Pre: two-dimensional vector
	// Post: the index of the first available space in the vector, or -1 if no space is available
	int getSpace(vector<vector<int>>* arr)
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
	int findItem(vector<vector<int>>* arr, int id)
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
};
