#include "headers.h"
#include <time.h>

ReplayData::ReplayData()
{
	endOfReplay = false;

	fast = false;

	header.randomSeed = time(nullptr);
	header.version = VERSION;
	header.release = RELEASE;
}

ReplayData::~ReplayData()
{
	if (replayMode == REPLAY_MODE::RECORD)
	{
		save();
		file.seekp(0);
		file.write((char *)&header, sizeof(ReplayDataHeader));
		file.flush();
		if (file.bad())
		{
			fmt::print("Error saving replay data: {}\n", strerror(errno));
			exit(1);
		}
	}
}

void ReplayData::printReplayInformation()
{
	tm *timeinfo = localtime(&header.randomSeed);
	fmt::print("Recorded on : {}", asctime(timeinfo));
	fmt::print("Map         : {}\n", header.map);
	fmt::print("Score       : {}\n", header.score);
	fmt::print("Time        : {}\n", Math::formatTime(header.time));
}

void ReplayData::swapHeaderEndians()
{
	header.randomSeed = SDL_SwapLE32(header.randomSeed);
	header.version = SDL_SwapLE32(header.version);
	header.release = SDL_SwapLE32(header.release);
	header.skill = SDL_SwapLE32(header.skill);
	header.score = SDL_SwapLE32(header.score);
}

void ReplayData::setMode(REPLAY_MODE::TYPE replayMode)
{
	this->replayMode = replayMode;

	if (replayMode == REPLAY_MODE::PLAYBACK)
	{
		file = std::fstream(filename, std::ios_base::in);

		file.read((char *)&header, sizeof header);

		if (file.bad())
		{
			fmt::print("ERROR: Replay file '{}' could not be loaded.\n", filename);
			this->replayMode = REPLAY_MODE::NONE;
			return;
		}

		swapHeaderEndians();

		printf("\n==== REPLAY HEADER DATA ====\n");
		printReplayInformation();
		if ((header.version != VERSION) && (header.release != RELEASE))
		{
			fmt::print("\nWARNING: Replay is from a different version ({} {}) and may not play back correctly\n", header.version, header.release);
		}
		printf("Press F5 to toggle Fast Playback\n");
		load();
	}
	else if (replayMode == REPLAY_MODE::RECORD)
	{
		file = std::fstream(filename, std::ios_base::out);

		swapHeaderEndians();

		file.write((char *)&header, sizeof header);

		if (file.bad())
		{
			fmt::print("Error writing replay data header: {}\n", strerror(errno));
			this->replayMode = REPLAY_MODE::NONE;
			return;
		}

		reset();
	}
}

void ReplayData::reset()
{
	index = 0;

	for (int i = 0; i < DATA_LENGTH; i++)
	{
		for (int j = 0; j < CONTROL::MAX; j++)
		{
			data[i][j] = -1;
		}
	}

	for (int i = 0; i < CONTROL::MAX; i++)
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
		for (int i = 0; i < CONTROL::MAX; i++)
		{
			c[i] = 0;
		}

		return;
	}

	for (int i = 0; i < CONTROL::MAX; i++)
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

	for (int i = 0; i < CONTROL::MAX; i++)
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
			for (int i = 0; i < CONTROL::MAX; i++)
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

	file.read((char *)data, sizeof data);

	if (file.bad())
	{
		printf("Error reading replay data\n");
		exit(1);
	}

	debug(("ReplayData::load() - Done\n"));
}

void ReplayData::save()
{
	debug(("ReplayData::save()\n"));

	file.write((char *)data, sizeof data);

	if (file.bad())
	{
		printf("Error saving replay data\n");
		exit(1);
	}

	debug(("ReplayData::save() - Done\n"));

	reset();
}
