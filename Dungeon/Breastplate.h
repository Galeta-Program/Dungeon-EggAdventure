/***********************************************************************
 * File: Breastplate.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for breastplate main program header
***********************************************************************/
#pragma once

class Breastplate
{
private:
	int hp; // additional hp
	int level = 0; // level of breastplate
	float damageReduct; // hero damage reduction rate
	const float maxDamageReduct = 0.8; // max damage reduction rate

public:
	// Intent:	default constructor
	// Pre:		None
	// Post:	None
	Breastplate()
	{
		// set values of breastplate
		hp = level * 3;
		damageReduct = level * 0.02;

		if (damageReduct >= maxDamageReduct) damageReduct = maxDamageReduct;
	}

	// Intent:	setting constructor
	// Pre:		level of boots
	// Post:	None
	Breastplate(int _level)
	{
		// set values of breastplate
		level = _level;
		hp = level * 3;
		damageReduct = level * 0.02;

		if (damageReduct >= maxDamageReduct) damageReduct = maxDamageReduct;
	}

	// Intent:	get hero damage reduction rate
	// Pre:		None
	// Post:    damage reduction rate
	float getDamageReduct()
	{
		return damageReduct;
	}

	// Intent:	set level of breastplate
	// Pre:		level of breastplate
	// Post:	None
	void setLevel(int _level)
	{
		// set values of breastplate'
		level = _level;
		hp = level * 3;
		damageReduct = level * 0.02;

		if (damageReduct >= maxDamageReduct) damageReduct = maxDamageReduct;
	}

	// Intent:	get additional hp
	// Pre:		None
	// Post:	additional hp
	int getHP()
	{
		return hp;
	}

	// Intent:	get level of breastplate
	// Pre:		None
	// Post:	level of breastplate
	int getLevel()
	{
		return level;
	}
};
