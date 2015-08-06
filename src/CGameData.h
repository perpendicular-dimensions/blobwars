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

class GameData {

	public:
		
		bool completedWorld;

		List dataList;

	void clear();
	void destroy();
	
	void addCompletedObjective(const char *key, const char *value, int current, int target);
	void addCompletedObjective(Data *newData);
	
	void setMIARescueCount(const char *key, int rescues, int total);
	bool MIARescued(const char *stageName, char *name);
	
	bool objectiveCompleted(const char *stageName, const char *name);
	void getObjectiveValues(const char *stageName, const char *name, int *current, int *target);
	
	bool stagePreviouslyCleared(const char *stageName);
	bool isCompleted(const char *key, const char *value);
	int getPercentageComplete();
	
	bool levelPrefectlyCleared(const char *level);
	bool requiredLevelCleared(const char *requiredLevel);
	
	void calculateWorldCompleted();
	void calculateAllCompleted();

	GameData();

};
