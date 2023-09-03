/***********************************************************************
 * File: Position.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for Position main program header
***********************************************************************/
#pragma once

class Position
{	
public:
	int x; // x of position
	int y; // y of position

	// Intent:	default constructor
	// Pre:		None
	// Post:	None
	Position()
	{
		x = 0;
		y = 0;
	}

	// Intent:	setting constructor
	// Pre:		x of position, y of position
	// Post:	None
	Position(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};
