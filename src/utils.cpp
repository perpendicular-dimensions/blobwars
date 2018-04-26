/* Copyright (c) 2018 Perpendicular Dimensions

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

int stoi(std::string_view str) {
	int value = 0;
	for (auto c: str) {
		if (c >= '0' && c <= '9') {
			value *= 10;
			value += c - '0';
		} else {
			break;
		}
	}
	return value;
}
