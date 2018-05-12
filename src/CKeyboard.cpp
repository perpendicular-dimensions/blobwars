/*
Copyright (C) 2004-2011 Parallel Realities
Copyright (C) 2011-2015 Perpendicular Dimensions

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
	control[CONTROL::LEFT] = SDL_SCANCODE_LEFT;
	control[CONTROL::RIGHT] = SDL_SCANCODE_RIGHT;
	control[CONTROL::DOWN] = SDL_SCANCODE_DOWN;
	control[CONTROL::JUMP] = SDL_SCANCODE_UP;
	control[CONTROL::UP] = 0;
	control[CONTROL::FIRE] = SDL_SCANCODE_LCTRL;
	control[CONTROL::JETPACK] = SDL_SCANCODE_SPACE;
	control[CONTROL::PAUSE] = SDL_SCANCODE_P;
	control[CONTROL::MAP] = SDL_SCANCODE_TAB;
}

std::string Keyboard::translateKey(int scancode)
{
	if (scancode <= 0)
	{
		return "...";
	}

	SDL_Keycode key = SDL_GetKeyFromScancode((SDL_Scancode)scancode);

	std::string keyName = _(SDL_GetKeyName(key));

	/*
	This is not really necessary, but it just makes
	everything look neater. It runs through the string
	and uppercase the first letter and any letter after
	a space.
	*/

	bool uppercase = true;

	for (auto &&c: keyName)
	{
		if ((c >= SDL_SCANCODE_A) && (c <= SDL_SCANCODE_Z))
		{
			if (uppercase)
			{
				c -= 32;
				uppercase = false;
			}
		}
		else if (c == SDL_SCANCODE_SPACE)
		{
			uppercase = true;
		}
	}

	return keyName;
}
