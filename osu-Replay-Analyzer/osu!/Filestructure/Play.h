#ifndef PLAY_H
#define PLAY_H

#include "Beatmap.h"
#include "Replay.h"

class ScoreEngine;

class Play
{
	public:
		enum ModSource
		{
			BEATMAP,
			REPLAY,
			CUSTOM
		};

		Play();
		virtual ~Play();

		Beatmap *beatmap;
		Replay  *replay;
		ScoreEngine *scoreEngine;
		Mods custom;

		void LoadBeatmap(std::string _beatmapFile);
		void LoadReplay(std::string _replayFile);
		
		void setMods(ModSource _source);
		Mods* getMod();

	private:
		
		void ValidateMods();
		void ProcessBeatmap();

		void ResetTimings();
		void ApplyMods();
		void ApplyVisual();
		void ApplyTimings();

		Mods activeMods, prevMods;
		ModSource currModSource;
};

#endif