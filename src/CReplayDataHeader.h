struct ReplayDataHeader
{
	char map[PATH_MAX];
	long randomSeed;
	float version;
	int release;
	int skill;
	int score;
	unsigned int time;
};
