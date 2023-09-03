/***********************************************************************
 * File: Helmet.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for helmet main program header
***********************************************************************/
#pragma once

class Helmet
{
private:
	int hp; // additional hp
	int level = 0; // level of helmet

public:
	// Intent:	default constructor
	// Pre:		None
	// Post:	None
	Helmet()
	{
		// set values of helmet
		hp = level * 2;
	}

	// Intent:	setting constructor
	// Pre:		level of helmet
	// Post:	None
	Helmet(int _level)
	{
		// set values of helmet
		level = _level;
		hp = level * 2;
	}

	// Intent:	set level of helmet
	// Pre:		level of helmet
	// Post:	None
	void setLevel(int _level)
	{
		// set values of helmet
		level = _level;
		hp = level * 2;
	}

	// Intent:	get additional hp
	// Pre:		None
	// Post:	additional hp
	int getHP()
	{
		return hp;
	}

	// Intent:	get level of helmet
	// Pre:		None
	// Post:	level of helmet
	int getLevel()
	{
		return level;
	}
};

