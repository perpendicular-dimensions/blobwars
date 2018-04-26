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
	dataList.clear();
}

void GameData::destroy()
{
	clear();
}

void GameData::addCompletedObjective(const std::string &key, const std::string &value, int current, int target)
{
	Data *data = (Data*)dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		if (key == data->key)
		{
			if (value == data->value)
			{
				data->set(key, value, current, target);
				return;
			}
		}
	}

	data = new Data();
	data->set(key, value, current, target);

	dataList.add(data);
}

void GameData::addCompletedObjective(Data *newData)
{
	Data *data = (Data*)dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		if (data->key == newData->key)
		{
			if (data->value == newData->value)
			{
				data->set(newData->key, newData->value, newData->current, newData->target);
				return;
			}
		}
	}

	dataList.add(newData);
}

void GameData::setMIARescueCount(const std::string &key, int rescues, int total)
{
	Data *data = (Data*)dataList.getHead();

	std::string newKey = key + " MIAs";

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		if (newKey == data->key)
		{
			data->value = "MIAs";
			data->current = rescues;
			data->target = total;
			return;
		}
	}

	data = new Data();

	data->set(newKey, "MIAs", rescues, total);

	dataList.add(data);
}

bool GameData::MIARescued(const std::string &stageName, const std::string &name)
{
	Data *data = (Data*)dataList.getHead();

	std::string  newName = "MIA_" + name;

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		if (data->key == stageName)
		{
			if (data->value == newName)
			{
				return data->isComplete();
			}
		}
	}

	return false;
}

bool GameData::objectiveCompleted(const std::string &stageName, const std::string &name)
{
	Data *data = (Data*)dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		if (data->key == stageName)
		{
			if (data->value == name)
			{
				return (data->current == data->target);
			}
		}
	}

	return false;
}

void GameData::getObjectiveValues(const std::string &stageName, const std::string &name, int *current, int *target)
{
	*current = -1;
	*target = -1;
	
	Data *data = (Data*)dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		if (data->key == stageName)
		{
			if (data->value == name)
			{
				data->getCurrentTarget(current, target);
				return;
			}
		}
	}
}

bool GameData::stagePreviouslyCleared(const std::string &stageName)
{
	Data *data = (Data*)dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		if (data->key == stageName)
		{
			return true;
		}
	}

	return false;
}

bool GameData::isCompleted(const std::string &key, const std::string &value)
{
	Data *data = (Data*)dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		if (key == data->key)
		{
			if (value == data->value)
				return true;
		}
	}

	return false;
}

bool GameData::levelPrefectlyCleared(const std::string &level)
{
	Data *data = (Data*)dataList.getHead();
	
	bool found = false;

	while (data->next != NULL)
	{
		data = (Data*)data->next;

		if (data->key == level)
		{
			found = true;

			if (!data->isComplete())
				return false;
		}
	}
	
	if (!found)
		return false;

	return true;
}

bool GameData::requiredLevelCleared(const std::string &requiredLevel)
{
	Data *data = (Data*)dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;

		if (data->key == requiredLevel)
		{
			return true;
		}
	}

	return false;
}

/*
Whether or not all the levels in the game have been unlocked
*/
void GameData::calculateWorldCompleted()
{
	completedWorld = false;
	
	Data *data = (Data*)dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		
		if (data->key == "BioMech HQ")
		{
			completedWorld = true;
		}
	}
}

int GameData::getPercentageComplete()
{
	float percentage, total, completed;

	total = completed = percentage = 0;

	Data *data = (Data*)dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;

		total++;

		if (data->isComplete())
			completed++;
	}

	if ((total == 0) || (completed == 0))
		return 0;

	percentage = (completed / total);
	percentage *= 100;

	return (int)percentage;
}
