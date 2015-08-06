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

class Math {

	private:

		Math(){}

	public:
	
		static long pSeed;

	static void limitChar(signed char *in, int low, int high);
	static void limitChar(unsigned char *in, int low, int high);
	static void limitInt(int *in, int low, int high);
	static void limitFloat(float *in, float low, float high);
	static void wrapChar(signed char *in, signed char low, signed char high);
	static void wrapInt(int *in, int low, int high);
	static void wrapFloat(float *in, float low, float high);
	static long prand();
	static long rrand(int min, int max);
	static bool boolFromWord(const char *word);
	static void addBit(long *currentBits, long newBits);
	static void removeBit(long *currentBits, long oldBits);
	static void calculateSlope(float x, float y, float x2, float y2, float *dx, float *dy);
	static char *formatTime(int seconds);
};
