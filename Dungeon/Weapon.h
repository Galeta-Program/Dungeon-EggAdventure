/***********************************************************************
 * File: Weapon.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for weapon main program header
***********************************************************************/
#pragma once

class Weapon
{
private:
	int id = 0; // id of weapon
	int atk; // atk of weapon
	int level = 0; // level of weapon

public:
	// Intent:	default constructor
	// Pre:		None
	// Post:	None
	Weapon()
	{
		// set values of weapon
		atk = level * 2;
	}

	// Intent:	setting constructor
	// Pre:		id of weapon, level of weapon
	// Post:	None
	Weapon(int _id, int _level = 0)
	{
		// set values of weapon
		id = _id;
		level = _level;
		atk = level * 2;
	}

	// Intent:	set level of weapon
	// Pre:		level of weapon
	// Post:	None
	void setLevel(int _level)
	{
		// set values of weapon
		level = _level;
		atk = level * 2;
	}

	// Intent:	get id of weapon
	// Pre:		None
	// Post:	id of weapon
	void setID(int _id)
	{
		id = _id;
	}

	// Intent:	get atk of weapon
	// Pre:		None
	// Post:	atk of weapon
	int getATK()
	{
		return atk;
	}

	// Intent:	get level of weapon
	// Pre:		None
	// Post:	level of weapon
	int getLevel()
	{
		return level;
	}

	// Intent:	get id of weapon
	// Pre:		None
	// Post:	id of weapon
	int getID()
	{
		return id;
	}
};
