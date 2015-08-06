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

class Train : public GameObject {

	private:

		float dx, dy;

		int pause, think;

	public:

		char name[50];

		int type;

		bool active;

		float x, y;
		
		bool waitAtStart;
		
		int startX, startY, endX, endY;

		int height, width;
		
		Sprite *sprite;

	Train();
	float getDX();
	float getDY();
	bool isReady();
	bool isMoving();
	bool waitsForPlayer();
	void setName(const char *name);
	void set(int startX, int startY, int endX, int endY, int pause, bool fromStart);
	bool openClose();
	void move();
	
	int getPause();

};
