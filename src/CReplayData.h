class ReplayData
{
	private:
	
		static const int DATA_LENGTH = 650;
	
		FILE *fp;
		int index;
		char data[DATA_LENGTH][CONTROL::MAX];
		
		bool endOfReplay;

	public:
	
			bool fast;
	
			char filename[PATH_MAX];
	
			REPLAY_MODE::TYPE replayMode;
			ReplayDataHeader header;
	
		ReplayData();
		~ReplayData();
		
		void setMode(REPLAY_MODE::TYPE replayMode);
		void printReplayInformation();
		
		void swapHeaderEndians();
		
		void read(int *c);
		void set(int *c);
		
		void load();
		void save();
		void commit();
		
		void reset();
};
