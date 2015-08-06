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

class Pak {

	private:
	
		Uint32 numberOfFiles;
		Uint32 listPos;
		Bytef *input;
		
		FileData *fd;
		FileData *currentFile;

		char pakFilename[1024];

		char filename[60];
		
	void showPakErrorAndExit();

	public:

	Pak();
	~Pak();
	void setPakFile(const char *pakFilename);
	bool unpack(const char *filename, unsigned char **buffer);
	bool fileExists(const char *filename);
	unsigned int getUncompressedSize() const;

};
