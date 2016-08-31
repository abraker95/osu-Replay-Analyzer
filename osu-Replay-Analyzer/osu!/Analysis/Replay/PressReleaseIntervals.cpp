#include "PressReleaseIntervals.h"

#include "../../../utils/geometry.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

#include "../AnalysisStruct.h"

Analyzer_PressReleaseIntervals::Analyzer_PressReleaseIntervals() : Analyzer("Press-Release Intervals (ms)") {}
Analyzer_PressReleaseIntervals::~Analyzer_PressReleaseIntervals() {}

void Analyzer_PressReleaseIntervals::AnalyzeStd(Play* _play)
{
if (!_play->replay->isValid())	return;

	std::vector<osu::TIMING>& replaystream = _play->replay->getReplayStream();
	int KEYS = _play->beatmap->getMods().getCS();

	std::vector<long> pressTimes;	pressTimes.resize(4);
	for (long& pressTime : pressTimes)	pressTime = -1;

	int minInterval = INT_MAX;

	osu::TIMING timing;
		timing.data = 0;

	for (int i=0; i<replaystream.size() - 1; i++)
	{
		for (int key = 0; key < 4; key++)
		{
			int pressState = OSUMANIA::getButtonState(replaystream[i].pos.X, replaystream[i + 1].pos.X, key);

			bool press   = ((pressState & 1) != 0);
			bool release = ((pressState & 2) != 0);

			// Only look at key release or key press events
			if (!press && !release) continue;

			if (press == true)
			{
				// If this triggers, some replay hacking might be going on
				//if (pressTimes[key] != INT_MIN)

				pressTimes[key] = replaystream[i].time;
			}
			else if (release == true)
			{
				int interval = replaystream[i].time - pressTimes[key];
				if (minInterval > interval) minInterval = interval;

				timing.time = pressTimes[key];
				timing.data = replaystream[i].time - pressTimes[key];
				timing.key = minInterval;

				data.push_back(timing);

				pressTimes[key] = INT_MIN;
			}
		}
	}
}

void Analyzer_PressReleaseIntervals::AnalyzeCatch(Play* _play){}

void Analyzer_PressReleaseIntervals::AnalyzeTaiko(Play* _play){}

void Analyzer_PressReleaseIntervals::AnalyzeMania(Play* _play)
{
	if (!_play->replay->isValid())	return;

	std::vector<osu::TIMING>& replaystream = _play->replay->getReplayStream();
	int KEYS = _play->beatmap->getMods().getCS();

	std::vector<long> pressTimes;	pressTimes.resize(KEYS);
	for (long& pressTime : pressTimes)	pressTime = -1;

	osu::TIMING timing;
		timing.data = 0;

	for (int i=0; i<replaystream.size() - 1; i++)
	{
		for(int key = 0; key<KEYS; key++)
		{
			int pressState = OSUMANIA::getButtonState(replaystream[i].pos.X, replaystream[i + 1].pos.X, key);

			bool press   = ((pressState & 1) != 0);
			bool release = ((pressState & 2) != 0);

			// Only look at key release or key press events
			if (!press && !release) continue;

			if (press == true)
			{
				// If this triggers, some replay hacking might be going on
				//if (pressTimes[key] != INT_MIN)

				pressTimes[key] = replaystream[i].time;
			}
			else if (release == true)
			{
				timing.time = pressTimes[key];
				timing.data = replaystream[i].time - pressTimes[key];
				timing.key = key;

				data.push_back(timing);

				pressTimes[key] = INT_MIN;
			}
		}
	}
}

void Analyzer_PressReleaseIntervals::AnalyzeDodge(Play* _play)
{

}