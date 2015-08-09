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

// Read in the define list used by the map data also
#include "defines.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define PI 3.14159265359

/* ############## errors ########### */

#define ERR_FILE 		"A required file (%s) was not found in PAK file"
#define ERR_LOCALE 	"Locale information missing for section '%s' (contains only partial translation)"

/* ########### maximums ############ */

#define MAX_SOUNDS 		75
#define MAX_TILES 		256
#define MAX_WEAPONS 	25
#define MAX_ITEMS 		25
#define MAX_ENEMIES		25

#define MAX_HEALTH		10

#define	MAX_FPS			62

/* ########### dead flags ############ */

#define	DEAD_ALIVE	0
#define	DEAD_DYING	1
#define	DEAD_DEAD	2

/* ########### game sections ######## */

#define SECTION_INTRO 		0
#define SECTION_TITLE 		1
#define SECTION_HUB 		2
#define SECTION_GAME		3
#define SECTION_GAMEOVER	4
#define SECTION_CREDITS		5
#define SECTION_EASYOVER	6

/* ############ sounds ############ */



enum {
	
	SND_WATERIN,
	SND_WATEROUT,
	SND_ITEM,
	SND_PISTOL,
	SND_RICO1,
	SND_RICO2,
	SND_MACHINEGUN,
	SND_DEATH1,
	SND_DEATH2,
	SND_DEATH3,
	SND_GRENADE,
	SND_ROCKET,
	SND_FIREBALL,
	SND_HIT,
	SND_SPLAT,
	SND_TELEPORT1,
	SND_TELEPORT2,
	SND_TELEPORT3,
	SND_GETWEAPON,
	SND_LASER,
	SND_ELECDEATH1,
	SND_ELECDEATH2,
	SND_ELECDEATH3,
	SND_LOCKEDDOOR,
	SND_OPENDOOR,
	SND_SWITCH1,
	SND_SWITCH2,
	SND_DROWNING,
	SND_AMBIANCE,
	SND_GULP,
	SND_EAT,
	SND_THROW,
	SND_GRBOUNCE,
	SND_DOOROPENED,
	SND_SPREADGUN,
	SND_ELECTRICITY1,
	SND_ELECTRICITY2,
	SND_ELECTRICITY3,
	SND_CLANG,
	SND_FIRECRACKLE,
	SND_CLOCK,
	SND_BOSSCUSTOM1,
	SND_BOSSCUSTOM2,
	SND_BOSSCUSTOM3,
	SND_BOSSCUSTOM4,
	SND_BOSSCUSTOM5,
	SND_POP,
	SND_STONEBREAK,
	SND_CHEAT = MAX_SOUNDS - 3,
	SND_HIGHLIGHT = MAX_SOUNDS - 2,
	SND_SELECT = MAX_SOUNDS - 1

};

/* ########### channels ########### */

#define CH_ANY			-1
#define CH_WEAPON		0
#define CH_ITEM			1
#define CH_EXPLODE		3
#define CH_DEATH		4
#define CH_TOUCH		5
#define CH_AMBIANCE		6
#define CH_SPAWN		7

/* ########## keyboard ############# */

struct CONTROL
{
	enum TYPE
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		FIRE,
		JUMP,
		MAP,
		PAUSE,
		JETPACK,
		MAX
	};
};

/* ############ replay ############ */

struct REPLAY_MODE
{
	enum TYPE
	{
		NONE,
		RECORD,
		PLAYBACK
	};
};

/* ########### widgets ############ */

const char widgetName[][25] = {

	"BUTTON",
	"RADIO",
	"SMOOTH_SLIDER",
	"SLIDER",
	"LABEL",
	"KEYBOARD",
	"JOYPAD",
	"-1"

};

#define WG_BUTTON				0
#define WG_RADIO				1
#define WG_SMOOTH_SLIDER 		2
#define WG_SLIDER				3
#define WG_LABEL 				4
#define WG_KEYBOARD				5
#define WG_JOYPAD 				6

/* ############## skill ############# */

const char skill[][10] = {

	"Easy",
	"Normal",
	"Hard",
	"Extreme"

};

/* ############## text ############## */

#define TXT_LEFT 		0
#define TXT_CENTERED 	1
#define TXT_RIGHT 		2

#define INFO_NORMAL			0
#define INFO_OBJECTIVE		1
#define INFO_HINT			2
#define INFO_ACTIVATE		3
#define INFO_BAD			4

/* ########## weapons ############# */

#define WP_PISTOL 			0
#define WP_MACHINEGUN 		1
#define WP_LASER 			2
#define WP_GRENADES 		3
#define WP_SPREAD 			4

#define WP_ROCKETS 			5
#define WP_PLASMARIFLE 		6
#define WP_FLAMETHROWER 	7
#define WP_ICEGUN 			8
#define WP_LAVABALL1		9
#define WP_LAVABALL2		10
#define WP_AIMEDPISTOL		11
#define WP_ALIENSPREAD		12
#define WP_SHELLS			13
#define WP_ROCK1			14
#define WP_STALAGTITE		15
#define WP_BOMB				16
#define WP_ALIENLASER		17
#define WP_ALIENGRENADE		18
#define WP_AIMEDMACHINE		19

/* ########### entity defs ######### */

#define OBJ_UP 		0
#define OBJ_DOWN 	1
#define OBJ_LEFT 	2
#define OBJ_RIGHT 	3

#define ENV_AIR 	0
#define ENV_WATER 	1
#define ENV_SLIME 	2
#define ENV_LAVA 	3

#define PLAYER_WALK_SPEED	4
#define PLAYER_FLY_SPEED	8
#define PLAYER_JUMP_SPEED 	-10.25

/* ######### paticle flags ########### */

#define PAR_WEIGHTLESS	1
#define PAR_COLLIDES	2
#define PAR_LIQUID      4

/* ######### effect flags ########### */

#define EFF_BLEEDS		1
#define EFF_TRAILSFIRE	2
#define EFF_SMOKES		4
#define EFF_WEIGHTLESS	8
#define EFF_COLORED		16

/* ########## item defs ############ */

enum {

	ITEM_PISTOL,
	ITEM_MACHINEGUN,
	ITEM_LASER,
	ITEM_GRENADES,
	ITEM_SPREAD,
	ITEM_CHERRY,
	ITEM_DOUBLECHERRY,
	ITEM_TRIPLECHERRY,
	ITEM_POINTS,
	ITEM_POINTS2,
	ITEM_POINTS3,
	ITEM_POINTS4,
	ITEM_POINTS5,
	ITEM_POINTS6,
	ITEM_POINTS7,
	ITEM_MISC = 100,
	ITEM_MISC_NOSHOW,
	ITEM_MISC_INVISIBLE

};

/* ########### map ############ */

#define MAPWIDTH  	400
#define MAPHEIGHT 	300
#define BRICKSIZE  	32
#define BRICKSHIFT	5

#define OFFSETX 320
#define OFFSETY 240

#define MAP_AIR 		0
#define MAP_WATER 		1
#define MAP_SLIME 		2
#define MAP_LAVA 		3
#define MAP_BREAKABLE	4
#define MAP_BREAKABLE2	8
#define MAP_SOLID		9
#define MAP_NORESET		115
#define MAP_DECORATION	120
#define MAP_EXITSIGN	196
#define MAP_WATERANIM	200
#define MAP_SLIMEANIM	207
#define MAP_LAVAANIM	214
#define MAP_TOPLAYER	221

#define MAP_AIR_WALL_1	127
#define MAP_AIR_WALL_2	129
#define MAP_AIR_CEILING_1	242
#define MAP_AIR_WALL_3	243
#define MAP_AIR_WALL_4	244
#define MAP_AIR_CEILING_2	247

#define MAP_WATER_WALL	246

#define MAP_SHAKEAMOUNT	2

/* ########### mia stuff ############## */

#define MIAPERCENT 0.70

/* ########### mission stuff ############ */

#define MIS_INPROGRESS		0
#define MIS_COMPLETE		1
#define MIS_PLAYEROUT		2
#define MIS_PLAYERDEAD		3
#define MIS_PLAYERQUIT		4
#define MIS_PLAYERESCAPE	5
#define MIS_GAMECOMPLETE	6
#define MIS_TIMEUP			7
#define MIS_PLAYERRESTART	8

/* ########### pak file stuff ######## */

#define PAK_MAX_FILENAME	60

#ifndef USEPAK
	#define USEPAK 1
#endif
#ifndef PAKLOCATION
	#define PAKLOCATION ""
#endif
#ifndef PAKNAME
	#define PAKNAME "blobwars.pak"
#endif
#ifndef PAKFULLPATH
	#define PAKFULLPATH PAKLOCATION PAKNAME
#endif
#ifndef PATH_MAX
	#define PATH_MAX	4096
#endif
#ifndef GAMEPLAYMANUAL
	#define GAMEPLAYMANUAL "manual.html"
#endif
#ifndef MEDAL_SERVER_HOST
	#define MEDAL_SERVER_HOST "localhost"
#endif
#ifndef MEDAL_SERVER_PORT
	#define MEDAL_SERVER_PORT 80
#endif

enum {

	PAK_IMG,
	PAK_SOUND,
	PAK_MUSIC,
	PAK_DATA,
	PAK_FONT,
	PAK_TAGS
};

/* ######### miscellaneous ############## */

#define DIR_X 1
#define DIR_Y 2
#define DIR_XY 3

/* ############# debug ################## */

#if DEBUG
	#define debug(x) {printf("*** DEBUG: "); printf x;}
#else
	#define debug(x) {}
#endif

#ifndef IGNORE_FLAGTOKEN_ERRORS
	#define IGNORE_FLAGTOKEN_ERRORS 0
#endif

