#include "Play.h"
#include "../Score/ScoreEngine.h"

#include "../../utils/mathUtils.h"
#include "../osuCalc.h"

#include "../Filestructure/SliderHitObject.h"

#include <algorithm>
#include <assert.h>

Play::Play()
{

	replay = new Replay();
	scoreEngine = new ScoreEngine();

	activeMods = custom;

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
	if (beatmap != nullptr)
		delete beatmap;
	
	beatmap = new Beatmap();
	beatmap->Read(_beatmapFile);

	// Make sure the beatmap is valid before continuing
	if (!beatmap->isValid()) return;
	beatmap->ResetModified();

	setMods(BEATMAP);
	beatmap->Process();
}

/// \TODO: Perhaps split this off to another thread
void Play::LoadReplay(std::string _replayFile)
{
	if (replay != nullptr)
		delete replay;

	replay = new Replay();
	replay->ProcessReplay(_replayFile, beatmap);

	// v3 and v4 maps have their offsets adjusted +24 ms for some reason
	if ((beatmap->metadata.format == 3) || (beatmap->metadata.format == 4))
	{
		for (osu::TIMING& timing : replay->replayStream)
			timing.time += 24;
	}

	// Make sure the replay is valid before continuing
	if (!replay->isValid())	return;

	beatmap->ResetModified();
	setMods(REPLAY);
	beatmap->Process();
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
	beatmap->ResetModified();
	this->ApplyVisual();
	this->ApplyTimings();
}

void Play::ApplyTimings()
{
	// Get the modded divisor (bpm speed factor)
	double divisor = activeMods.getTM();
	int adjustedOffset = 0;
	
	// v3 and v4 maps have their offsets adjusted +24 ms for some reason
	if ((beatmap->metadata.format == 3) || (beatmap->metadata.format == 4))
		adjustedOffset = 24;

	// Mod timing points
	for (auto &tp : this->beatmap->modTimingPoints)
	{
		if (tp.beatInterval > 0)
			tp.beatInterval /= divisor;
		tp.offset = ceil((double)tp.offset / divisor) + (double)adjustedOffset;
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
		// flip on the x axis
		for (auto &hitobject : this->beatmap->modHitobjects)
		{
			// Flip hitobject's position
			hitobject->setPos(irr::core::vector2d<double>(hitobject->getPos().X, 384 - hitobject->getPos().Y));
			
			// Flip slider points
			if (hitobject->isHitobjectLong())
			{
				SliderHitObject* slider = hitobject->getSlider();
				for (irr::core::vector2di& point : slider->readCurvePoints)
					point.Y = 384 - point.Y;
			}
		}
	}
}