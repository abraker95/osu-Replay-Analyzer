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
	if (!beatmap->isValid())	
		return;

	this->beatmap->Process();
}

void Play::LoadBeatmap(std::string _beatmapFile)
{
	if (beatmap != nullptr)
		delete beatmap;
	beatmap = new Beatmap(_beatmapFile);

	ProcessBeatmap();
	ResetTimings();
	setMods(BEATMAP);
}

void Play::LoadReplay(std::string _replayFile)
{
	if (replay != nullptr)
		delete replay;

	if (scoreEngine != nullptr)
		delete scoreEngine;

	replay->ProcessReplay(_replayFile, beatmap);
	setMods(REPLAY);
	scoreEngine->genAccTimings(this);
}

void Play::setMods(ModSource _source)
{
	currModSource = _source;
	prevMods = activeMods;

	/// \TODO: Prevent chnanging certain mod on specific gamemodes (like mania and CS)
	switch (_source)
	{
		case BEATMAP:
			activeMods = beatmap->getMods();
			break;

		case REPLAY:
			activeMods = replay->getMods();
			break;

		case CUSTOM:
			activeMods = this->custom;
			break;

		default:
			break;
	}

	ApplyMods();
}

Mods* Play::getMod()
{
	return &activeMods;
}

// ------------- [PRIVATE] ----------------

void Play::ApplyMods()
{
	this->ApplyVisual();

	if (prevMods.getTM() != activeMods.getTM())
	{
		this->ResetTimings();
		this->ApplyTimings();
	}
}

void Play::ResetTimings()
{
	// reset diffs, timingpoints, and modified hitobjects
	this->beatmap->ResetModified();
	this->beatmap->modTimingPoints = this->beatmap->origTimingPoints;

	for (Hitobject* hitobject : this->beatmap->origHitobjects)
		beatmap->modHitobjects.push_back(*hitobject);
}


void Play::ApplyTimings()
{
	double divisor = activeMods.getTM();

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
}

void Play::ApplyVisual()
{
	/// \TODO: Hitcircles, etc are to have the following modifiable attributes:
	///				 FI	 FIR			 FO	  FOR    					 H
	///	  ------------O---|---------------O---|--------------------------X
	///
	/// H - hit time
	/// FO - fade out time offset relative to hit time
	/// FOR - fade out rate (1.0 max; %/ms)
	/// FI - fade in time offset relative to fade out time
	/// FIR - fade in rate (1.0 max; %/ms)
	/// 

	if (activeMods.getModifier().HD)
	{
		/// \TODO
	}

	if (activeMods.getModifier().FL)
	{
		/// \TODO
	}

	if (activeMods.getModifier().FI)
	{
		/// \TODO
	}

	if (activeMods.getModifier().HR)
	{
		// flip on the diagonal x-y axis
		for (auto &hitobject : this->beatmap->modHitobjects)
			hitobject.setPos(irr::core::vector2d<double>(hitobject.getPos().Y, hitobject.getPos().X));
	}
}