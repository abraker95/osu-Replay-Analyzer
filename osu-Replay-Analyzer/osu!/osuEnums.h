#ifndef OSU_ENUMS
#define OSU_ENUMS

enum GAMEMODE
{
	OSU_STANDARD = 0,
	OSU_TAIKO = 1,
	OSU_CATCH = 2,
	OSU_MANIA = 3,
	OSU_DODGE = 4,
	GAMEMODE_ERROR
};

/// \TODO: The Mods class takes care of this, so get rid of this
enum MODS
{
	// non
	NF = 1 << 0,
	EZ = 1 << 1,
	// video mod no longer exists
	HD = 1 << 3,
	HR = 1 << 4,
	SD = 1 << 5,
	DT = 1 << 6,
	RL = 1 << 7,
	HT = 1 << 8,
	NC = 1 << 9,
	FL = 1 << 10,
	AU = 1 << 11,
	SO = 1 << 12,
	AP = 1 << 13,
	PF = 1 << 14,
	K4 = 1 << 15,
	K5 = 1 << 16,
	K6 = 1 << 17,
	K7 = 1 << 18,
	K8 = 1 << 19,
	FI = 1 << 20,
	RD = 1 << 21,
	// Cinema
	// Taget practice
	K9 = 1 << 24,
	CP = 1 << 25,
	// ???
};

enum HITOBJECTYPE
{
	CIRCLE    = 1 << 0,
	SLIDER    = 1 << 1,
	NCOMBO    = 1 << 2,
	SPINNER   = 1 << 3,
	// ???
	MANIALONG = 1 << 7
};

namespace HITOBJECTNORMAL
{
	enum
	{
		XPOS = 0,
		YPOS = 1,
		TIME = 2,
		TYPE = 3,
		HITSOUND = 4,
		ADDITION = 5
	};
};

namespace HITOBJECTSLIDER
{
	enum 
	{
		XPOS = 0,
		YPOS = 1,
		TIME = 2,
		TYPE = 3,
		HITSOUND = 4,
		SLIDERTYPE = 5,
		PIXELENGTH = 6,
		ADDITION = 5
	};
};

namespace HITOBJECTSPINNER
{
	enum 
	{
		XPOS = 0,
		YPOS = 1,
		TIME = 2,
		TYPE = 3,
		HITSOUND = 4,
		ENDTIME = 5,
		ADDITION = 6
	};
};
#endif