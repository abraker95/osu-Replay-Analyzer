#include "Distance-size.h"

#include "../../../utils/geometry.h"
#include "../../osuCalc.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

Analyzer_DistSize::Analyzer_DistSize() : Analyzer("dist/size (dB)") {}
Analyzer_DistSize::~Analyzer_DistSize() {}

void Analyzer_DistSize::AnalyzeStd(Play* _play)
{
	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
	timing.data = 0;

	long time = 0;
	double size = CS2px(_play->getMod()->getCS());

	osu::TIMING prevTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, time, &time);
	osu::TIMING currTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, time, &time);
  
	while(true)
	{
		double dist = getDist(vector2d<double>(prevTickPoint.pos.X, prevTickPoint.pos.Y),
							  vector2d<double>(currTickPoint.pos.X, currTickPoint.pos.Y));
		if (dist == INFINITY) dist = INT_MAX; // irrlitch has issues displaying infinity

		double dist_size;
			 if (size == 0) dist_size = INT_MAX;
		else if (dist == 0) dist_size = 0;
		else		  	    dist_size = 10*log(dist / size);

		timing.data  = dist_size;
		timing.pos   = currTickPoint.pos;
		timing.press = currTickPoint.press;   // records if it is at mid hold object or not
		timing.time  = currTickPoint.time;

		data.push_back(timing);

		prevTickPoint = currTickPoint;
		currTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, time, &time);

		if (currTickPoint.isInvalid()) break;
	}
}

void Analyzer_DistSize::AnalyzeCatch(Play* _play)
{

}

void Analyzer_DistSize::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_DistSize::AnalyzeMania(Play* _play)
{

}

void Analyzer_DistSize::AnalyzeDodge(Play* _play)
{

}