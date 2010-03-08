/*
Copyright (C) 2004 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "headers.h"

Keyboard::Keyboard()
{
	setDefaultKeys();
}

void Keyboard::setDefaultKeys()
{
	control[CONTROL::LEFT] = SDLK_LEFT;
	control[CONTROL::RIGHT] = SDLK_RIGHT;
	control[CONTROL::DOWN] = SDLK_DOWN;
	control[CONTROL::JUMP] = SDLK_UP;
	control[CONTROL::UP] = 0;
	control[CONTROL::FIRE] = SDLK_LCTRL;
	control[CONTROL::JETPACK] = SDLK_SPACE;
	control[CONTROL::PAUSE] = SDLK_p;
	control[CONTROL::MAP] = SDLK_TAB;
}

const char *Keyboard::translateKey(int key)
{
	static char keyName[50];
	strcpy(keyName, "");
	keyName[0] = '\0';
	
	if (key <= 0)
	{
		return "...";
	}
	
	strcpy(keyName, _(SDL_GetKeyName((SDLKey)key)));
	
	/*
	This is not really neccessary, but it just makes
	everything look neater. It runs through the string
	and uppercase the first letter and any letter after
	a space.
	*/
	
	bool uppercase = true;
	char *c = keyName;
	
	while (*c != '\0')
	{
		if ((*c >= SDLK_a) && (*c <= SDLK_z))
		{
			if (uppercase)
			{
				*c -= 32;
				uppercase = false;
			}
		}
		else if (*c == SDLK_SPACE)
		{
			uppercase = true;
		}
		
		c++;
	}
	
	return keyName;
}

