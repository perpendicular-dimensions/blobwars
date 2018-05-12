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

class Entity
{
public:
	std::string name;

	float x = 0;
	float y = 0;
	float dx = 0;
	float dy = 0;

	int tx = 0;
	int ty = 0;
	int width = 0;
	int height = 0;

	signed char oxygen = 7;
	signed char fuel = 7;

	unsigned char immune = 120;
	unsigned char environment = ENV_AIR;
	unsigned char damage = 0;
	unsigned char face = 0;

	int thinktime = 0;
	int baseThink = 0;
	int health = 0;
	int reload = 0;

	unsigned int id = 0;
	unsigned int value = 0;
	long flags = 0;
	unsigned char currentFrame = 0;
	unsigned char currentTime = 0;

	int deathSound = -1;

	Sprite *sprite[3] = {};

	bool falling = false;

	Weapon *currentWeapon = nullptr;

	int dead = DEAD_ALIVE;

	Entity *owner = this;

	bool referenced = false;

	Entity() {}
	Entity(int id, const std::string &name, int x, int y);
	void setName(const std::string &name);
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
