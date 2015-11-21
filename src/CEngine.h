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
		char lastKeyPressed[25];
		char lastKeyEvents[25];
		
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

		char userHomeDirectory[PATH_MAX];

		int useAudio;
		int fullScreen;
		int skill;
		bool practice;

		// Development stuff
		bool devNoMonsters;

		SDL_RWops *sdlrw;

		unsigned char *binaryBuffer; // used for unzipping
		unsigned char *dataBuffer; // used for unzipping

		int messageTime;
		int messagePriority;
		int messageType;
		char message[100];

		char saveSlot[10][80];
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
	void setUserHome(const char *path);
	Pak *getPak();
	bool unpack(const char *filename, int fileType);
	bool loadData(const char *filename);
	void reportFontFailure();
	void setPlayerPosition(int x, int y, int limitLeft, int limitRight, int limitUp, int limitDown);
	int getFrameLoop() const;
	void doFrameLoop();
	void doTimeDifference();
	float getTimeDifference() const;
	void resetTimeDifference();
	void setInfoMessage(const char *message, int priority, int type);
	void deleteWidgets();
	void addWidget(Widget *widget);
	bool loadWidgets(const char *filename);
	Widget *getWidgetByName(const char *name);
	void showWidgetGroup(const char *groupName, bool show);
	void enableWidgetGroup(const char *groupName, bool show);
	void showWidget(const char *name, bool show);
	void enableWidget(const char *name, bool enable);
	void setWidgetVariable(const char *name, int *variable);
	bool widgetChanged(const char *name);
	void highlightWidget(int dir);
	void highlightWidget(const char *name);
	int processWidgets();
	bool loadDefines();
	int getValueOfDefine(const char *word);
	char *getDefineOfValue(const char *prefix, int value);
	int getValueOfFlagTokens(const char *line);
	void delay(unsigned int frameLimit);
};
