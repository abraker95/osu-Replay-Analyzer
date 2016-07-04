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
	if (beatmap->metadata.format != -1)
	{
		if (replay != nullptr)
		{
			this->ApplyAR();
			this->ApplyCS();
			this->ApplyOD();
			this->ApplyVisual();
			this->ApplyTimings();
			this->beatmap->Process();
		}
	}
}

// ------------- [PRIVATE] ----------------

void Play::ApplyAR()
{
	double &ar = this->beatmap->origDiff.ar;

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
}

void Play::ApplyCS()
{
	double &cs = this->beatmap->origDiff.cs;

	if (replay->HasMod(MODS::EZ)) cs *= 0.5;
	if (replay->HasMod(MODS::HR)) cs *= 1.3;
}

void Play::ApplyOD()
{
	double &od = this->beatmap->origDiff.od;

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
	std::vector<TimingPoint> &timingpoints = beatmap->timingPoints;
	double divisor = 1;

	if (replay->HasMod(MODS::HT)) divisor = 0.75;
	if (replay->HasMod(MODS::DT)) divisor = 1.5;

	for (auto &tp : timingpoints)
	{
		if (tp.beatInterval > 0)
			tp.beatInterval /= divisor;
		tp.offset = ceil((double)tp.offset / divisor);
	}

	for (auto &hitobject : beatmap->hitObjects)
	{
		hitobject->time /= divisor;
		int type = hitobject->getHitobjectType();

		if (type == HITOBJECTYPE::MANIALONG || type == HITOBJECTYPE::SLIDER || type == HITOBJECTYPE::SPINNER)
			hitobject->slider->endTime /= divisor;
	}

	for (auto &frame : replay->replayStream)
	{
		std::get<0>(frame) /= divisor;
	}
}

void Play::ApplyVisual()
{
	if (replay->HasMod(MODS::HD))
	{
		beatmap->origMod.hidden = true;
	}

	if (replay->HasMod(MODS::FL))
	{
		beatmap->origMod.flashlight = true;
	}

	if (replay->HasMod(MODS::FI))
	{
		beatmap->origMod.fadeIn = true;
	}

	if (replay->HasMod(MODS::HR))
	{
		// \TODO: HR mod flips notes
	}
}