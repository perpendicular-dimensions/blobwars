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

#include "headers.h"

GameData::GameData()
{
	completedWorld = false;
}

void GameData::clear()
{
	objectives.clear();
}

GameData::Completion *GameData::find(const std::string &key, const std::string &value)
{
	auto it = objectives.find(key);

	if (it != objectives.end())
	{
		auto &subobjectives = it->second;
		auto subit = subobjectives.find(value);
		if (subit != subobjectives.end())
			return &subit->second;
	}

	return nullptr;
}

void GameData::addCompletedObjective(const std::string &key, const std::string &value, int current, int target)
{
	objectives[key][value].set(current, target);
}

void GameData::setMIARescueCount(const std::string &key, int rescues, int total)
{
	addCompletedObjective(key + " MIAs", "MIAs", rescues, total);
}

void GameData::getMIARescueCount(const std::string &key, int *rescues, int *total)
{
	getObjectiveValues(key + " MIAs", "MIAs", rescues, total);
}

bool GameData::MIARescued(const std::string &stageName, const std::string &name)
{
	auto it = find(stageName, "MIA_" + name);

	if (it)
		return it->isComplete();

	return false;
}

int GameData::countMIAs()
{
	int count = 0;

	for (auto &&objective: objectives)
	{
		for (auto &&subobjective: objective.second)
		{
			if (contains(subobjective.first, "MIA_"))
				count++;
		}
	}

	return count;
}

bool GameData::objectiveCompleted(const std::string &stageName, const std::string &name)
{
	auto it = find(stageName, name);

	if (it)
		return it->isComplete();

	return false;
}

void GameData::getObjectiveValues(const std::string &stageName, const std::string &name, int *current, int *target)
{
	auto it = find(stageName, name);

	if (it)
	{
		*current = it->current;
		*target = it->target;
	}
	else
	{
		*current = -1;
		*target = -1;
	}
}

bool GameData::stagePreviouslyCleared(const std::string &stageName)
{
	return objectives.find(stageName) != objectives.end();
}

bool GameData::levelPrefectlyCleared(const std::string &level)
{
	auto it = objectives.find(level);

	if (it == objectives.end() || it->second.empty())
		return false;

	for (auto &&subobjective: it->second)
	{
		if (!subobjective.second.isComplete())
			return false;
	}

	return true;
}

bool GameData::requiredLevelCleared(const std::string &requiredLevel)
{
	return objectives.find(requiredLevel) != objectives.end();
}

/*
Whether or not all the levels in the game have been unlocked
*/
void GameData::calculateWorldCompleted()
{
	completedWorld = requiredLevelCleared("BioMech HQ");
}

int GameData::getPercentageComplete()
{
	float percentage = 0;
	float total = 0;
	float completed = 0;

	for (auto &&objective: objectives)
	{
		for (auto &&subobjective: objective.second)
		{
			total++;
			if (subobjective.second.isComplete())
				completed++;
		}
	}

	if ((total == 0) || (completed == 0))
		return 0;

	percentage = (completed / total);
	percentage *= 100;

	return (int)percentage;
}
