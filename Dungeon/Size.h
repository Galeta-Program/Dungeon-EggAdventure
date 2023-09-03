/***********************************************************************
 * File: Size.h
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for Size main program header
***********************************************************************/
#pragma once

class Size
{
public:
	int width;  // width of size
	int height; // height of size

	// Intent:	default constructor
	// Pre:		None
	// Post:	None
	Size()
	{
		width = 0;
		height = 0;
	}

	// Intent:	setting constructor
	// Pre:		width of size, height of size
	// Post:	None
	Size(int _width, int _height)
	{
		width = _width;
		height = _height;
	}
};
