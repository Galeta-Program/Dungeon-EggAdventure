/***********************************************************************
 * File: Boots.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for boots main program header
***********************************************************************/
#pragma once

class Boots
{
private:
	int hp; // additional hp
	int level = 0; // level of boots
	float speedUpRate; // hero speed up rate
	const float maxSpeedUpRate = 1; // max speed up rate

public:
	// Intent:	default constructor
	// Pre:		None
	// Post:	None
	Boots()
	{
		// set values of boots
		hp = level * 2;
		speedUpRate = level * 0.1;

		if (speedUpRate >= maxSpeedUpRate) speedUpRate = maxSpeedUpRate;
	}

	// Intent:	setting constructor
	// Pre:		level of boots
	// Post:	None
	Boots(int _level)
	{
		// set values of boots
		level = _level;
		hp = level * 2;
		speedUpRate = level * 0.1;

		if (speedUpRate >= maxSpeedUpRate) speedUpRate = maxSpeedUpRate;
	}

	// Intent:	get hero speed up rate
	// Pre:		None
	// Post:	speed up rate
	float getSpeedUpRate()
	{
		return speedUpRate;
	}

	// Intent:	set level of boots
	// Pre:		level of boots
	// Post:	None
	void setLevel(int _level)
	{
		// set values of boots
		level = _level;
		hp = level * 2;
		speedUpRate = level * 0.1;

		if (speedUpRate >= maxSpeedUpRate) speedUpRate = maxSpeedUpRate;
	}

	// Intent:	get additional hp
	// Pre:		None
	// Post:	additional hp
	int getHP()
	{
		return hp;
	}

	// Intent:	get level of boots
	// Pre:		None
	// Post:	level of boots
	int getLevel()
	{
		return level;
	}
};