#ifndef PLAY_H
#define PLAY_H

#include "Beatmap.h"
#include "Replay.h"

class ScoreEngine;

class Play
{
	public:
		Play(std::string _beatmapFile, std::string _replayFile = "");
		virtual ~Play();

		Beatmap *beatmap;
		Replay  *replay;
		ScoreEngine *scoreEngine;

		void LoadBeatmap(std::string _beatmapFile);
		void LoadReplay(std::string _replayFile);

		void ResetMods();

	private:
		void ValidateMods();
		void ProcessBeatmap();

		void setMods();

		void ApplyAR();
		void ApplyCS();
		void ApplyOD();
		void ApplyVisual();
		void ApplyTimings();
};

#endif