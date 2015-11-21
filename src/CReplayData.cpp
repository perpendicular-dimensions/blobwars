#include "headers.h"
#include <time.h>

ReplayData::ReplayData()
{
	fp = NULL;

	endOfReplay = false;
	
	fast = false;
	
	header.randomSeed = time(NULL);
	header.version = VERSION;
	header.release = RELEASE;
	
	filename[0] = 0;
}

ReplayData::~ReplayData()
{
	if (replayMode == REPLAY_MODE::RECORD)
	{
		save();
		rewind(fp);
		int size = fwrite(&header, sizeof(ReplayDataHeader), 1, fp);
		if (size != 1)
		{
			printf("Error saving replay data: %s\n", strerror(errno));
			exit(1);
		}
	}
	
	if (replayMode != REPLAY_MODE::NONE)
	{
		fclose(fp);
	}
}

void ReplayData::printReplayInformation()
{
	tm *timeinfo = localtime(&header.randomSeed);
	printf("Recorded on : %s", asctime(timeinfo));
	printf("Map         : %s\n", header.map);
	printf("Score       : %d\n", header.score);
	printf("Time        : %s\n", Math::formatTime(header.time));
}

void ReplayData::swapHeaderEndians()
{
	#if UNIX
	header.randomSeed 	= SDL_SwapLE32(header.randomSeed);
	header.version 		= SDL_SwapLE32(header.version);
	header.release 		= SDL_SwapLE32(header.release);
	header.skill 		= SDL_SwapLE32(header.skill);
	header.score 		= SDL_SwapLE32(header.score);
	#endif
}

void ReplayData::setMode(REPLAY_MODE::TYPE replayMode)
{
	this->replayMode = replayMode;

	if (replayMode == REPLAY_MODE::PLAYBACK)
	{
		fp = fopen(filename, "rb");
		
		if (!fp)
		{
			printf("ERROR: Replay file '%s' could not be loaded.\n", filename);
			this->replayMode = REPLAY_MODE::NONE;
			return;
		}
		
		if (fread(&header, sizeof(ReplayDataHeader), 1, fp) != 1)
		{
			printf("ERROR: Replay file '%s' is corrupt\n", filename);
			this->replayMode = REPLAY_MODE::NONE;
			fclose(fp);
			return;
		}
		
		swapHeaderEndians();
		
		printf("\n==== REPLAY HEADER DATA ====\n");
		printReplayInformation();
		if ((header.version != VERSION) && (header.release != RELEASE))
		{
			printf("\nWARNING: Replay is from a different version (%f %d) and may not play back correctly\n", header.version, header.release);
		}
		printf("Press F5 to toggle Fast Playback\n");
		load();
	}
	else if (replayMode == REPLAY_MODE::RECORD)
	{
		fp = fopen(filename, "wba");
		
		if (!fp)
		{
			printf("ERROR: Replay file '%s' could not be opened for writing.\n", filename);
			this->replayMode = REPLAY_MODE::NONE;
			return;
		}
		
		swapHeaderEndians();
		
		int size = fwrite(&header, sizeof(ReplayDataHeader), 1, fp);
		if (size != 1)
		{
			printf("Error writing replay data header: %s\n", strerror(errno));
			this->replayMode = REPLAY_MODE::NONE;
			fclose(fp);
			fp = NULL;
			return;
		}
		
		reset();
	}
}

void ReplayData::reset()
{
	index = 0;
		
	for (int i = 0 ; i < DATA_LENGTH ; i++)
	{
		for (int j = 0 ; j < CONTROL::MAX ; j++)
		{
			data[i][j] = -1;
		}
	}
	
	for (int i = 0 ; i < CONTROL::MAX ; i++)
	{
		data[0][i] = 0;
	}
}

void ReplayData::read(int *c)
{
	if (replayMode != REPLAY_MODE::PLAYBACK)
	{
		return;
	}
	
	if (endOfReplay)
	{
		for (int i = 0 ; i < CONTROL::MAX ; i++)
		{
			c[i] = 0;
		}
		
		return;
	}

	for (int i = 0 ; i < CONTROL::MAX ; i++)
	{
		c[i] = data[index][i];
	}
	
	c[CONTROL::MAP] = 0;
	c[CONTROL::PAUSE] = 0;
}

void ReplayData::set(int *c)
{
	if (replayMode != REPLAY_MODE::RECORD)
	{
		return;
	}

	for (int i = 0 ; i < CONTROL::MAX ; i++)
	{
		data[index][i] = c[i];
	}
}

void ReplayData::commit()
{
	if ((endOfReplay) || (replayMode == REPLAY_MODE::NONE))
	{
		return;
	}
	
	index++;
	
	if (index < DATA_LENGTH)
	{
		if (replayMode == REPLAY_MODE::PLAYBACK)
		{
			if (data[index][0] == -1)
			{
				printf("===== END OF REPLAY =====\n");
				endOfReplay = true;
				exit(0);
				return;
			}
		}
		else
		{
			for (int i = 0 ; i < CONTROL::MAX ; i++)
			{
				data[index][i] = 0;
			}
		}
	}
	
	if (index >= DATA_LENGTH)
	{
		if (replayMode == REPLAY_MODE::PLAYBACK)
		{
			load();
		}
		else if (replayMode == REPLAY_MODE::RECORD)
		{
			save();
		}
		
		index = 0;
	}
}

void ReplayData::load()
{
	if (endOfReplay)
	{
		return;
	}

	debug(("ReplayData::load()\n"));
	
	int size = fread(data, 1, DATA_LENGTH * CONTROL::MAX, fp);
	
	if (size != DATA_LENGTH * CONTROL::MAX)
	{
		printf("Error reading replay data\n");
		exit(1);
	}
	
	debug(("ReplayData::load() - Done (%d)\n", size));
}

void ReplayData::save()
{
	debug(("ReplayData::save()\n"));
	
	int size = fwrite(data, 1, DATA_LENGTH * CONTROL::MAX, fp);
	
	if (size != DATA_LENGTH * CONTROL::MAX)
	{
		printf("Error saving replay data\n");
		exit(1);
	}
	
	debug(("ReplayData::save() - Done (%d)\n", size));
	
	reset();
}
