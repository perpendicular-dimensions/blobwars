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

Sprite::~Sprite()
{
	for (auto &&frame: frames)
		SDL_FreeSurface(frame.image);
}

/**
* Sets the specified train with the specified image with
* a specified time
* @param i The index of the frame to set (0 - 7)
* @param shape The image to be used
* @param time How long this frame will last
*/
void Sprite::addFrame(SDL_Surface *shape, int time)
{
	frames.emplace_back(shape, time);

	currentFrame = 0;
	currentTime = frames[0].time;
}

void Sprite::animate()
{
	currentTime--;

	if (currentTime > 0)
		return;

	currentFrame++;

	if (currentFrame >= frames.size())
		currentFrame = 0;

	currentTime = frames[currentFrame].time;
}

void Sprite::getNextFrame(unsigned char *frame, unsigned char *time)
{
	if (*frame >= frames.size())
		*frame = 0;

	*time = frames[*frame].time;
}

SDL_Surface *Sprite::getCurrentFrame() const
{
	return frames[currentFrame].image;
}

SDL_Surface *Sprite::getFrame(unsigned char frame) const
{
	return frames[frame].image;
}
