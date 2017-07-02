#include "ReadingIntensity.h"

#include "../../osuCalc.h"
#include "../../osu_standard.h"
#include "../../../utils/geometry.h"

Analyzer_ReadingIntensity::Analyzer_ReadingIntensity() : Analyzer("Potential Reading Intensity (db)"){}
Analyzer_ReadingIntensity::~Analyzer_ReadingIntensity(){}

void Analyzer_ReadingIntensity::AnalyzeStd(Play* _play)
{
	Database<Hitobject>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
		timing.data = 0;

	long time = 0;
	double ARms = AR2ms(_play->getMod()->getAR());

	osu::TIMING prevTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, time, &time);
	osu::TIMING currTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, time, &time);

	while(true)
	{
		double readingRate = (currTickPoint.time - prevTickPoint.time) / ARms;
		readingRate = MAX(0.0, -10 * log(readingRate));  // This is not the actual reading intensity. It's the intensity if the pattern is unpredictable (overlaps, cluster, etc)

		timing.data  = readingRate;
		timing.pos   = currTickPoint.pos;
		timing.press = currTickPoint.press;   // records if it is at mid hold object or not
		timing.time  = currTickPoint.time;

		data.push_back(timing);

		prevTickPoint = currTickPoint;
		currTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, currTickPoint.time);

		if (currTickPoint.isInvalid()) break;
	}
}

void Analyzer_ReadingIntensity::AnalyzeCatch(Play* _play)
{

}

void Analyzer_ReadingIntensity::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_ReadingIntensity::AnalyzeMania(Play* _play)
{

}

void Analyzer_ReadingIntensity::AnalyzeDodge(Play* _play)
{

}