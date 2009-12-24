#include "headers.h"

Config::Config()
{
	sdlJoystick = NULL;
	
	for (int i = 0 ; i < CONTROL::MAX ; i++)
	{
		command[i] = 0;
	}
}

Config::~Config()
{
}

void Config::populate()
{
	for (int i = 0 ; i < CONTROL::MAX ; i++)
	{
		command[i] = (engine->keyState[keyboard.control[i]]); 
		
		if (joystick.control[i] >= 0)
		{
			command[i] = (command[i] || (engine->joystickState[joystick.control[i]]));
		}
	}
	
	if (engine->joyX < -joystick.sensitivity) command[CONTROL::LEFT] = 1;
	if (engine->joyX > joystick.sensitivity)  command[CONTROL::RIGHT] = 1;
	if (engine->joyY < -joystick.sensitivity) command[CONTROL::UP] = 1;
	if (engine->joyY > joystick.sensitivity)  command[CONTROL::DOWN] = 1;
}

void Config::populate(int *data)
{
	for (int i = 0 ; i < CONTROL::MAX ; i++)
	{
		command[i] = data[i];
	}
	
	command[CONTROL::MAP] = 0;
	command[CONTROL::PAUSE] = 0;
}

bool Config::isControl(CONTROL::TYPE type)
{
	return command[type];
}

void Config::resetControl(CONTROL::TYPE type)
{
	engine->keyState[keyboard.control[type]] = 0;
	
	command[type] = 0;
	
	if (joystick.control[type] >= 0)
	{
		engine->joystickState[joystick.control[type]] = 0;
	}
	
	if (type == CONTROL::LEFT) engine->joyX = 0;
	if (type == CONTROL::RIGHT) engine->joyX = 0;
	if (type == CONTROL::UP) engine->joyY = 0;
	if (type == CONTROL::DOWN) engine->joyY = 0;
}

void Config::doPause()
{
	if (!engine->paused)
	{
		if (engine->keyState[keyboard.control[CONTROL::PAUSE]])
		{
			engine->paused = true;
			engine->keyState[keyboard.control[CONTROL::PAUSE]] = 0;
		}
	}
	else
	{
		if ((engine->keyState[keyboard.control[CONTROL::PAUSE]]) || (engine->keyState[SDLK_ESCAPE]))
		{
			engine->paused = false;
			engine->keyState[keyboard.control[CONTROL::PAUSE]] = 0;
			engine->keyState[SDLK_ESCAPE] = 0;
		}
	}
}

bool Config::loadJoystickConfig()
{
	char filename[PATH_MAX];
	sprintf(filename, "%sjoystick.cfg", engine->userHomeDirectory);
	
	debug(("Loading joystick config from %s\n", filename));
	
	FILE *fp = fopen(filename, "rb");
	
	if (!fp)
	{
		return false;
	}
		
	if (fread(&joystick, sizeof(Joystick), 1, fp) != 1)
	{
		fclose(fp);
		return false;
	}
		
	fclose(fp);
	
	return true;
}

bool Config::saveJoystickConfig()
{
	char filename[PATH_MAX];
	sprintf(filename, "%sjoystick.cfg", engine->userHomeDirectory);
	
	debug(("Saving joystick config to %s\n", filename));
	
	FILE *fp = fopen(filename, "wb");
	
	if (!fp)
	{
		debug(("WARNING: Couldn't save joystick config\n"));
		return false;
	}
		
	fwrite(&joystick, sizeof(Joystick), 1, fp);
		
	fclose(fp);
	
	return true;
}

bool Config::loadKeyConfig()
{
	char filename[PATH_MAX];
	sprintf(filename, "%skeyboard.cfg", engine->userHomeDirectory);
	
	debug(("Loading keyboard config from %s\n", filename));
	
	FILE *fp = fopen(filename, "rb");
	
	if (!fp)
	{
		return false;
	}
		
	if (fread(&keyboard, sizeof(Keyboard), 1, fp) != 1)
	{
		fclose(fp);
		return false;
	}

	for (int i = 0; i < CONTROL::MAX; i++)
	{
		if (keyboard.control[i] < 0 || keyboard.control[i] >= 350)
		{
			fclose(fp);
			return false;
		}
	}
		
	fclose(fp);
	
	// Keyboard break fix - Feb 09
	keyboard.control[CONTROL::UP] = 0;
	
	return true;
}

bool Config::saveKeyConfig()
{
	char filename[PATH_MAX];
	sprintf(filename, "%skeyboard.cfg", engine->userHomeDirectory);
	
	debug(("Saving keyboard config to %s\n", filename));
	
	FILE *fp = fopen(filename, "wb");
	
	if (!fp)
	{
		return false;
	}
		
	fwrite(&keyboard, sizeof(keyboard), 1, fp);
		
	fclose(fp);
	
	return true;
}

void Config::restoreKeyDefaults()
{
	keyboard.setDefaultKeys();
}
