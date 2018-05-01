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

/// A Sprite

class Sprite {

	public:

		std::string name;

		SDL_Surface *image[8] = {};
		unsigned char frameLength[8] = {};

		unsigned char currentFrame = 0;
		unsigned char currentTime = 0;
		unsigned char maxFrames = 0;

		bool randomFrames = false;

	void setFrame(int i, SDL_Surface *shape, int time);
	void animate();
	void getNextFrame(unsigned char *frame, unsigned char *time);
	SDL_Surface *getCurrentFrame() const;
};
