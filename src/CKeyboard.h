class Keyboard {

	public:
		
			int control[CONTROL::MAX];
	
		Keyboard();
		void setDefaultKeys();
		static const char *translateKey(int value);

};
