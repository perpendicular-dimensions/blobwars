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

class Entity : public GameObject {

	public:

		char name[50];

		float x, y, dx, dy;
		int tx, ty, width, height;

		signed char oxygen, fuel;

		unsigned char immune;
		unsigned char environment, damage;
		unsigned char face;

		signed int thinktime, baseThink, health, reload;

		unsigned int id, value;
		long flags;
		unsigned char currentFrame, currentTime;

		int deathSound;
		
		Sprite *sprite[3];

		bool falling;

		Weapon *currentWeapon;
		
		int dead;

		Entity *owner;

	Entity();
	void setName(const char *name);
	void setSprites(Sprite *sprite1, Sprite *sprite2, Sprite *sprite3);
	void animate();
	SDL_Surface *getFaceImage();
	void place(int x, int y);
	void setVelocity(float dx, float dy);
	void move();
	void setRandomVelocity();
	void applyGravity();
	void checkEnvironment();
	void think();
	
};
