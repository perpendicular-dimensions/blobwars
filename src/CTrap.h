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

class Trap : public GameObject {

	public:

		char name[50];
		bool active;
		unsigned char type, damage;
		unsigned char thinktime, waitTime[2];

		signed char currentAction;

		float x, y, dx, dy, speed;
		int width, height;
		int startX, startY, endX, endY;

		Sprite *sprite;

	Trap();
	void setName(const char *name);
	void setTrapType(int trapType);
	void setSprite(Sprite *sprite);
	void setSpeed(int speed);
	void setPosition(int x, int y);
	void setDestinations(int startX, int startY, int endX, int endY);
	void setWaitTimes(int wait1, int wait2);
	void setDamage(int damage);
	bool performSpikeAction();
	bool performSwingAction();
	bool performBarrierAction();
	bool performFlameAction();
	bool think();

};
