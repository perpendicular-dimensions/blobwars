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
#include <errno.h>
extern Graphics graphics;

Engine::Engine()
{
	memset(keyState, 0, sizeof keyState);

	memset(joystickState, 0, sizeof joystickState);

	joyX = joyY = 0;

	mouseLeft = mouseRight = 0;
	waitForButton = false;
	waitForKey = false;
	
	allowJoypad = true;

	fullScreen = 0;

	useAudio = 2;
	
	practice = false;
	
	allowQuit = false;

	saveConfig = false;

	highlightedWidget = nullptr;
	highlightedIndex = 0;

	message[0] = 0;
	messageTime = -1;

	// Development Stuff
	devNoMonsters = false;

	#ifdef FRAMEWORK_SDL
	char pakPath[PATH_MAX] = PAKFULLPATH;
	if (CFBundleGetMainBundle() != nullptr) {
		CFURLRef pakURL = CFBundleCopyResourceURL(CFBundleGetMainBundle(), CFSTR(PAKNAME), nullptr, nullptr);
		if (pakURL != nullptr) {
			CFShow(pakURL);
			CFURLGetFileSystemRepresentation(pakURL, true, (uint8_t*)pakPath, sizeof(pakPath));
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
	cheatHealth = cheatExtras = cheatFuel = cheatLevels = false;
	cheatBlood = cheatInvulnerable = cheatReload = cheatSpeed = cheatSkipLevel = false;
	
	extremeAvailable = 0;
}

void Engine::clearCheatVars()
{
	lastKeyEvents.clear();
	cheatHealth = cheatExtras = cheatFuel = cheatLevels = false;
	cheatBlood = cheatInvulnerable = cheatReload = cheatSpeed = cheatSkipLevel = false;
}

bool Engine::compareLastKeyInputs()
{
	if (contains(lastKeyEvents, "LOCKANDLOAD"))
	{
		cheats = true;
		return true;
	}

	return false;
}

void Engine::addKeyEvent()
{
	if (lastKeyPressed.size() != 1)
	{
		return;
	}

	if (lastKeyEvents.size() >= 25)
		lastKeyEvents.erase(0, 1);

	lastKeyEvents.append(lastKeyPressed);

	compareLastKeyInputs();
}

void Engine::getInput()
{
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

			case SDL_MOUSEMOTION:
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				break;

			case SDL_KEYDOWN:
				
				if (waitForButton)
				{
					if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					{
						lastButtonPressed = -1;
						*highlightedWidget->value = abs(*highlightedWidget->value) - 1000;
						highlightedWidget->redraw();
						waitForButton = false;
						allowJoypad = false;
					}
					
					if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
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
					if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					{
						*highlightedWidget->value = -*highlightedWidget->value;
					}
					else
					{
						*highlightedWidget->value = event.key.keysym.scancode;
					}
					
					lastButtonPressed = -1;
					highlightedWidget->redraw();
					waitForButton = false;
					waitForKey = false;
					allowJoypad = false;
					
					return;
				}

				keyState[event.key.keysym.scancode] = 1;
				lastKeyPressed = SDL_GetKeyName(SDL_GetKeyFromScancode(event.key.keysym.scancode));
				addKeyEvent();
				break;

			case SDL_KEYUP:
				keyState[event.key.keysym.scancode] = 0;
				break;

			case SDL_JOYAXISMOTION:
				if (event.jaxis.axis == 0)
				{
					joyX = event.jaxis.value;
					int joycurX = joyX < -25000 ? -1 : joyX > 25000 ? 1 : 0;
					if (joycurX != joyprevX)
						joykeyX = joyprevX = joycurX;
				}
				else if (event.jaxis.axis == 1)
				{
					joyY = event.jaxis.value;
					int joycurY = joyY < -25000 ? -1 : joyY > 25000 ? 1 : 0;
					if (joycurY != joyprevY)
						joykeyY = joyprevY = joycurY;
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
				joykeyFire = true;
				break;

			case SDL_JOYBUTTONUP:
				joystickState[event.jbutton.button] = 0;
				joykeyFire = false;
				break;

			case SDL_JOYHATMOTION:
				switch (event.jhat.value) {
				case SDL_HAT_CENTERED:
					joyX = 0,      joyY = 0;      break;
				case SDL_HAT_LEFT:
					joyX = -32768, joyY = 0;      break;
				case SDL_HAT_LEFTUP:
					joyX = -32768, joyY = -32768; break;
				case SDL_HAT_UP:
					joyX = 0,      joyY = -32768; break;
				case SDL_HAT_RIGHTUP:
					joyX = 32767,  joyY = -32768; break;
				case SDL_HAT_RIGHT:
					joyX = 32767,  joyY = 0;      break;
				case SDL_HAT_RIGHTDOWN:
					joyX = 32767,  joyY = 32767;  break;
				case SDL_HAT_DOWN:
					joyX = 0,      joyY = 32767;  break;
				case SDL_HAT_LEFTDOWN:
					joyX = -32768, joyY = 32767;  break;
				}

				if (joyX != joyprevX)
					joykeyX = joyprevX = joyX;
				if (joyY != joyprevY)
					joykeyY = joyprevY = joyY;

				break;

			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
					paused = true;
				break;

			default:
				break;
		}
	}
}

int Engine::getMouseX() const
{
	return mouseX;
}

int Engine::getMouseY() const
{
	return mouseY;
}

void Engine::moveMouse(int dx, int dy)
{
	mouseX += dx;
	mouseY += dy;
	Math::limitInt(&mouseX, 0, 640);
	Math::limitInt(&mouseY, 0, 480);
}

bool Engine::userAccepts()
{
	return keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_ESCAPE] || keyState[SDL_SCANCODE_LCTRL] || keyState[SDL_SCANCODE_RCTRL] || keyState[SDL_SCANCODE_RETURN] || keyState[SDL_SCANCODE_LCTRL] || joykeyFire;
}

void Engine::flushInput()
{
	while (SDL_PollEvent(&event)){}
}

void Engine::clearInput()
{
	memset(keyState, 0, sizeof keyState);

	mouseLeft = mouseRight = 0;
	joykeyX = joykeyY = 0;
	joykeyFire = false;
}

void Engine::setUserHome(const std::string &path)
{
	userHomeDirectory = path;
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
bool Engine::unpack(const std::string &filename, int fileType)
{
	bool ret = true;

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
		sdlrw = SDL_RWFromMem(binaryBuffer.data(), pak.getUncompressedSize());
		if (!sdlrw)
		{
			printf("Fatal Error: SDL_RWops allocation failed\n");
			exit(1);
		}
	}

	if ((fileType == PAK_MUSIC) || (fileType == PAK_FONT) || (fileType == PAK_TAGS))
	{
		std::string tempPath = userHomeDirectory;
		
		if (fileType == PAK_MUSIC)
		{
			tempPath.append("music.mod");
		}
		else if (fileType == PAK_TAGS)
		{
			tempPath.append("music.tags");
		}
		else if (fileType == PAK_FONT)
		{
			tempPath.append("font.ttf");
		}

		std::ofstream file(tempPath);

		file.write((const char *)binaryBuffer.data(), pak.getUncompressedSize());
		file.close();

		if (file.bad())
		{
			fmt::print("Fatal Error: could not write to {}: {}", tempPath, strerror(errno));
			ret = false;
		}
	}

	debug(("unpack() : Loaded %s (%d), ret: %d\n", filename, pak.getUncompressedSize(), (int)ret));

	return ret;
}

bool Engine::loadData(const std::string &filename)
{
	bool ret = true;

	#if USEPAK
		return unpack(filename, PAK_DATA);
	#endif

	std::ifstream file(filename);

	size_t fSize = file.seekg(0, std::ios_base::end).tellg();
	file.seekg(0);

	dataBuffer.resize(fSize + 1);

	file.read((char *)dataBuffer.data(), fSize);
	dataBuffer[fSize] = 0;

	if (file.bad())
		ret = false;

	debug(("loadData() : Loaded %s (%d), ret: %d\n", filename, fSize, (int)ret));

	return ret;
}

void Engine::reportFontFailure()
{
	fmt::print("\nUnable to load font. The game cannot continue without it.\n");
	fmt::print("Please confirm that the game and all required SDL libraries are installed\n");
	fmt::print("The following information may be useful to you,\n\n");
	fmt::print("Expected location of PAK file: {}\n", PAKFULLPATH);
	fmt::print("Location of TMP directory: {}\n", userHomeDirectory);
	fmt::print("\nAlso try checking http://www.parallelrealities.co.uk/blobWars.php for updates\n\n");
	exit(1);
}

void Engine::setPlayerPosition(int x, int y, int limitLeft, int limitRight, int limitUp, int limitDown)
{
	playerPosX = x - OFFSETX;
	playerPosY = y - OFFSETY;

	Math::limitInt(&playerPosX, limitLeft, limitRight);
	Math::limitInt(&playerPosY, limitUp, limitDown);
}

int Engine::getFrameLoop() const
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

float Engine::getTimeDifference() const
{
	return timeDifference;
}

void Engine::resetTimeDifference()
{
	time1 = time2 = SDL_GetTicks();
}

void Engine::setInfoMessage(const std::string &message, int priority, int type)
{
	if (priority >= messagePriority)
	{
		this->message = message;
		messageTime = 180;
		messagePriority = priority;
		messageType = type;
	}
}

void Engine::deleteWidgets()
{
	for (auto &&widget: widgets)
		widget->redraw(); // wat?!

	widgets.clear();

	highlightedWidget = nullptr;
	highlightedIndex = 0;
}

void Engine::addWidget(Widget *widget)
{
	widgets.emplace_back(widget);
}

bool Engine::loadWidgets(const std::string &filename)
{
	deleteWidgets();

	if (!loadData(filename))
		return false;

	char token[50], name[50], groupName[50], label[80], options[100];
	int x, y, min, max;

	int i;

	Widget *widget;

	for (auto line: split(dataBuffer, '\n'))
	{
		scan(line, "%s %s %s %*c %[^\"] %*c %*c %[^\"] %*c %d %d %d %d", token, name, groupName, label, options, &x, &y, &min, &max);

		if (strcmp(token, "END") == 0)
			break;

		widget = new Widget;

		i = 0;

		while (true)
		{
			if (token == widgetName[i])
				widget->type = i;

			if ("-1" == widgetName[i])
				break;

			i++;
		}

		widget->setProperties(name, groupName, label, options, x, y, min, max);

		addWidget(widget);
	}

	highlightedWidget = widgets.front().get();
	highlightedIndex = 0;

	return true;
}

int Engine::getWidgetIndexByName(const std::string &name)
{
	int i = 0;

	for (auto &&widget: widgets)
	{
		if (widget->name == name)
			return i;
		i++;
	}

	return -1;
}

Widget *Engine::getWidgetByName(const std::string &name)
{
	for (auto &&widget: widgets)
	{
		if (widget->name == name)
			return widget.get();
	}

	return nullptr;

	debug(("No such widget '%s'\n", name));

	return nullptr;
}

void Engine::showWidgetGroup(const std::string &groupName, bool show)
{
	bool found = false;

	for (auto &&widget: widgets)
	{
		if (widget->groupName == groupName)
		{
			widget->visible = show;
			widget->redraw();
			found = true;
		}
	}

	if (!found)
		debug(("Group '%s' does not exist\n", groupName));
}

void Engine::enableWidgetGroup(const std::string &groupName, bool show)
{
	bool found = false;

	for (auto &&widget: widgets)
	{
		if (widget->groupName == groupName)
		{
			widget->enabled = show;
			widget->redraw();
			found = true;
		}
	}

	if (!found)
		debug(("Group '%s' does not exist\n", groupName));
}

void Engine::showWidget(const std::string &name, bool show)
{
	Widget *widget = getWidgetByName(name);
	if (widget != nullptr)
	{
		widget->visible = show;
		widget->redraw();
	}
}

void Engine::enableWidget(const std::string &name, bool enable)
{
	Widget *widget = getWidgetByName(name);
	if (widget != nullptr)
	{
		widget->enabled = enable;
		widget->redraw();
	}
}

void Engine::setWidgetVariable(const std::string &name, int *variable)
{
	Widget *widget = getWidgetByName(name);
	if (widget != nullptr)
		widget->value = variable;
}

bool Engine::widgetChanged(const std::string &name)
{
	Widget *widget = getWidgetByName(name);
	if (widget != nullptr)
		return widget->changed;

	return false;
}

void Engine::highlightWidget(int dir)
{
	highlightedWidget->redraw();

	while (true)
	{
		highlightedIndex += dir;
		if (highlightedIndex < 0)
			highlightedIndex = widgets.size() - 1;
		else if ((size_t)highlightedIndex >= widgets.size())
			highlightedIndex = 0;

		highlightedWidget = widgets[highlightedIndex].get();

		if (highlightedWidget->type == WG_LABEL)
			continue;

		if ((highlightedWidget->enabled) && (highlightedWidget->visible))
			break;
	}

	highlightedWidget->redraw();
}

void Engine::highlightWidget(const std::string &name)
{
	highlightedIndex = getWidgetIndexByName(name);
	highlightedWidget = widgets[highlightedIndex].get();
}

int Engine::processWidgets()
{
	int update = 0;

	if (keyState[SDL_SCANCODE_UP] || joykeyY < 0)
	{
		highlightWidget(-1);
		update = 1;
		clearInput();
	}

	if (keyState[SDL_SCANCODE_DOWN] || joykeyY > 0)
	{
		highlightWidget(1);
		update = 1;
		clearInput();
	}

	if ((keyState[SDL_SCANCODE_LEFT] || joykeyX < 0) && (highlightedWidget->type != WG_BUTTON && highlightedWidget->type != WG_JOYPAD))
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

	if ((keyState[SDL_SCANCODE_RIGHT] || joykeyX > 0) && (highlightedWidget->type != WG_BUTTON && highlightedWidget->type != WG_JOYPAD))
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

	if ((keyState[SDL_SCANCODE_RETURN]) || (keyState[SDL_SCANCODE_SPACE]) || (keyState[SDL_SCANCODE_LCTRL]) || (joykeyFire && highlightedWidget->type != WG_JOYPAD))
	{
		if (highlightedWidget->value == nullptr)
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

	if (joykeyX > 0 && highlightedWidget->type == WG_JOYPAD)
	{
		waitForButton = true;
		waitForKey = false;
		allowJoypad = true;

		if (*highlightedWidget->value > -1000)
		{
			*highlightedWidget->value = (-1000 - *highlightedWidget->value);
		}

		clearInput();
	}

	if (joykeyX < 0 && highlightedWidget->type == WG_JOYPAD)
	{
		if (waitForButton)
		{
			lastButtonPressed = -1;
			*highlightedWidget->value = abs(*highlightedWidget->value) - 1000;
		}
		else
		{
			lastButtonPressed = -2;
			*highlightedWidget->value = -2;
		}

		waitForButton = false;
		allowJoypad = false;
		highlightedWidget->redraw();
		clearInput();
	}

	return update;
}

/*
Loads key-value defines into a linked list, comments are ignored. The defines.h file is used by the
game at compile time and run time, so everything is synchronised. This technique has the advantage of
allowing the game's data to be human readable and easy to maintain.
*/
bool Engine::loadDefines()
{
	char string[2][1024];

	if (!loadData("data/defines.h"))
		return false;

	for (auto line: split(dataBuffer, '\n'))
	{
		if (!contains(line, "/*"))
		{
			scan(line, "%*s %s %[^\n\r]", string[0], string[1]);
			defines[string[0]] = string[1];
		}
	}

	return true;
}

static int parseDefine(const std::string &value)
{
	if (!value.empty() && value[0] == '(')
	{
		int base, shift;
		sscanf(value.c_str(), "( %d << %d )", &base, &shift);
		return base << shift;
	}
	else
	{
		return stoi(value);
	}
}

/*
Returns the value of a #defined value... ACTIVE is declared as 1 so it will
traverse the list and return 1 when it encounters ACTIVE. This has two advantages.
1) It makes the game data human readable and 2) It means if I change a #define in
the code, I don't have to change all the data entries too. You probably already
thought of that though... :)
*/
int Engine::getValueOfDefine(const std::string &word)
{
	auto it = defines.find(word);

	if (it != defines.end())
	{
		return parseDefine(it->second);
	}

	fmt::print("ERROR: getValueOfDefine() : {} is not defined!\n", word);
	exit(1);
}

/*
Does the opposite of the above(!)
*/
std::string Engine::getDefineOfValue(const std::string &prefix, int value)
{
	for (auto it = defines.begin(); it != defines.end(); ++it)
	{
		if (contains(it->first, prefix))
		{
			int rtn = parseDefine(it->second);

			if (rtn == value)
			{
				return it->first;
			}
		}
	}

	fmt::print("ERROR: getDefineOfValue() : {}, {} is not defined!\n", prefix, value);
	exit(1);
}

/*
I like this function. It receives a list of flags declared by their #define name... like
the function above, delimited with plus signs. So ENT_FLIES+ENT_AIMS. It then works out the
flags (in a bit of a half arsed manner because of my lazy (2 << 25) declarations, adds all
the values together and then returns them... phew! Makes data files human readable though :)
*/
int Engine::getValueOfFlagTokens(const std::string &line)
{
	if (line == "0")
		return 0;

	int flags = 0;

	for (auto word: split(line, '+'))
	{
		flags |= getValueOfDefine(std::string(word));
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
