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
	fd = nullptr;
	
	numberOfFiles = 0;
	listPos = 0;
	currentFile = nullptr;
}

Pak::~Pak()
{
	delete[] fd;
}

void Pak::showPakErrorAndExit()
{
	printf("\nFatal Error: The Blob Wars PAK file was either not found or was not accessible.\n");
	printf("(If you compiled from source did you forget to run make install?)\n");
	printf("The path to the file was expected to be,\n\n");
	printf(PAKFULLPATH"\n\n");
	printf("Please try running the game again. If problems persist either reinstall the game or check,\n\n");
	printf("http://www.parallelrealities.co.uk/blobWars.php\n\n");
	printf("for updates.\n\n");
	exit(1);
}

void Pak::setPakFile(const std::string &pakFilename)
{
	#if USEPAK
	strlcpy(this->pakFilename, pakFilename, sizeof this->pakFilename);
	
	debug(("Pak : Filename set to %s\n", pakFilename));

	FILE *pak = fopen(pakFilename, "rb");

	if (!pak)
	{
		return showPakErrorAndExit();
	}

	fseek(pak, (-sizeof(uint32_t)) * 2, SEEK_END);
	if (fread(&listPos, sizeof(uint32_t), 1, pak) != 1)
	{
		fclose(pak);
		return showPakErrorAndExit();
	}
	if (fread(&numberOfFiles, sizeof(uint32_t), 1, pak) != 1)
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

bool Pak::unpack(const std::string &filename, std::vector<char> *buffer)
{
	debug(("Pak : Unpacking %s...\n", filename));
	
	currentFile = nullptr;
	
	for (unsigned int i = 0 ; i < numberOfFiles ; i++)
	{
		if (filename == fd[i].filename)
		{
			currentFile = &fd[i];
			break;
		}
	}
	
	if (currentFile == nullptr)
	{
		return false;
	}
	
	std::ifstream pak(pakFilename);

	pak.seekg(currentFile->location);

	std::vector<char> input;
	input.reserve(currentFile->cSize * 1.01 + 12);
	buffer->reserve(currentFile->fSize + 1);

	pak.read(input.data(), currentFile->cSize);

	if (pak.bad())
	{
		showPakErrorAndExit();
	}
	
	uLongf fSize = (uLongf)currentFile->fSize;
	
	uncompress((Bytef *)buffer->data(), &fSize, (Bytef *)input.data(), currentFile->cSize);
	buffer->at(currentFile->fSize) = 0;

	debug(("Pak : Unpack %s...Done\n", filename));

	return true;
}

bool Pak::fileExists(const std::string &filename)
{
	for (unsigned int i = 0 ; i < numberOfFiles ; i++)
	{
		if (fd[i].filename == filename)
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
