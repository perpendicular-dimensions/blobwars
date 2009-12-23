/*
Copyright (C) 2004 Parallel Realities

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

Engine::Engine()
{
	for (int i = 0 ; i < 350 ; i++)
	{
		keyState[i] = 0;
	}
	
	for (int i = 0 ; i < 32 ; i++)
	{
		joystickState[i] = 0;
	}

	joyX = joyY = 0;

	mouseLeft = mouseRight = 0;
	waitForButton = false;
	waitForKey = false;
	
	allowJoypad = true;

	strcpy(lastKeyPressed, "");

	fullScreen = 0;

	useAudio = 2;
	
	practice = false;
	
	allowQuit = false;

	saveConfig = false;

	highlightedWidget = NULL;

	strcpy(message, "");
	messageTime = -1;

	// Development Stuff
	devNoMonsters = false;

	dataBuffer = NULL;
	binaryBuffer = NULL;
	#ifdef FRAMEWORK_SDL
	char pakPath[PATH_MAX];
	strncpy(pakPath, PAKFULLPATH, sizeof(pakPath));
	if (CFBundleGetMainBundle() != NULL) {
		CFURLRef pakURL = CFBundleCopyResourceURL(CFBundleGetMainBundle(), CFSTR(PAKNAME), NULL, NULL);
		if (pakURL != NULL) {
			CFShow(pakURL);
			CFURLGetFileSystemRepresentation(pakURL, true, (UInt8*)pakPath, sizeof(pakPath));
			CFRelease(pakURL);
		}
	}
	pak.setPakFile(pakPath);
	#else
	pak.setPakFile(PAKFULLPATH);
	#endif

	// Timer
	time1 = time2 = 0;
	timeDifference = 0;

	// Cheats
	memset(lastKeyEvents, ' ', 25);
	cheatHealth = cheatExtras = cheatFuel = cheatLevels = false;
	cheatBlood = cheatInvulnerable = cheatReload = cheatSpeed = cheatSkipLevel = false;
	
	extremeAvailable = 0;
}

void Engine::destroy()
{
	debug(("engine: free widgets\n"));
	deleteWidgets();

	debug(("engine: free databuffer\n"));
	if (dataBuffer != NULL)
		delete[] dataBuffer;

	debug(("engine: free binarybuffer\n"));
	if (binaryBuffer != NULL)
		delete[] binaryBuffer;

	debug(("Clearing Define List...\n"));
	defineList.clear();
}

void Engine::clearCheatVars()
{
	memset(lastKeyEvents, ' ', 25);
	cheatHealth = cheatExtras = cheatFuel = cheatLevels = false;
	cheatBlood = cheatInvulnerable = cheatReload = cheatSpeed = cheatSkipLevel = false;
}

bool Engine::compareLastKeyInputs()
{
	if (strstr(lastKeyEvents, "lockandload"))
	{
		cheats = true;
		return true;
	}

	return false;
}

void Engine::addKeyEvent()
{
	if (strlen(lastKeyPressed) > 1)
	{
		return;
	}

	int index = -1;

	for (int i = 0 ; i < 25 ; i++)
	{
		if (lastKeyEvents[i] == ' ')
		{
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		for (int i = 0 ; i < 25 ; i++)
		{
			lastKeyEvents[i] = lastKeyEvents[i + 1];
		}

		index = 24;
	}

	lastKeyEvents[index] = lastKeyPressed[0];

	compareLastKeyInputs();
}

void Engine::getInput()
{
	SDL_GetMouseState(&mouseX, &mouseY);

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				if (allowQuit)
				{
					exit(0);
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) mouseLeft = 1;
				if (event.button.button == SDL_BUTTON_RIGHT) mouseRight = 1;
				break;

			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) mouseLeft = 0;
				if (event.button.button == SDL_BUTTON_RIGHT) mouseRight = 0;
				break;

			case SDL_KEYDOWN:
				
				if (waitForButton)
				{
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						lastButtonPressed = -1;
						*highlightedWidget->value = abs(*highlightedWidget->value) - 1000;
						highlightedWidget->redraw();
						waitForButton = false;
						allowJoypad = false;
					}
					
					if (event.key.keysym.sym == SDLK_BACKSPACE)
					{
						lastButtonPressed = -2;
						*highlightedWidget->value = -2;
						highlightedWidget->redraw();
						waitForButton = false;
						allowJoypad = false;
					}
					
					return;
				}
				
				if (waitForKey)
				{
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						*highlightedWidget->value = -*highlightedWidget->value;
					}
					else
					{
						*highlightedWidget->value = event.key.keysym.sym;
					}
					
					lastButtonPressed = -1;
					highlightedWidget->redraw();
					waitForButton = false;
					waitForKey = false;
					allowJoypad = false;
					
					return;
				}

				keyState[event.key.keysym.sym] = 1;
				strcpy(lastKeyPressed, SDL_GetKeyName(event.key.keysym.sym));
				addKeyEvent();
				break;

			case SDL_KEYUP:
				keyState[event.key.keysym.sym] = 0;
				break;

			case SDL_JOYAXISMOTION:
				if (event.jaxis.axis == 0)
				{
				  joyX = event.jaxis.value;
				}
				else if (event.jaxis.axis == 1)
				{
				  joyY = event.jaxis.value;
				}
				
				break;

			case SDL_JOYBUTTONDOWN:
			
				if (waitForButton)
				{
					lastButtonPressed = event.jbutton.button;
					*highlightedWidget->value = lastButtonPressed;
					highlightedWidget->redraw();
					waitForButton = false;
					allowJoypad = false;
					return;
				}
				
				joystickState[event.jbutton.button] = 1;
				break;

			case SDL_JOYBUTTONUP:
				joystickState[event.jbutton.button] = 0;
				break;

			default:
				break;
		}
	}
}

int Engine::getMouseX()
{
	return mouseX;
}

int Engine::getMouseY()
{
	return mouseY;
}

void Engine::setMouse(int x, int y)
{
	SDL_WarpMouse(x, y);
}

bool Engine::userAccepts()
{
	if ((keyState[SDLK_SPACE]) || (keyState[SDLK_ESCAPE]) || (keyState[SDLK_LCTRL]) || (keyState[SDLK_RCTRL]) || (keyState[SDLK_RETURN]) || (keyState[SDLK_LCTRL]))
	{
		return true;
	}

	return false;
}

void Engine::flushInput()
{
	while (SDL_PollEvent(&event)){}
}

void Engine::clearInput()
{
	for (int i = 0 ; i < 350 ; i++)
		keyState[i] = 0;

	mouseLeft = mouseRight = 0;
}

void Engine::setUserHome(const char *path)
{
	strcpy(userHomeDirectory, path);
	debug(("User Home = %s\n", path));
}

Pak *Engine::getPak()
{
	return &pak;
}

/*
Searches the pak file for the required data. When
it is found, the data is read into a character buffer.
In the case of music, the data music be written to a temporary directory
since SDL currently provides no means to load music directly from memory
*/
bool Engine::unpack(const char *filename, int fileType)
{
	if (fileType == PAK_DATA)
	{
		if (dataBuffer != NULL)
			delete[] dataBuffer;

		dataBuffer = NULL;
	}
	else
	{
		if (binaryBuffer != NULL)
			delete[] binaryBuffer;

		binaryBuffer = NULL;
	}

	if (fileType != PAK_DATA)
	{
		if (!pak.unpack(filename, &binaryBuffer))
		{
			return false;
		}
	}
	else
	{
		if (!pak.unpack(filename, &dataBuffer))
		{
			return false;
		}
	}

	if ((fileType == PAK_IMG) || (fileType == PAK_SOUND))
	{
		sdlrw = SDL_RWFromMem(binaryBuffer, pak.getUncompressedSize());
		if (!sdlrw)
		{
			printf("Fatal Error: SDL_RWops allocation failed\n");
			exit(1);
		}
	}

	if ((fileType == PAK_MUSIC) || (fileType == PAK_FONT))
	{
		char tempPath[PATH_MAX];
		
		FILE *fp = NULL;

		if (fileType == PAK_MUSIC)
		{
			sprintf(tempPath, "%smusic.mod", userHomeDirectory);
			fp = fopen(tempPath, "wb");
		}

		if (fileType == PAK_FONT)
		{
			sprintf(tempPath, "%sfont.ttf", userHomeDirectory);
			fp = fopen(tempPath, "wb");
		}

		if (!fp)
		{
			return false;
		}

		fwrite(binaryBuffer, 1, pak.getUncompressedSize(), fp);
		fclose(fp);
	}

	debug(("unpack() : Loaded %s (%d)\n", filename, pak.getUncompressedSize()));

	return true;
}

bool Engine::loadData(const char *filename)
{
	if (dataBuffer != NULL)
	{
		delete[] dataBuffer;
		dataBuffer = NULL;
	}
	
	#if USEPAK
		return unpack(filename, PAK_DATA);
	#endif

	FILE *fp;
	fp = fopen(filename, "rb");
	if (fp == NULL)
		return false;

	fseek(fp, 0, SEEK_END);

	int fSize = ftell(fp);

	rewind(fp);

	dataBuffer = new unsigned char[fSize];

	fread(dataBuffer, 1, fSize, fp);

	fclose(fp);

	debug(("loadData() : Loaded %s (%d)\n", filename, fSize));

	return true;
}

void Engine::reportFontFailure()
{
	printf("\nUnable to load font. The game cannot continue without it.\n");
	printf("Please confirm that the game and all required SDL libraries are installed\n");
	printf("The following information may be useful to you,\n\n");
	printf("Expected location of PAK file: %s\n", PAKFULLPATH);
	printf("Location of TMP directory: %s\n", userHomeDirectory);
	printf("\nAlso try checking http://www.parallelrealities.co.uk/blobWars.php for updates\n\n");
	exit(1);
}

void Engine::setPlayerPosition(int x, int y, int limitLeft, int limitRight, int limitUp, int limitDown)
{
	playerPosX = x - OFFSETX;
	playerPosY = y - OFFSETY;

	Math::limitInt(&playerPosX, limitLeft, limitRight);
	Math::limitInt(&playerPosY, limitUp, limitDown);
}

int Engine::getFrameLoop()
{
	return frameLoop;
}

void Engine::doFrameLoop()
{
	Math::wrapChar(&(++frameLoop), 0, 59);
}

void Engine::doTimeDifference()
{
	timeDifference = (time2 - time1) / 10.0;
	time1 = time2;
	time2 = SDL_GetTicks();
}

float Engine::getTimeDifference()
{
	return timeDifference;
}

void Engine::resetTimeDifference()
{
	time1 = time2 = SDL_GetTicks();
}

void Engine::setInfoMessage(const char *message, int priority, int type)
{
	if (priority >= messagePriority)
	{
		strcpy(this->message, message);
		messageTime = 180;
		messagePriority = priority;
		messageType = type;
	}
}

void Engine::deleteWidgets()
{
	Widget *widget;

	for (widget = (Widget*)widgetList.getHead()->next ; widget != NULL ; widget = (Widget*)widget->next)
		widget->redraw();

	widgetList.clear();

	highlightedWidget = NULL;
}

void Engine::addWidget(Widget *widget)
{
	widget->previous = (Widget*)widgetList.getTail();
	widgetList.add(widget);
}

bool Engine::loadWidgets(const char *filename)
{
	deleteWidgets();

	if (!loadData(filename))
		return false;

	char token[50], name[50], groupName[50], label[80], options[100], *line;
	int x, y, min, max;

	int i;

	Widget *widget;

	line = strtok((char*)dataBuffer, "\n");

	while (true)
	{
		sscanf(line, "%s", token);

		if (strcmp(token, "END") == 0)
			break;

		sscanf(line, "%*s %s %s %*c %[^\"] %*c %*c %[^\"] %*c %d %d %d %d", name, groupName, label, options, &x, &y, &min, &max);

		widget = new Widget;

		i = 0;

		while (true)
		{
			if (strcmp(token, widgetName[i]) == 0)
				widget->type = i;

			if (strcmp("-1", widgetName[i]) == 0)
				break;

			i++;
		}

		widget->setProperties(name, groupName, label, options, x, y, min, max);

		addWidget(widget);


		if ((line = strtok(NULL, "\n")) == NULL)
			break;
	}

	highlightedWidget = (Widget*)widgetList.getHead()->next;

	return true;
}

Widget *Engine::getWidgetByName(const char *name)
{
	Widget *widget = (Widget*)widgetList.getHead();

	while (widget->next != NULL)
	{
		widget = (Widget*)widget->next;

		if (strcmp(widget->name, name) == 0)
			return widget;
	}

	debug(("No such widget '%s'\n", name));

	return NULL;
}

void Engine::showWidgetGroup(const char *groupName, bool show)
{
	bool found = false;

	Widget *widget = (Widget*)widgetList.getHead();

	while (widget->next != NULL)
	{
		widget = (Widget*)widget->next;

		if (strcmp(widget->groupName, groupName) == 0)
		{
			widget->visible = show;
			widget->redraw();
			found = true;
		}
	}

	if (!found)
		debug(("Group '%s' does not exist\n", groupName));
}

void Engine::enableWidgetGroup(const char *groupName, bool show)
{
	bool found = false;

	Widget *widget = (Widget*)widgetList.getHead();

	while (widget->next != NULL)
	{
		widget = (Widget*)widget->next;

		if (strcmp(widget->groupName, groupName) == 0)
		{
			widget->enabled = show;
			widget->redraw();
			found = true;
		}
	}

	if (!found)
		debug(("Group '%s' does not exist\n", groupName));
}

void Engine::showWidget(const char *name, bool show)
{
	Widget *widget = getWidgetByName(name);
	if (widget != NULL)
	{
		widget->visible = show;
		widget->redraw();
	}
}

void Engine::enableWidget(const char *name, bool enable)
{
	Widget *widget = getWidgetByName(name);
	if (widget != NULL)
	{
		widget->enabled = enable;
		widget->redraw();
	}
}

void Engine::setWidgetVariable(const char *name, int *variable)
{
	Widget *widget = getWidgetByName(name);
	if (widget != NULL)
		widget->value = variable;
}

bool Engine::widgetChanged(const char *name)
{
	Widget *widget = getWidgetByName(name);
	if (widget != NULL)
		return widget->changed;

	return false;
}

void Engine::highlightWidget(int dir)
{
	highlightedWidget->redraw();

	if (dir == 1)
	{
		while (true)
		{
			if (highlightedWidget->next != NULL)
			{
				highlightedWidget = (Widget*)highlightedWidget->next;
			}
			else
			{
				highlightedWidget = (Widget*)widgetList.getHead()->next;
			}

			if (highlightedWidget->type == 4)
				continue;

			if ((highlightedWidget->enabled) && (highlightedWidget->visible))
				break;
		}
	}

	if (dir == -1)
	{
		while (true)
		{
			if ((highlightedWidget->previous != NULL) && (highlightedWidget->previous != (Widget*)widgetList.getHead()))
			{
				highlightedWidget = highlightedWidget->previous;
			}
			else
			{
				highlightedWidget = (Widget*)widgetList.getTail();
			}

			if (highlightedWidget->type == WG_LABEL)
				continue;

			if ((highlightedWidget->enabled) && (highlightedWidget->visible))
				break;
		}
	}

	highlightedWidget->redraw();
}

void Engine::highlightWidget(const char *name)
{
	highlightedWidget = getWidgetByName(name);
}

int Engine::processWidgets()
{
	int update = 0;

	if (keyState[SDLK_UP])
	{
		highlightWidget(-1);
		update = 1;
		clearInput();
	}

	if (keyState[SDLK_DOWN])
	{
		highlightWidget(1);
		update = 1;
		clearInput();
	}

	if (keyState[SDLK_LEFT] && (highlightedWidget->type != WG_BUTTON && highlightedWidget->type != WG_JOYPAD))
	{
		SDL_Delay(1);

		if (*highlightedWidget->value > highlightedWidget->min)
		{
			*highlightedWidget->value = *highlightedWidget->value - 1;
			update = 3;
			if ((highlightedWidget->type == WG_RADIO) || (highlightedWidget->type == WG_SLIDER))
				update = 1;
			highlightedWidget->changed = true;
		}

		if ((highlightedWidget->type == WG_RADIO) || (highlightedWidget->type == WG_SLIDER))
			clearInput();
	}

	if (keyState[SDLK_RIGHT] && (highlightedWidget->type != WG_BUTTON && highlightedWidget->type != WG_JOYPAD))
	{
		SDL_Delay(1);

		if (*highlightedWidget->value < highlightedWidget->max)
		{
			*highlightedWidget->value = *highlightedWidget->value + 1;
			update = 3;
			if ((highlightedWidget->type == WG_RADIO) || (highlightedWidget->type == WG_SLIDER))
				update = 1;
			highlightedWidget->changed = true;
		}

		if ((highlightedWidget->type == WG_RADIO) || (highlightedWidget->type == WG_SLIDER))
			clearInput();
	}

	if ((keyState[SDLK_RETURN]) || (keyState[SDLK_SPACE]) || (keyState[SDLK_LCTRL]))
	{
		if (highlightedWidget->value == NULL)
		{
			debug(("%s has not been implemented!\n", highlightedWidget->name));
		}
		else
		{
			if (highlightedWidget->type == WG_BUTTON)
			{
				*highlightedWidget->value = 1;
				highlightedWidget->changed = true;
			}
			else if (highlightedWidget->type == WG_JOYPAD)
			{
				waitForButton = true;
				waitForKey = false;
				allowJoypad = true;
				
				if (*highlightedWidget->value > -1000)
				{
					*highlightedWidget->value = (-1000 - *highlightedWidget->value);
				}
			}
			else if (highlightedWidget->type == WG_KEYBOARD)
			{
				waitForKey = true;
				waitForButton = false;
				allowJoypad = false;
				*highlightedWidget->value = -*highlightedWidget->value;
			}
			
			update = 2;
		}
		

		flushInput();
		clearInput();
	}

	return update;
}

#if !UNIX
char *strtok_r(char *s1, const char *s2, char **lasts)
{
	char *ret;
	
	if (s1 == NULL)
	{
		s1 = *lasts;
	}
	
	while (*s1 && strchr(s2, *s1))
	{
		++s1;
	}
	
	if (*s1 == '\0')
	{
		return NULL;
	}
	
	ret = s1;
	
	while(*s1 && !strchr(s2, *s1))
	{
		++s1;
	}
	
	if(*s1)
	{
		*s1++ = '\0';
	}
	
	*lasts = s1;
	
	return ret;
}
#endif

/*
Loads key-value defines into a linked list, comments are ignored. The defines.h file is used by the
game at compile time and run time, so everything is syncronised. This technique has the advantage of
allowing the game's data to be human readable and easy to maintain.
*/
bool Engine::loadDefines()
{
	char string[2][1024];

	if (!loadData("data/defines.h"))
		return false;

	char *token = strtok((char*)dataBuffer, "\n");

	Data *data;

	while (true)
	{
		token = strtok(NULL, "\n");
		if (!token)
			break;

		if (!strstr(token, "/*"))
		{
			sscanf(token, "%*s %s %[^\n\r]", string[0], string[1]);
			data = new Data();
			data->set(string[0], string[1], 1, 1);
			defineList.add(data);
		}
	}

	return true;
}

/*
Returns the value of a #defined value... ACTIVE is declared as 1 so it will
traverse the list and return 1 when it encounters ACTIVE. This has two advantages.
1) It makes the game data human readable and 2) It means if I change a #define in
the code, I don't have to change all the data entries too. You probably already
thought of that though... :)
*/
int Engine::getValueOfDefine(const char *word)
{
	int rtn = 0;

	Data *data = (Data*)defineList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;

		if (strcmp(data->key, word) == 0)
		{
			rtn = atoi(data->value);
			return rtn;
		}
	}

	printf("ERROR: getValueOfDefine() : %s is not defined!\n", word);
	exit(1);
}

/*
Does the opposite of the above(!)
*/
char *Engine::getDefineOfValue(const char *prefix, int value)
{
	int rtn = 0;

	Data *data = (Data*)defineList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;

		if (strstr(data->key, prefix))
		{
			rtn = atoi(data->value);
			
			if (rtn == value)
			{
				return data->key;
			}
		}
	}

	printf("ERROR: getDefineOfValue() : %s, %d is not defined!\n", prefix, value);
	exit(1);
}

/*
I like this function. It receives a list of flags declared by their #define name... like
the function above, delimited with plus signs. So ENT_FLIES+ENT_AIMS. It then works out the
flags (in a bit of a half arsed manner because of my lazy (2 << 25) declarations, adds all
the values together and then returns them... phew! Makes data files human readable though :)
*/
int Engine::getValueOfFlagTokens(const char *realLine)
{
	if (strcmp(realLine, "0") == 0)
		return 0;

	char *store;
	char line[1024];
	bool found;
	int value;
	strcpy(line, realLine);

	int flags = 0;

	char *word = strtok_r(line, "+", &store);

	if (!word)
	{
		printf("ERROR: getValueOfFlagTokens() : NULL Pointer!\n");
		exit(1);
	}

	Data *data;

	while (true)
	{
		data = (Data*)defineList.getHead();
		found = false;

		while (data->next != NULL)
		{
			data = (Data*)data->next;

			if (strcmp(data->key, word) == 0)
			{
				value = -1;
				sscanf(data->value, "%d", &value);

				if (value == -1)
				{
					sscanf(data->value, "%*s %*d %*s %d", &value);
					value = 2 << value;
				}

				flags += value;
				found = true;
				break;
			}
		}

		if (!found)
		{
			printf("ERROR: getValueOfFlagTokens() : Illegal Token '%s'\n", word);
			#if IGNORE_FLAGTOKEN_ERRORS
				break;
			#else
				exit(1);
			#endif
		}

		word = strtok_r(NULL, "+", &store);
		if (!word)
			break;
	}

	return flags;
}

void Engine::delay(unsigned int frameLimit) {
	unsigned int ticks = SDL_GetTicks();

	if(frameLimit < ticks)
		return;
	
	if(frameLimit > ticks + 16)
		SDL_Delay(16);
	else
		SDL_Delay(frameLimit - ticks);
}
