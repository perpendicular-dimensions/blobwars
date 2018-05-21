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

void Audio::setSoundVolume(int soundVolume)
{
	this->soundVolume = soundVolume;
	if (engine->useAudio)
		Mix_Volume(-1, soundVolume);
}

void Audio::setMusicVolume(int musicVolume)
{
	this->musicVolume = musicVolume;
	if (engine->useAudio)
	{
		Mix_VolumeMusic(musicVolume);
	}
}

void Audio::registerEngine(Engine *engine)
{
	this->engine = engine;
}

bool Audio::loadSound(int i, const std::string &filename)
{
	if (!engine->useAudio)
	{
		return true;
	}

	if (i >= MAX_SOUNDS)
	{
		printf("ERROR: SOUND INDEX IS HIGHER THAN MAXIMUM ALLOWED %d >= %d\n", i, MAX_SOUNDS);
		exit(1);
	}

	if (sound[i] != nullptr)
	{
		Mix_FreeChunk(sound[i]);
		sound[i] = nullptr;
	}

#if USEPAK
	engine->unpack(filename, PAK_SOUND);
	sound[i] = Mix_LoadWAV_RW(engine->sdlrw, 1);
#else
	sound[i] = Mix_LoadWAV(filename.c_str());
#endif

	if (!sound[i])
	{
		debug(("WARNING - Failed to load %s\n", filename));
		return false;
	}

	return true;
}

bool Audio::loadMusic(const std::string &filename)
{
	std::string tempPath = fmt::format("{}music.mod", engine->userHomeDirectory);

	if (!engine->useAudio)
	{
		return true;
	}

	remove(tempPath.c_str());

	if (music != nullptr)
	{
		Mix_HaltMusic();
		SDL_Delay(5);
		Mix_FreeMusic(music);
		music = nullptr;
	}

#if USEPAK
	engine->unpack(filename, PAK_MUSIC);
	music = Mix_LoadMUS(tempPath.c_str());
#else
	music = Mix_LoadMUS(filename.c_str());
#endif

	songtitle[0] = 0;
	songalbum[0] = 0;
	songartist[0] = 0;
	songlicense = -1;

	if (!music)
	{
		debug(("WARNING - Failed to load %s\n", filename));
		return false;
	}

#if USEPAK
	tempPath = fmt::format("{}music.tags", engine->userHomeDirectory);
	remove(tempPath.c_str());

	std::string tagFileName = fmt::format("{}.tags", filename);
	engine->unpack(tagfilename, PAK_TAGS);
#else
	tempPath = fmt::format("{}.tags", filename);
#endif

	std::ifstream file(tempPath);
	std::string line;

	while (std::getline(file, line))
	{
		if (!line.compare(0, 6, "title="))
			songtitle = line.substr(6);
		else if (!line.compare(0, 6, "album="))
			songalbum = line.substr(6);
		else if (!line.compare(0, 7, "artist="))
			songartist = line.substr(7);
		else if (!line.compare(0, 8, "license="))
		{
			if (!line.compare(8, 6, "CC-BY "))
				songlicense = 0;
			else if (!line.compare(8, 9, "CC-BY-SA "))
				songlicense = 1;
		}
	}

	levelMusicName = filename;

	return true;
}

void Audio::playSoundRelative(int snd, int channel, float x)
{
	if ((!engine->useAudio) || (soundVolume == 0))
		return;

	if (!output)
	{
		return;
	}

	int angle = atanf(x / 480) * 180 / M_PI;
	int attenuation = fabsf(x) / 40;

	if (angle < 0)
		angle += 360;

	if (attenuation > 255)
		attenuation = 255;

	Mix_Volume(channel, soundVolume);
	Mix_PlayChannel(channel, sound[snd], 0);
	Mix_SetPosition(channel, angle, attenuation);
}

void Audio::playSound(int snd, int channel, float x)
{
	x -= (engine->playerPosX + 320);
	playSoundRelative(snd, channel, x);
}

void Audio::playSound(int snd, int channel)
{
	playSoundRelative(snd, channel, 0);
}

void Audio::playMusic()
{
	if (!engine->useAudio)
		return;

	if (!output)
	{
		return;
	}

	Mix_PlayMusic(music, -1);

	Mix_VolumeMusic(musicVolume);
}

void Audio::playMusicOnce()
{
	if (!engine->useAudio)
		return;

	if (!output)
	{
		return;
	}

	Mix_PlayMusic(music, 0);

	Mix_VolumeMusic(musicVolume);
}

bool Audio::loadGameOverMusic()
{
	if (!engine->useAudio)
	{
		return true;
	}

	std::string tempPath = engine->userHomeDirectory + "music.mod";

	remove(tempPath.c_str());
	SDL_Delay(250); // wait a bit, just to be sure!

	if (music != nullptr)
	{
		Mix_HaltMusic();
		SDL_Delay(5);
		Mix_FreeMusic(music);
		music = nullptr;
	}

#if USEPAK
	engine->unpack("music/gameover", PAK_MUSIC);
	music = Mix_LoadMUS(tempPath.c_str());
#else
	music = Mix_LoadMUS("music/gameover");
#endif

	if (!music)
	{
		return false;
	}

	return true;
}

bool Audio::reloadLevelMusic()
{
	// remove the Game Over music first...

	if (music != nullptr)
	{
		Mix_HaltMusic();
		SDL_Delay(5);
		Mix_FreeMusic(music);
		music = nullptr;
	}

	return loadMusic(levelMusicName);
}

void Audio::playAmbience()
{
	if ((!engine->useAudio) || (soundVolume == 0))
	{
		return;
	}

	if (!output)
	{
		return;
	}

	Mix_PlayChannel(CH_AMBIENCE, sound[SND_AMBIENCE], -1);
}

void Audio::stopAmbience()
{
	if ((!engine->useAudio) || (soundVolume == 0))
		return;

	Mix_HaltChannel(CH_AMBIENCE);
}

int Audio::playMenuSound(int sound)
{
	if ((!engine->useAudio) || (soundVolume == 0))
		return sound;

	if ((sound == 0) || (sound == 3))
		return sound;

	if (sound == 1)
		playSound(SND_HIGHLIGHT, CH_ANY);

	if (sound == 2)
		playSound(SND_SELECT, CH_ANY);

	return sound;
}

void Audio::pause()
{
	if (!engine->useAudio)
		return;

	for (int i = 0; i < 8; i++)
		Mix_Pause(i);

	Mix_PauseMusic();
}

void Audio::resume()
{
	if (!engine->useAudio)
		return;

	if (!output)
	{
		return;
	}

	for (int i = 0; i < 8; i++)
		Mix_Resume(i);

	Mix_ResumeMusic();
}

void Audio::stopMusic()
{
	if (!engine->useAudio)
		return;

	Mix_HaltMusic();
}

void Audio::fadeMusic()
{
	if (!engine->useAudio)
		return;

	Mix_FadeOutMusic(3500);
}

void Audio::free()
{
	for (int i = 0; i < MAX_SOUNDS - 3; i++)
	{
		if (sound[i] != nullptr)
		{
			Mix_FreeChunk(sound[i]);
			sound[i] = nullptr;
		}
	}

	if (music != nullptr)
	{
		Mix_HaltMusic();
		SDL_Delay(5);
		Mix_FreeMusic(music);
	}

	music = nullptr;

	if (quickSound != nullptr)
		Mix_FreeChunk(quickSound);

	quickSound = nullptr;
}

Audio::~Audio()
{
	free();

	for (int i = MAX_SOUNDS - 3; i < MAX_SOUNDS; i++)
		if (sound[i] != nullptr)
			Mix_FreeChunk(sound[i]);
}
