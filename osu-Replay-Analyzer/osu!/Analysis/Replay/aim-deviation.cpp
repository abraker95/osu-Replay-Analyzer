#include "aim-deviation.h"
#include "../../../utils/geometry.h"

#include "../AnalysisStruct.h"
#include "../Replay/tap-deviation.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

Analyzer_AimDeviation::Analyzer_AimDeviation() : Analyzer("aim deviation (px)") {}
Analyzer_AimDeviation::~Analyzer_AimDeviation() {}

void Analyzer_AimDeviation::AnalyzeStd(Play* _play)
{
	if (_play->replay->isValid() == false) return;
	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();

	std::vector<osu::TIMING>& tapDeviations = *AnalysisStruct::beatmapAnalysis.getAnalyzer("tap deviation (ms)")->getData();
	osu::TIMING timing;
		timing.data = 0;

	for(osu::TIMING tapDeviation : tapDeviations)
	{
		if (tapDeviation.data == INT_MAX) continue;   // skip tapped misses
		if (tapDeviation.press != true) continue; // don't record non presses

		// find the object
		int i = OSUSTANDARD::FindHitobjectAt(hitobjects, tapDeviation.time, tapDeviation.data <= 0);
		vector2d<double> posTarget, posPlayer;

		// Iterate on sliders
		if (hitobjects[i]->isHitobjectLong())
		{
			std::vector<osu::TIMING>& const replayStream = _play->replay->getReplayStream();
			int replayIndex = osu::FindTimingAt(replayStream, tapDeviation.time);
			
			while (replayStream[replayIndex++].time < hitobjects[i]->getEndTime())
			{
				// Stop recording if key is released
				//if (replayStream[replayIndex].press == false)	break;

				posTarget = hitobjects[i]->getSlider()->GetSliderPos(replayStream[replayIndex].time);
				posPlayer = vector2d<double>(replayStream[replayIndex].pos.X, replayStream[replayIndex].pos.Y);

				// record timing data
				timing.data = getDist(posTarget, posPlayer);
				timing.pos = vector2df(posTarget.X, posTarget.Y);
				timing.press = true;
				timing.time = replayStream[replayIndex].time;

				data.push_back(timing);
			}

			continue;
		}

		posTarget = hitobjects[i]->getPos();
		posPlayer = vector2d<double>(tapDeviation.pos.X, tapDeviation.pos.Y);

		// record timing data
		timing.data = getDist(posTarget, posPlayer);
		timing.pos = vector2df(posTarget.X, posTarget.Y);
		timing.press = true;
		timing.time = tapDeviation.time;

		data.push_back(timing);
	}
}

void Analyzer_AimDeviation::AnalyzeCatch(Play* _play)
{

}

void Analyzer_AimDeviation::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_AimDeviation::AnalyzeMania(Play* _play)
{

}

void Analyzer_AimDeviation::AnalyzeDodge(Play* _play)
{

}