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

void GameData::addCompletedObjective(const char *key, const char *value, int current, int target)
{
	Data *data = (Data*)dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		if (strcmp(key, data->key) == 0)
		{
			if (strcmp(value, data->value) == 0)
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
		if (strcmp(data->key, newData->key) == 0)
		{
			if (strcmp(data->value, newData->value) == 0)
			{
				data->set(newData->key, newData->value, newData->current, newData->target);
				return;
			}
		}
	}

	dataList.add(newData);
}

void GameData::setMIARescueCount(const char *key, int rescues, int total)
{
	Data *data = (Data*)dataList.getHead();

	char newKey[100];
	snprintf(newKey, sizeof newKey, "%s MIAs", key);

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		if (strcmp(newKey, data->key) == 0)
		{
			strlcpy(data->value, "MIAs", sizeof data->value);
			data->current = rescues;
			data->target = total;
			return;
		}
	}

	data = new Data();

	data->set(newKey, "MIAs", rescues, total);

	dataList.add(data);
}

bool GameData::MIARescued(const char *stageName, char *name)
{
	Data *data = (Data*)dataList.getHead();

	char newName[100];
	snprintf(newName, sizeof newName, "MIA_%s", name);

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		if (strcmp(data->key, stageName) == 0)
		{
			if (strcmp(data->value, newName) == 0)
			{
				return data->isComplete();
			}
		}
	}

	return false;
}

bool GameData::objectiveCompleted(const char *stageName, const char *name)
{
	Data *data = (Data*)dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		if (strcmp(data->key, stageName) == 0)
		{
			if (strcmp(data->value, name) == 0)
			{
				return (data->current == data->target);
			}
		}
	}

	return false;
}

void GameData::getObjectiveValues(const char *stageName, const char *name, int *current, int *target)
{
	*current = -1;
	*target = -1;
	
	Data *data = (Data*)dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		if (strcmp(data->key, stageName) == 0)
		{
			if (strcmp(data->value, name) == 0)
			{
				data->getCurrentTarget(current, target);
				return;
			}
		}
	}
}

bool GameData::stagePreviouslyCleared(const char *stageName)
{
	Data *data = (Data*)dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		if (strcmp(data->key, stageName) == 0)
		{
			return true;
		}
	}

	return false;
}

bool GameData::isCompleted(const char *key, const char *value)
{
	Data *data = (Data*)dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		if (strcmp(key, data->key) == 0)
		{
			if (strcmp(value, data->value) == 0)
				return true;
		}
	}

	return false;
}

bool GameData::levelPrefectlyCleared(const char *level)
{
	Data *data = (Data*)dataList.getHead();
	
	bool found = false;

	while (data->next != NULL)
	{
		data = (Data*)data->next;

		if (strcmp(data->key, level) == 0)
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

bool GameData::requiredLevelCleared(const char *requiredLevel)
{
	Data *data = (Data*)dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;

		if (strcmp(data->key, requiredLevel) == 0)
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
		
		if (strcmp(data->key, "BioMech HQ") == 0)
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
