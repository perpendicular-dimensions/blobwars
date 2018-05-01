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

class Widget {

	public:

		std::string name;
		std::string groupName;
		std::string label;
		std::string options;

		int type = 0;

		int x = 0;
		int y = 0;

		int *value = nullptr; // NB - THIS IS A POINTER!!

		int min = 0;
		int max = 0;

		bool enabled = true;
		bool visible = true;
		bool changed = false;

		SDL_Surface *image = nullptr;

	Widget(const std::string &name, const std::string &groupName, const std::string &label, const std::string &options, int type, int x, int y, int min, int max);
	~Widget();
	void setValue(int *value);
	void redraw();

};
