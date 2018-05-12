class Keyboard
{
public:
	int control[CONTROL::MAX];

	Keyboard();
	void setDefaultKeys();
	static std::string translateKey(int value);
};
