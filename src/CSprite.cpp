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

/**
* Creates a new Sprite object
*/
Sprite::Sprite()
{
	for (int i = 0 ; i < 8 ; i++)
	{
		image[i] = NULL;
		frameLength[i] = 0;
	}

	currentFrame = 0;
	currentTime = 0;
	maxFrames = 0;

	next = NULL;

	randomFrames = false;
}

/**
* Sets the specified train with the specified image with
* a specified time
* @param i The index of the frame to set (0 - 7)
* @param shape The image to be used
* @param time How long this frame will last
*/
void Sprite::setFrame(int i, SDL_Surface *shape, int time)
{
	image[i] = shape;
	frameLength[i] = time;

	currentFrame = 0;
	currentTime = frameLength[0];

	if (i > maxFrames)
		maxFrames = i;
}

void Sprite::animate()
{
	currentTime--;

	if (currentTime > 0)
		return;

	currentFrame++;
	
	if (currentFrame == 8)
		currentFrame = 0;

	if (frameLength[currentFrame] == 0)
		currentFrame = 0;

	currentTime = frameLength[currentFrame];
}

void Sprite::getNextFrame(unsigned char *frame, unsigned char *time)
{
	if (*frame >= 8)
		*frame = 0;

	if (frameLength[*frame] == 0)
		*frame = 0;

	*time = frameLength[*frame];
}

SDL_Surface *Sprite::getCurrentFrame()
{
	return image[currentFrame];
}

void Sprite::free()
{
	for (int i = 0 ; i < 8 ; i++)
	{
		if (image[i] != NULL)
		{
			SDL_FreeSurface(image[i]);
			image[i] = NULL;
			frameLength[i] = 0;
		}
	}
	
	currentFrame = 0;
	currentTime = 0;
	maxFrames = 0;
}
