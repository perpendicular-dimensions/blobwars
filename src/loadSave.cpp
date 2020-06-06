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

void initSaveSlots()
{
	int modTime = 0;

	engine.continueSaveSlot = 0;

	//READ SAVE GAME DATA
	for (int i = 0; i < 5; i++)
	{
		std::string filename = fmt::format("{}save{}.dat", engine.userHomeDirectory, i);

		std::ifstream file(filename);

		if (file.bad())
		{
			engine.saveSlot[i] = fmt::format("{} - {}", i + 1, _("Empty"));
		}
		else
		{
			PODGame tempGame;
			file.read((char *)&tempGame, sizeof tempGame);

			if (file.bad())
			{
				engine.saveSlot[i] = fmt::format("{} - {}", i + 1, _("Corrupt Save Data"));
			}
			else
			{
				engine.saveSlot[i] = fmt::format("{} - {} ({:02d}:{:02d}:{:02d})", i + 1, _(tempGame.s_stageName), tempGame.totalHours, tempGame.totalMinutes, tempGame.totalSeconds);
			}

			struct stat fileInfo;

			if (stat(filename.c_str(), &fileInfo) != -1)
			{
				if (fileInfo.st_mtime > modTime)
				{
					modTime = fileInfo.st_mtime;
					engine.continueSaveSlot = (i + 1);
				}
			}
		}
	}

	debug("Continue Save Slot = {}\n", engine.continueSaveSlot);
}

/*
Fill in later...
*/
bool loadGame(int slot)
{
	debug("Loading Game #{}...\n", slot);
	game.clear();

	std::string line;
	char string[2][1024];
	int param[2];

	std::string filename = fmt::format("{}save{}.dat", engine.userHomeDirectory, slot);

	std::ifstream file(filename);

	if (file.bad())
	{
		return false;
	}

	file.read((char *)&game, sizeof game);

	if (file.bad())
	{
		return graphics.showErrorAndExit("The save data loaded was not in the format expected", ""), false;
	}

	filename = fmt::format("{}persistant{}.dat", engine.userHomeDirectory, slot); // sic, to keep compatibility with older versions

	file.close();
	file.open(filename);

	if (file.bad())
	{
		return false;
	}

	while (std::getline(file, line))
	{
		scan(line, "%*c %[^\"] %*c %*c %[^\"] %*c %d %d", string[0], string[1], &param[0], &param[1]);

		if (param[0] == -1 && param[1] == -1)
		{
			break;
		}

		gameData.addCompletedObjective(string[0], string[1], param[0], param[1]);
	}

	std::string stageName;
	int numberOfLines = 0;
	bool complete = false;

	while (std::getline(file, stageName))
	{
		if (stageName == "@EOF@")
		{
			complete = true;
			break;
		}

		if (!std::getline(file, line))
		{
			break;
		}

		scan(line, "%d", &numberOfLines);

		debug("Read {} with {} lines.\n", stageName, numberOfLines);

		auto persistent = map.createPersistent(stageName);

		for (int i = 0; i < numberOfLines; i++)
		{
			if (!std::getline(file, line))
			{
				graphics.showErrorAndExit("Unexpected end of file reading save data", "");
			}

			auto persistData = new PersistData();

			persistData->data = line;

			//debug("Read {}: {}", i, persistData->data);

			persistent.push_back(persistData->data);
		}
	}

	if (!complete)
	{
		graphics.showErrorAndExit("Unexpected end of file reading save data", "");
	}

	debug("Loaded Successfully\n");

	return true;
}

static int confirmSave()
{
	if (game.autoSave)
	{
		return game.autoSaveSlot;
	}

	if (!engine.loadWidgets("data/saveWidgets"))
		graphics.showErrorAndExit(ERR_FILE, "data/saveWidgets");

	int slot[6], quitYes, quitNo;
	slot[0] = slot[1] = slot[2] = slot[3] = slot[4] = slot[5] = 0;
	quitYes = quitNo = 0;

	engine.setWidgetVariable("slot1", &slot[0]);
	engine.setWidgetVariable("slot2", &slot[1]);
	engine.setWidgetVariable("slot3", &slot[2]);
	engine.setWidgetVariable("slot4", &slot[3]);
	engine.setWidgetVariable("slot5", &slot[4]);
	engine.setWidgetVariable("slot6", &slot[5]);

	engine.setWidgetVariable("contyes", &quitYes);
	engine.setWidgetVariable("contno", &quitNo);

	for (int i = 0; i < 5; i++)
	{
		std::string widgetName = fmt::format("slot{}", i + 1);
		engine.getWidgetByName(widgetName)->label = engine.saveSlot[i];
	}

	engine.highlightWidget("slot1");

	int menuSound = 0;

	int rtn = -1;

	engine.showWidgetGroup("gameSlots", true);
	engine.showWidgetGroup("continueconf", false);

	graphics.setFontSize(4);
	SDL_Surface *title = graphics.quickSprite("savetitle", graphics.getString("Save Game", true));

	while (true)
	{
		graphics.updateScreen();
		SDL_FillRect(graphics.screen, nullptr, graphics.black);
		engine.getInput();
		config.populate();

		menuSound = engine.processWidgets();

		if (menuSound)
			audio.playMenuSound(menuSound);

		graphics.blit(title, 320, 100, graphics.screen, true);

		drawWidgets();

		if (slot[5])
		{
			engine.showWidgetGroup("gameSlots", false);
			engine.showWidgetGroup("continueconf", true);
			engine.highlightWidget("contno");
			drawWidgets();
			slot[5] = 0;
		}

		if (quitYes)
		{
			break;
		}

		if (quitNo)
		{
			engine.showWidgetGroup("gameSlots", true);
			engine.showWidgetGroup("continueconf", false);
			engine.highlightWidget("slot1");
			drawWidgets();
			quitNo = 0;
		}

		for (int i = 0; i < 5; i++)
		{
			if (slot[i])
			{
				rtn = i;
			}
		}

		if ((slot[0]) || (slot[1]) || (slot[2]) || (slot[3]) || (slot[4]) || (slot[5]))
		{
			break;
		}
	}

	SDL_FillRect(graphics.screen, nullptr, graphics.black);
	graphics.updateScreen();

	return rtn;
}

void saveGame()
{
	std::string message;

	SDL_FillRect(graphics.screen, nullptr, graphics.black);
	graphics.updateScreen();

	int slot = confirmSave();

	if (slot == -1)
		return;

	graphics.setFontSize(1);
	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
	message = fmt::format(_("Saving Game to Save Slot #{}. Please Wait..."), slot + 1);
	graphics.drawString(message, 320, 220, true, graphics.screen);
	graphics.updateScreen();

	std::string filename = fmt::format("{}save{}.dat", engine.userHomeDirectory, slot);

	std::ofstream file(filename);

	const auto &podgame = static_cast<PODGame>(game);
	file.write((char *)&podgame, sizeof podgame);
	file.close();

	if (file.bad())
	{
		return graphics.showErrorAndExit("File write error whilst saving game", strerror(errno));
	}

	filename = fmt::format("{}persistant{}.dat", engine.userHomeDirectory, slot); // sic, to keep compatibility with older versions

	file.open(filename);

	createPersistentMapData();

	for (auto &&objective: gameData.objectives)
	{
		for (auto &&subobjective: objective.second)
		{
			fmt::print(file, "\"{}\" \"{}\" {} {}\n", objective.first, subobjective.first, subobjective.second.current, subobjective.second.target);
		}
	}

	fmt::print(file, "\"@EOF@\" \"@EOF@\" -1 -1\n");

	for (auto &[levelName, lines]: map.persistents)
	{
		if (levelName.empty())
		{
			continue;
		}

		fmt::print(file, "{}\n", levelName);
		fmt::print(file, "{}\n", lines.size());

		for (auto &line: lines)
		{
			fmt::print(file, "{}\n", line);
		}
	}

	fmt::print(file, "@EOF@\n");
	file.close();

	if (file.bad())
	{
		return graphics.showErrorAndExit("File write error whilst saving game", "");
	}

	map.clear();

	graphics.drawString(_("Save Complete"), 320, 260, true, graphics.screen);
	graphics.updateScreen();
}
