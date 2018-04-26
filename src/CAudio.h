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

class Audio {

	private:
	
		std::string levelMusicName;

		Engine *engine;

		Mix_Chunk *sound[MAX_SOUNDS];
		Mix_Chunk *quickSound;

		int musicVolume;
		int soundVolume;

		Mix_Music *music;

	public:
		std::string songtitle;
		std::string songalbum;
		std::string songartist;
		int songlicense;

		int output;
		bool useSound, useMusic;

	Audio();
	void setSoundVolume(int soundVolume);
	void setMusicVolume(int musicVolume);
	void registerEngine(Engine *engine);
	bool loadSound(int i, const std::string &filename);
	bool loadMusic(const std::string &filename);
	void playSoundRelative(int snd, int channel, float x);
	void playSound(int snd, int channel, float x);
	void playSound(int snd, int channel);
	void playMusic();
	void playMusicOnce();
	int playMenuSound(int sound);
	bool loadGameOverMusic();
	bool reloadLevelMusic();
	void playAmbiance();
	void stopAmbiance();
	void pause();
	void resume();
	void stopMusic();
	void fadeMusic();
	void free();
	void destroy();

};
