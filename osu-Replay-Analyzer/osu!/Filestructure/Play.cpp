#include "Play.h"
#include "../Score/ScoreEngine.h"

#include "../../utils/mathUtils.h"
#include "../osuCalc.h"

#include "../Filestructure/SliderHitObject.h"

#include <algorithm>
#include <assert.h>

Play::Play()
{
	beatmap = new Beatmap();
	replay = new Replay();
	scoreEngine = new ScoreEngine();

	activeMods = custom;

	//ProcessBeatmap();

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

/// \TODO: Perhaps split this off to another thread
void Play::LoadBeatmap(std::string _beatmapFile)
{
	beatmap->ClearObjects();
	beatmap->Read(_beatmapFile);

	if (!beatmap->isValid())
		return;

	ResetTimings();
	setMods(BEATMAP);
	ProcessBeatmap();
}

void Play::ProcessBeatmap()
{
	if (!beatmap->isValid())
		return;

	this->beatmap->Process();
}

/// \TODO: Perhaps split this off to another thread
void Play::LoadReplay(std::string _replayFile)
{
	replay->ProcessReplay(_replayFile, beatmap);

	if (!replay->isValid())
		return;

	ResetTimings();
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
	{
		if (hitobject->isHitobjectLong())
			beatmap->modHitobjects.push_back(new SliderHitObject(*hitobject->getSlider()));			
		else
			beatmap->modHitobjects.push_back(new Hitobject(*hitobject));
	}
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
	for (auto hitobject : this->beatmap->modHitobjects)
	{
		hitobject->time /= divisor;

		if (hitobject->isHitobjectLong())
			hitobject->getSlider()->endTime /= divisor;
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
			hitobject->setPos(irr::core::vector2d<double>(hitobject->getPos().Y, hitobject->getPos().X));
	}
}