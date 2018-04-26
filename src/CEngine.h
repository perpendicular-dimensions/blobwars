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

class Engine {

	private:

		SDL_Event event;
		signed char frameLoop;
		int mouseX, mouseY;

		// Time Difference
		unsigned int time1;
		unsigned int time2;
		float timeDifference;
		
		// used for cheating!
		std::string lastKeyPressed;
		std::string lastKeyEvents;
		
		// Joystick config stuff
		int lastButtonPressed;
		
		Pak pak;

	public:
		
		int extremeAvailable;

		char keyState[SDL_NUM_SCANCODES];
		char mouseLeft, mouseRight;
		
		int joyX, joyY;
		int joyprevX, joyprevY;
		int joykeyX, joykeyY;
		bool joykeyFire;
		int joystickState[32]; // hopefully no one has a joystick with this many buttons(!)
		bool waitForButton;
		bool waitForKey;
		bool allowJoypad;

		bool paused;
		bool saveConfig;
		bool allowQuit;

		std::string userHomeDirectory;

		int useAudio;
		int fullScreen;
		int skill;
		bool practice;

		// Development stuff
		bool devNoMonsters;

		SDL_RWops *sdlrw;

		std::vector<char> binaryBuffer; // used for unzipping
		std::vector<char> dataBuffer; // used for unzipping

		int messageTime;
		int messagePriority;
		int messageType;
		std::string message;

		std::string saveSlot[10];
		int continueSaveSlot;

		int playerPosX, playerPosY;

		List widgetList;

		Widget *highlightedWidget;

		Entity world;

		List defineList;

		bool cheats;
		int cheatHealth, cheatExtras, cheatFuel, cheatLevels, cheatBlood, cheatInvulnerable;
		int cheatReload, cheatSpeed, cheatSkipLevel;

	Engine();
	void destroy();
	void getInput();
	int getMouseX() const;
	int getMouseY() const;

	void moveMouse(int dx, int dy);
	bool userAccepts();
	
	void clearCheatVars();
	bool compareLastKeyInputs();
	void addKeyEvent();
	void flushInput();
	void clearInput();
	void setUserHome(const std::string &path);
	Pak *getPak();
	bool unpack(const std::string &filename, int fileType);
	bool loadData(const std::string &filename);
	void reportFontFailure();
	void setPlayerPosition(int x, int y, int limitLeft, int limitRight, int limitUp, int limitDown);
	int getFrameLoop() const;
	void doFrameLoop();
	void doTimeDifference();
	float getTimeDifference() const;
	void resetTimeDifference();
	void setInfoMessage(const std::string &message, int priority, int type);
	void deleteWidgets();
	void addWidget(Widget *widget);
	bool loadWidgets(const std::string &filename);
	Widget *getWidgetByName(const std::string &name);
	void showWidgetGroup(const std::string &groupName, bool show);
	void enableWidgetGroup(const std::string &groupName, bool show);
	void showWidget(const std::string &name, bool show);
	void enableWidget(const std::string &name, bool enable);
	void setWidgetVariable(const std::string &name, int *variable);
	bool widgetChanged(const std::string &name);
	void highlightWidget(int dir);
	void highlightWidget(const std::string &name);
	int processWidgets();
	bool loadDefines();
	int getValueOfDefine(const std::string &word);
	std::string getDefineOfValue(const std::string &prefix, int value);
	int getValueOfFlagTokens(const std::string &line);
	void delay(unsigned int frameLimit);
};
