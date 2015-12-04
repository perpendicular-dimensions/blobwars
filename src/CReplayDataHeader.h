struct ReplayDataHeader
{
	char map[PATH_MAX];
	time_t randomSeed;
	float version;
	int release;
	int skill;
	int score;
	unsigned int time;
};
