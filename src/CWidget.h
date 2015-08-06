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

class Widget : public GameObject {

	public:

		char name[50], groupName[50], label[80], options[100];

		int type;

		int x, y;

		int *value; // NB - THIS IS A POINTER!!

		int min, max;

		bool enabled, visible, changed;

		SDL_Surface *image;

		Widget *previous;

	Widget();
	void setProperties(const char *name, const char *groupName, const char *label, const char *options, int x, int y, int min, int max);
	void setValue(int *value);
	void redraw();

};
