/***********************************************************************
 * File: Item.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for Item main program header
***********************************************************************/
#pragma once
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Item
{
private:
	RenderWindow* mainWindow; // the window of game
	Sprite itemBGSprite; // sprite of item background
	Texture itemBGTexture; // texture of item background
	Sprite coinSprite;  // sprite of coin
	Texture coinTexture; // texture of coin
	Sprite itemSprite; // sprite of item
	Texture itemTexture; // texture of item
	Text priceText; // text of item price
	Text amountText; // text of item amount
	int price; // price of itme
	int amount; // amount of item
	int id; // id of item

public:
	// Intent:	setting constructor
	// Pre:		the window of game, amount of item, price of item, id of item
	// Post:	None
	Item(RenderWindow* window, int _amount, int _price, int _id = 0)
	{
		// set datas and image of item 
		mainWindow = window;
		coinTexture.loadFromFile("./image/coin.png");
		itemBGTexture.loadFromFile("./image/itembox.png");
		itemBGSprite.setTexture(itemBGTexture);
		coinSprite.setTexture(coinTexture);
		itemTexture.loadFromFile("./image/Item/id_" + to_string(_id) + ".png");
		itemSprite.setTexture(itemTexture);
		itemSprite.setScale(1, 1);
		id = _id;
		amount = _amount;
		price = _price;
		priceText.setString(to_string(price));
		priceText.setCharacterSize(30);
		priceText.setFillColor(Color::Black);
		amountText.setString(to_string(amount));
		amountText.setCharacterSize(30);
		amountText.setFillColor(Color::Black);
	}

	// Intent:	set position of item
	// Pre:		x of position, y of position
	// Post:	None
	void setPosition(int x, int y)
	{
		// set position of item all objects
		itemBGSprite.setPosition(x, y);
		itemSprite.setPosition(x, y);
		coinSprite.setPosition(x, y + itemBGSprite.getTexture()->getSize().y);
		amountText.setPosition(x + 5, y);
		priceText.setPosition(x + coinSprite.getTexture()->getSize().x + 5, y + itemBGSprite.getTexture()->getSize().y);
	}

	// Intent:	set item
	// Pre:		id of item
	// Post:	None
	void setItem(int _id)
	{
		// set id of item
		id = _id;

		// set image of item
		itemTexture.loadFromFile("./image/Item/id_" + to_string(id) + ".png");
		itemSprite.setTexture(itemTexture);
	}

	// Intent:	set price of item
	// Pre:		price
	// Post:	None
	void setPrice(int _price)
	{
		price = _price; // set id of item
		priceText.setString(to_string(price)); // set text of item price
	}

	// Intent:	set amount of item
	// Pre:		amount of item
	// Post:	None
	void setAmount(int _amount)
	{
		amount = _amount; // set amount of item
		amountText.setString(to_string(amount)); // set text of item amount
	}

	// Intent:	get overall size of item 
	// Pre:		None
	// Post:	overall size of item 
	Size getSize()
	{
		return Size(itemBGTexture.getSize().x, itemBGTexture.getSize().y + coinTexture.getSize().y);
	}

	// Intent:	get id of item 
	// Pre:		None
	// Post:	id of item 
	int getID()
	{
		return id;
	}

	// Intent:	get amount of item 
	// Pre:		None
	// Post:	amount of item 
	int getAmount()
	{
		return amount;
	}

	// Intent:	get price of item 
	// Pre:		None
	// Post:	price of item 
	int getPrice()
	{
		return price;
	}

	// Intent:	get sprite of item background 
	// Pre:		None
	// Post:	sprite of item background 
	Sprite getSprite()
	{
		return itemBGSprite;
	}

	// Intent:  draw all objects of item to the window of game 
	// Pre:		None
	// Post:	None
	void drawInWindow()
	{
		// set font of text
		Font font;
		font.loadFromFile("resources/sansation.ttf");
		amountText.setFont(font);
		priceText.setFont(font);

		// draw objects on window 
		mainWindow->draw(itemBGSprite);
		mainWindow->draw(itemSprite);

		if (amount > 0) mainWindow->draw(amountText);

		if (price != 0)
		{
			mainWindow->draw(coinSprite);
			mainWindow->draw(priceText);
		}      
	}

};
