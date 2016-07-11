#include "Play.h"
#include "../Score/ScoreEngine.h"

#include "../../utils/mathUtils.h"
#include "../osuCalc.h"

#include <algorithm>
#include <assert.h>

Play::Play(std::string _beatmapFile, std::string _replayFile)
{
	beatmap = new Beatmap(_beatmapFile);
	replay = new Replay(_replayFile);

	if(beatmap ->getGamemode() != GAMEMODE::GAMEMODE_ERROR)
		ProcessBeatmap();

	scoreEngine = new ScoreEngine(this);

	/*  \TODO: Implement ability to compare beatmap MD5 hash to validate replay
		if(beatmap->getMD5() != replay->getBeatmapMD5())
			delete replay;
	*/
}

Play::~Play()
{
	if (beatmap != nullptr)
		delete beatmap;

	if (replay != nullptr)
		delete replay;

	if (scoreEngine != nullptr)
		delete scoreEngine;
}

void Play::ProcessBeatmap()
{
	if (beatmap->getGamemode() != GAMEMODE::GAMEMODE_ERROR)
	{
		this->ResetMods();
			this->ApplyAR();
			this->ApplyCS();
			this->ApplyOD();
			this->ApplyVisual();
			this->ApplyTimings();
		this->beatmap->Process();
	}
}

void Play::LoadBeatmap(std::string _beatmapFile)
{
	if (beatmap != nullptr)
		delete beatmap;
	beatmap = new Beatmap(_beatmapFile);

	ProcessBeatmap();
}

void Play::LoadReplay(std::string _replayFile)
{
	if (replay != nullptr)
		delete replay;

	if (scoreEngine != nullptr)
		delete scoreEngine;

	ProcessBeatmap();
	scoreEngine = new ScoreEngine(this);
}

void Play::ResetMods()
{
	this->beatmap->modTimingPoints = this->beatmap->origTimingPoints;
	this->beatmap->modifiedDiff = this->beatmap->origDiff;


	for (Hitobject* hitobject : this->beatmap->origHitobjects)
		beatmap->modHitobjects.push_back(*hitobject);

	/*this->beatmap->modHitobjects.resize(3);
	for (int i=0; i<this->beatmap->modHitobjects.size(); i++)
	{
		memcpy(&beatmap->modHitobjects[i], &beatmap->origHitobjects[i], sizeof(Hitobject));	
	}*/
	
}

// ------------- [PRIVATE] ----------------


void Play::setMods()
{

}

void Play::ApplyAR()
{
	this->beatmap->modifiedDiff.ar = this->beatmap->origDiff.ar;
	if (replay == nullptr)
		return;

	double &ar = this->beatmap->modifiedDiff.ar;

	if (replay->HasMod(MODS::EZ))
	{
		ar *= 0.5;
	}

	if (replay->HasMod(MODS::HR))
	{
		// make sure ar doesnt go over 10
		if (ar*1.4 < 10) ar *= 1.4;
		else			 ar = 10;
	}

	if (replay->HasMod(MODS::HT))  ar = ms2AR(AR2ms(ar) / 0.75);
	if (replay->HasMod(MODS::DT))  ar = std::min(ms2AR(AR2ms(ar) / 1.5), 11.0);

	// for old maps which had AR as part as OD
	if (this->beatmap->origDiff.ar == -1)
	{
		this->beatmap->origDiff.ar = this->beatmap->origDiff.od;
		this->beatmap->modifiedDiff.ar = this->beatmap->modifiedDiff.od;
	}
}

void Play::ApplyCS()
{
	this->beatmap->modifiedDiff.cs = this->beatmap->origDiff.cs;
	if (replay == nullptr)
		return;

	double &cs = this->beatmap->modifiedDiff.cs;

	if (replay->HasMod(MODS::EZ)) cs *= 0.5;
	if (replay->HasMod(MODS::HR)) cs *= 1.3;
}

void Play::ApplyOD()
{
	this->beatmap->modifiedDiff.od = this->beatmap->origDiff.od;
	if (replay == nullptr)
		return;

	double &od = this->beatmap->modifiedDiff.od;

	if (replay->HasMod(MODS::EZ))
	{
		od *= 0.5;
	}

	if (replay->HasMod(MODS::HR))
	{
		// make sure od doesnt go over 10
		if (od*1.4 < 10) od *= 1.4;
		else			 od = 10;
	}

	if (replay->HasMod(MODS::HT))
	{
		//od *= 0.75;
	}

	if (replay->HasMod(MODS::DT))
	{
		//od *= 1.5;
	}
}

void Play::ApplyTimings()
{
	double divisor = 1;

	if (replay != nullptr)
	{
		if (replay->HasMod(MODS::HT)) divisor = 0.75;
		if (replay->HasMod(MODS::DT)) divisor = 1.5;
	}
	
	// Mod timing points
	for (auto &tp : this->beatmap->modTimingPoints)
	{
		if (tp.beatInterval > 0)
			tp.beatInterval /= divisor;
		tp.offset = ceil((double)tp.offset / divisor);
	}

	// Mod hitobject timings
	for (auto &hitobject : this->beatmap->modHitobjects)
	{
		hitobject.time /= divisor;
		int type = hitobject.getHitobjectType();

		if (type == HITOBJECTYPE::MANIALONG || type == HITOBJECTYPE::SLIDER || type == HITOBJECTYPE::SPINNER)
			hitobject.slider.endTime /= divisor;
	}

	// Mod replay stream timings
	for (auto &frame : replay->replayStream)
	{
		std::get<0>(frame) /= divisor;
	}
}

void Play::ApplyVisual()
{
	if (replay == nullptr)
		return;

	if (replay->HasMod(MODS::HD))
	{
		beatmap->origMod.hidden = true;
		beatmap->modifiedMod.hidden = true;
	}

	if (replay->HasMod(MODS::FL))
	{
		beatmap->origMod.flashlight = true;
		beatmap->modifiedMod.hidden = true;
	}

	if (replay->HasMod(MODS::FI))
	{
		beatmap->origMod.fadeIn = true;
		beatmap->modifiedMod.hidden = true;
	}

	if (replay->HasMod(MODS::HR))
	{
		for (auto &hitobject : this->beatmap->modHitobjects)
		{

		}
	}
}