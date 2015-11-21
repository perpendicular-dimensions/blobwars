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

#include "headers.h"

Pak::Pak()
{
	input = NULL;
	fd = NULL;
	
	numberOfFiles = 0;
	listPos = 0;
	currentFile = NULL;

	pakFilename[0] = 0;
	filename[0] = 0;
}

Pak::~Pak()
{
	delete[] input;
	delete[] fd;
}

void Pak::showPakErrorAndExit()
{
	printf("\nFatal Error: The Blob Wars PAK file was either not found or was not accessable.\n");
	printf("(If you compiled from source did you forget to run make install?)\n");
	printf("The path to the file was expected to be,\n\n");
	printf(PAKFULLPATH"\n\n");
	printf("Please try running the game again. If problems persist either reinstall the game or check,\n\n");
	printf("http://www.parallelrealities.co.uk/blobWars.php\n\n");
	printf("for updates.\n\n");
	exit(1);
}

void Pak::setPakFile(const char *pakFilename)
{
	#if USEPAK
	strlcpy(this->pakFilename, pakFilename, sizeof this->pakFilename);
	
	debug(("Pak : Filename set to %s\n", pakFilename));

	FILE *pak = fopen(pakFilename, "rb");

	if (!pak)
	{
		return showPakErrorAndExit();
	}

	fseek(pak, (-sizeof(Uint32)) * 2, SEEK_END);
	if (fread(&listPos, sizeof(Uint32), 1, pak) != 1)
	{
		fclose(pak);
		return showPakErrorAndExit();
	}
	if (fread(&numberOfFiles, sizeof(Uint32), 1, pak) != 1)
	{
		fclose(pak);
		return showPakErrorAndExit();
	}
	
	debug(("Pak : File list resides at %d\n", (int)listPos));
	debug(("Pak : Number of files are %d\n", (int)numberOfFiles));
	
	fd = new FileData[numberOfFiles];
	
	fseek(pak, listPos, SEEK_SET);
	
	int result = 0;
	
	for (unsigned int i = 0 ; i < numberOfFiles ; i++)
	{
		result = fread(&fd[i], sizeof(FileData), 1, pak);
		
		if (!result)
		{
			fclose(pak);
			return showPakErrorAndExit();
		}
		
		debug(("Read FileData #%d / %d : %s\n", (i + 1), numberOfFiles, fd[i].filename));
	}
	
	fclose(pak);

	#else
	(void)pakFilename;
	#endif
}

bool Pak::unpack(const char *filename, unsigned char **buffer)
{
	debug(("Pak : Unpacking %s...\n", filename));
	
	currentFile = NULL;
	
	for (unsigned int i = 0 ; i < numberOfFiles ; i++)
	{
		if (strcmp(filename, fd[i].filename) == 0)
		{
			currentFile = &fd[i];
			break;
		}
	}
	
	if (currentFile == NULL)
	{
		return false;
	}
	
	FILE *pak = fopen(pakFilename, "rb");
	if (!pak)
	{
		showPakErrorAndExit();
	}
	
	fseek(pak, currentFile->location, SEEK_SET);

	delete[] input;
	input = NULL;
	
	input = new unsigned char[(int)(currentFile->cSize * 1.01) + 12];
	*buffer = new unsigned char[currentFile->fSize + 1];

	if (fread(input, 1, currentFile->cSize, pak) != currentFile->cSize)
	{
		fclose(pak);
		return showPakErrorAndExit(), false;
	}
	
	uLongf fSize = (uLongf)currentFile->fSize;
	
	uncompress(*buffer, &fSize, input, currentFile->cSize);
	(*buffer)[currentFile->fSize] = 0;

	fclose(pak);
	
	delete[] input;
	input = NULL;
	
	debug(("Pak : Unpack %s...Done\n", filename));

	return true;
}

bool Pak::fileExists(const char *filename)
{
	for (unsigned int i = 0 ; i < numberOfFiles ; i++)
	{
		if (strcmp(fd[i].filename, filename) == 0)
		{
			return true;
		}
	}
	
	return false;
}

unsigned int Pak::getUncompressedSize() const
{
	return (unsigned int)currentFile->fSize;
}
