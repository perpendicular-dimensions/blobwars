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

#include <cassert>
#include <cerrno>
#include <clocale>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <istream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <streambuf>
#include <thread>
#include <vector>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <yaml-cpp/yaml.h>

#include <zlib.h>

#ifdef FRAMEWORK_SDL
#include <SDL/SDL.h>
#include <SDL_image/SDL_image.h>
#include <SDL_mixer/SDL_mixer.h>
#include <SDL_ttf/SDL_ttf.h>
#include <SDL_net/SDL_net.h>
#include <CoreFoundation/CoreFoundation.h>
extern C_LINKAGE void openHelpURL();
#ifndef SDL_GetGamma
extern DECLSPEC int SDLCALL SDL_GetGamma(float *red, float *green, float *blue);
#endif
#else
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_net.h"
#endif

#if !defined(_WIN32) && !defined(__APPLE__)
#include <libintl.h>
#define _(x) gettext(x)
#else
#define _(x) (x)
#define bindtextdomain(x, y) while (false)
#define textdomain(x) while (false)
#endif

#include "defs.h"
#include "utils.h"

#include "CMath.h"

#include "CSprite.h"
#include "CData.h"
#include "CParticle.h"
#include "CWeapon.h"
#include "CEntity.h"
#include "CBoss.h"
#include "CTrain.h"
#include "CSpawnPoint.h"
#include "CSwitch.h"
#include "CEffect.h"
#include "CObjective.h"
#include "CTeleporter.h"
#include "CLineDef.h"
#include "CTrap.h"

#include "CPersistData.h"

#include "CMap.h"

#include "CCollision.h"

#include "CWidget.h"

#include "CFileData.h"
#include "CPak.h"

#include "CJoystick.h"
#include "CKeyboard.h"

#include "CEngine.h"
#include "CGraphics.h"
#include "CAudio.h"

#include "CGame.h"
#include "CGameData.h"

#include "CHub.h"
#include "CRadarBlip.h"

#include "CCutscene.h"

#include "CReplayDataHeader.h"
#include "CReplayData.h"
#include "CConfig.h"
#include "CMedalServer.h"

#include "aquaBoss.h"
#include "bosses.h"
#include "bullets.h"
#include "cutscene.h"
#include "droidBoss.h"
#include "effects.h"
#include "enemies.h"
#include "entities.h"
#include "explosions.h"
#include "finalBattle.h"
#include "galdov.h"
#include "game.h"
#include "graphics.h"
#include "hub.h"
#include "info.h"
#include "init.h"
#include "intro.h"
#include "items.h"
#include "lineDefs.h"
#include "loadSave.h"
#include "main.h"
#include "map.h"
#include "mapData.h"
#include "mias.h"
#include "mission.h"
#include "objectives.h"
#include "obstacles.h"
#include "options.h"
#include "pak.h"
#include "particles.h"
#include "player.h"
#include "resources.h"
#include "spawnPoints.h"
#include "switches.h"
#include "tankBoss.h"
#include "teleporters.h"
#include "title.h"
#include "trains.h"
#include "traps.h"
#include "triggers.h"
#include "weapons.h"
#include "widgets.h"
