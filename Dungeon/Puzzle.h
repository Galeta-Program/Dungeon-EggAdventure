/***********************************************************************
 * File: DungeonGUI.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for Puzzle main program header
***********************************************************************/
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include "Hero.h"

using namespace sf;
using namespace std;

class Puzzle
{
private:
	RenderWindow* mainWindow;  // the window of game
	Hero* gHero; // hero
	Texture backgroundTexture; // sprite of puzzle background
	Sprite backgroundSprite; // texture of puzzle background
	vector<Sprite> puzzleBlockSprite; // sprites of puzzle blocks
	vector<Texture> puzzleBlockTexture;  // textures of puzzle blocks
	vector<Text> puzzleBlockText; // texts of puzzle blocks
	vector<vector<int>> puzzleBoard; // numberso of puzzle
	Font font; // font
	bool isComplete; // the puzzle whether complete
	bool isPushable = true; // puzzle block whether movable
	clock_t pushStartTime = clock(); // timer of puzzle block moveable

public:
	// Intent:	setting constructor
	// Pre:		the window of game, hero 
	// Post:	None
	Puzzle(RenderWindow* window, Hero *hero)
	{
		// set datas and image of puzzle
		mainWindow = window;
		gHero = hero;
		isComplete = false;
		font.loadFromFile("resources/sansation.ttf");
		backgroundTexture.loadFromFile("./image/puzzleBackground2.png");
		backgroundSprite.setTexture(backgroundTexture);
		backgroundSprite.setPosition(0, 0);

		// set numbers of puzzle
		puzzleBoard.assign(3, vector<int>(3, 0));
		setPuzzle();

		// create and set image and text of puzzle blcok
		puzzleBlockText.assign(8, Text("", font, 50));
		puzzleBlockSprite.assign(8, Sprite());
		puzzleBlockTexture.assign(8, Texture());

		for (int i = 0, index = 0; i < 3 && index < 8; i++)
		{
			for (int j = 0; j < 3 && index < 8; j++, index++)
			{
				if (!(i == 2 && j == 2))
				{
					puzzleBlockText[index].setString(to_string(puzzleBoard[i][j]));;
					puzzleBlockText[index].setFillColor(Color::Black);
					puzzleBlockTexture[index].loadFromFile("./image/puzzleBlock2.png");
					puzzleBlockSprite[index].setTexture(puzzleBlockTexture[index]);
					puzzleBlockSprite[index].setPosition(10 + j * puzzleBlockTexture[index].getSize().x, 10 + i * puzzleBlockTexture[index].getSize().y);
					puzzleBlockText[index].setPosition(puzzleBlockSprite[index].getPosition().x + puzzleBlockTexture[index].getSize().x / 2 - 12, puzzleBlockSprite[index].getPosition().y + 15);
				}				
			}
		}
	}

	// Intent:	deal with event of play puzzle
	// Pre:		event of game
	// Post:	None
	void playEvent(Event* event)
	{
		for (int i = 0; i < puzzleBlockSprite.size() && !isComplete; i++)
		{
			// if hero is overly on block and the block is moveable, pressed right mouse
			if (gHero->getSprite().getGlobalBounds().intersects(puzzleBlockSprite[i].getGlobalBounds()) && 
				puzzleBlockSprite[i].getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y) && Mouse::isButtonPressed(Mouse::Right) && isPushable)
			{		
				// get x, y of index on board from puzzle block position
				int x = (puzzleBlockSprite[i].getPosition().x - 10 - backgroundSprite.getPosition().x) / puzzleBlockTexture[i].getSize().x;
				int y = (puzzleBlockSprite[i].getPosition().y - 10 - backgroundSprite.getPosition().y) / puzzleBlockTexture[i].getSize().y;
				
				for (int dy = -1; dy <= 1; dy++)
				{
					for (int dx = -1; dx <= 1; dx++)
					{
						// if up, down, right, left of this position has a empty slot(number 0), move to the position
						if (abs(dx) != abs(dy) && x + dx >= 0 && x + dx < 3 && y + dy >= 0 && y + dy < 3 && puzzleBoard[y + dy][x + dx] == 0)
						{
							// switch both number
							puzzleBoard[y + dy][x + dx] = puzzleBoard[y][x];
							puzzleBoard[y][x] = 0;

							// move block
							puzzleBlockSprite[i].move(dx * 100, dy * 100);
							puzzleBlockText[i].move(dx * 100, dy * 100);
							dy = dx = 10;
							isPushable = false;
							pushStartTime = clock();
							break;
						}
					}
				}

				checkState(); // check the puzzle whether completed
			}
			else if (event->type == Event::MouseButtonReleased && event->mouseButton.button == Mouse::Right) isPushable = true;
		}
	}

	// Intent:	set position of puzzle 
	// Pre:		x of position, y of position
	// Post:	None
	void setPosition(int x, int y)
	{
		// reset position of puzzle all objects
		backgroundSprite.setPosition(x, y);

		for (int i = 0, index = 0; i < 3 && index < 8; i++)
		{
			for (int j = 0; j < 3 && index < 8; j++, index++)
			{
				puzzleBlockSprite[index].setPosition(x + 10 + j * 100, y + 10 + i * 100);
				puzzleBlockText[index].setPosition(puzzleBlockSprite[index].getPosition().x + 100 / 2 - 12, puzzleBlockSprite[index].getPosition().y + 15);
			}
		}
	}

	// Intent:	move puzzle
	// Pre:		delta x, delta y
	// Post:	None
	void move(int deltaX, int deltaY)
	{
		// move all objects of puzzle
		backgroundSprite.move(deltaX, deltaY);

		for (int i = 0, index = 0; i < 3 && index < 8; i++)
		{
			for (int j = 0; j < 3 && index < 8; j++, index++)
			{
				puzzleBlockSprite[index].move(deltaX, deltaY);
				puzzleBlockText[index].move(deltaX, deltaY);
			}
		}
	}

	// Intent:	check the puzzle whether completed
	// Pre:		None
	// Post:	None
	void checkState()
	{
		// make sure the numbers are the same
		for (int i = 0, num = 1; i < 3; i++)
		{
			for (int j = 0; j < 3 && num <= 8; j++, num++)
			{
				if (puzzleBoard[i][j] != num) return;
			}
		}

		isComplete = true;
	}

	// Intent:	get the puzzle game whether game over
	// Pre:		None
	// Post:	the puzzle whether completed
	bool isGameOver()
	{
		return isComplete;
	}

	// Intent:	draw puzzle on the window of game
	// Pre:		None
	// Post:	None
	void drawInWindow()
	{
		// if puzzle is overly on screen
		if (FloatRect(0, 0, mainWindow->getSize().x, mainWindow->getSize().y).intersects(backgroundSprite.getGlobalBounds()))
		{
			// draw all object of puzzle to window
			mainWindow->draw(backgroundSprite);

			for (int i = 0; i < puzzleBlockSprite.size(); i++)
			{
				mainWindow->draw(puzzleBlockSprite[i]);
				mainWindow->draw(puzzleBlockText[i]);
			}
		}	
	}

	// Intent:	set the numbers of puzzle randomly
	// Pre:		None
	// Post:	None
	void setPuzzle()
	{
		vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8 }; // create a vector of numbers 1 to 8
		random_device rd; // random device for generating random numbers
		mt19937 generator(rd()); // mersenne Twister random number generator

		do
		{
			shuffle(numbers.begin(), numbers.end(), generator);  // shuffle the numbers randomly
			int inversions = 0;

			for (int i = 0; i < numbers.size(); i++)
			{
				for (int j = i + 1; j < numbers.size(); j++)
				{
					if (numbers[i] > numbers[j])  // count the number of inversions
					{
						inversions++;
					}
				}
			}

			if (inversions % 2 == 0)  // check if the number of inversions is even
				break;  // if even, break the loop

		} while (true);

		for (int i = 0, index = 0; i < 3; i++)  // assign the numbers to the puzzle board
		{
			for (int j = 0; j < 3 && index < 8; j++, index++)
			{
				puzzleBoard[i][j] = numbers[index];
			}
		}
	}
};
