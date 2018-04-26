class ReplayData
{
	private:
	
		static const int DATA_LENGTH = 650;
	
		std::fstream file;
		int index;
		char data[DATA_LENGTH][CONTROL::MAX];
		
		bool endOfReplay;

	public:
	
			bool fast;
	
			std::string filename;
	
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
