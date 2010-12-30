class Config
{
	public:
	
		Engine *engine;

		int command[CONTROL::MAX];
		
		SDL_Joystick *sdlJoystick;
		
		Joystick joystick;
		Keyboard keyboard;

	Config();
	~Config();
	
	void populate();
	void populate(int *data);
	
	bool isControl(CONTROL::TYPE type) const;
	void resetControl(CONTROL::TYPE type);
	
	void doPause();
	
	bool loadJoystickConfig();
	bool saveJoystickConfig();
	bool loadKeyConfig();
	bool saveKeyConfig();
	void restoreKeyDefaults();
};
