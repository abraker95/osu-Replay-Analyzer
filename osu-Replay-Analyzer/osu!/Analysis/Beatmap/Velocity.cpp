#include "Velocity.h"
#include "../../osu_standard.h"
#include "../../../utils/geometry.h"

Analyzer_Velocity::Analyzer_Velocity() : Analyzer("velocity (px/ms)"){}
Analyzer_Velocity::~Analyzer_Velocity(){}

void Analyzer_Velocity::AnalyzeStd(Play* _play)
{
	Database<Hitobject>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
	timing.data = 0;

	long time = 0;

	osu::TIMING prevTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, time, &time);
	osu::TIMING currTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, time, &time);

	while(true)
	{
		double velocity = getVelocity(vector2d<double>(prevTickPoint.pos.X, prevTickPoint.pos.Y),
									  vector2d<double>(currTickPoint.pos.X, currTickPoint.pos.Y),
									  prevTickPoint.time, currTickPoint.time);
		if (velocity == INFINITY) velocity = INT_MAX; // irrlitch has issues displaying infinity

		timing.data  = velocity;
		timing.pos   = currTickPoint.pos;
		timing.press = currTickPoint.press;   // records if it is at mid hold object or not
		timing.time  = currTickPoint.time;

		data.push_back(timing);

		prevTickPoint = currTickPoint;
		currTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, time, &time);

		if (currTickPoint.isInvalid()) break;
	}
}

void Analyzer_Velocity::AnalyzeCatch(Play* _play)
{

}

void Analyzer_Velocity::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_Velocity::AnalyzeMania(Play* _play)
{

}

void Analyzer_Velocity::AnalyzeDodge(Play* _play)
{

}