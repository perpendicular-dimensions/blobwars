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

void adjustObjectives()
{
	bool needRequired = false;

	for (auto &&mia: map.mias)
	{
		if (gameData.MIARescued(map.name, mia.name))
		{
			mia.health = 0;
			map.foundMIAs++;
		}
	}

	if (map.foundMIAs >= map.requiredMIAs)
	{
		map.requiredMIAs = map.totalMIAs;
		needRequired = true;
	}

	if (game.skill == 3)
	{
		map.requiredMIAs = map.totalMIAs;
		needRequired = true;
	}

	int current = 0;
	int target = 0;

	bool previouslyCleared = gameData.stagePreviouslyCleared(map.name);

	for (auto &&objective: map.objectives)
	{
		if (game.skill == 0)
		{
			objective.required = false;
		}

		if (game.skill >= 3)
		{
			objective.required = true;
		}

		if (needRequired)
		{
			objective.required = true;
		}

		if (previouslyCleared)
		{
			if (gameData.objectiveCompleted(map.name, objective.description))
			{
				objective.completed = true;

				debug("Objective {} has been completed\n", objective.description);
			}

			gameData.getObjectiveValues(map.name, objective.description, &current, &target);
			objective.currentValue = current;
			objective.targetValue = target;

			// Make sure that gameplay adjustments are catered for...
			if (!objective.completed)
			{
				Math::limitInt(&objective.currentValue, 0, objective.targetValue);

				if (objective.currentValue == objective.targetValue)
				{
					objective.completed = true;

					debug("Objective {} has been completed (gameplay adjustment!)\n", objective.description);
				}
			}
		}
	}
}

bool allObjectivesCompleted()
{
	if (map.foundMIAs < map.requiredMIAs)
		return false;

	for (auto &&objective: map.objectives)
	{
		if ((objective.required) && (!objective.completed))
			return false;
	}

	return true;
}

bool perfectlyCompleted()
{
	if (map.foundMIAs < map.totalMIAs)
		return false;

	for (auto &&objective: map.objectives)
	{
		if (!objective.completed)
			return false;
	}

	return true;
}

bool requiredEnemy(const std::string &name)
{
	for (auto &&objective: map.objectives)
	{
		if (objective.target == name)
		{
			return true;
		}
	}

	return false;
}

bool requiredObjectivesCompleted()
{
	if (map.foundMIAs < map.requiredMIAs)
	{
		return false;
	}

	for (auto &&objective: map.objectives)
	{
		if (objective.target == "Exit")
		{
			continue;
		}

		if ((objective.required) && (!objective.completed))
		{
			return false;
		}
	}

	return true;
}

/*
Used by the skip level cheat
*/
void autoCompleteAllObjectives(bool allObjectives)
{
	if (allObjectives)
	{
		map.requiredMIAs = map.totalMIAs;
		map.foundMIAs = map.totalMIAs;
	}
	else
	{
		map.foundMIAs = map.requiredMIAs;
	}

	map.foundItems = map.totalItems;

	for (auto &&objective: map.objectives)
	{
		if (allObjectives)
		{
			objective.completed = true;
			objective.currentValue = objective.targetValue;
		}
		else
		{
			if (objective.required)
			{
				objective.completed = true;
				objective.currentValue = objective.targetValue;
			}
		}
	}

	int required = map.foundMIAs;

	for (auto &&mia: map.mias)
	{
		mia.health = 0;
		required--;

		if (required == 0)
			break;
	}
}

void checkObjectives(const std::string &name, bool alwaysInform)
{
	std::string message;

	int requiredValue;

	for (auto &&objective: map.objectives)
	{
		if (!objective.completed)
		{
			if (objective.target == name)
			{
				if (contains(objective.target, "Combo-"))
				{
					objective.currentValue = game.currentComboHits;
					Math::limitInt(&objective.currentValue, 0, objective.targetValue);
				}
				else
				{
					objective.currentValue++;
				}

				requiredValue = objective.targetValue - objective.currentValue;

				if (objective.currentValue == objective.targetValue)
				{
					if (!map.isBossMission)
					{
						message = fmt::format(_("{} - Objective Completed - Check Point Reached!"), _(objective.description.c_str()));
						game.setObjectiveCheckPoint();
					}
					else
					{
						message = fmt::format(_("{} - Objective Completed"), _(objective.description.c_str()));
					}

					if (objective.description == "Get the Aqua Lung")
					{
						message = fmt::format("Got the Aqua Lung! You can now swim forever!");
						game.hasAquaLung = true;
						presentPlayerMedal("Aqua_Lung");
					}

					if (objective.description == "Get the Jetpack")
					{
						message = fmt::format("Got the Jetpack! Press SPACE to Activate!");
						game.hasJetPack = true;
						presentPlayerMedal("Jetpack");
					}

					engine.setInfoMessage(message, 9, INFO_OBJECTIVE);
					objective.completed = true;
					game.totalObjectivesCompleted++;
				}
				else if (!contains(objective.target, "Combo-"))
				{
					if ((requiredValue % 10 == 0) || (requiredValue <= 10) || (alwaysInform))
					{
						switch (Math::prand() % 3)
						{
						case 0:
							message = fmt::format(_("{} - {} more to go..."), _(objective.description.c_str()), requiredValue);
							break;
						case 1:
							message = fmt::format(_("{} - need {} more"), _(objective.description.c_str()), requiredValue);
							break;
						case 2:
							message = fmt::format(_("{} - {} of {}"), _(objective.description.c_str()), objective.currentValue, objective.targetValue);
							break;
						}

						engine.setInfoMessage(message, 1, INFO_NORMAL);
					}
					else
					{
						return;
					}
				}
				else
				{
					continue;
				}
			}
		}
	}
}
