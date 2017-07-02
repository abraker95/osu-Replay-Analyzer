#include "PathDirection.h"
#include "../../osu_standard.h"
#include "../../../utils/geometry.h"

Analyzer_PathDirection::Analyzer_PathDirection() : Analyzer("Path Direction (rad)"){}
Analyzer_PathDirection::~Analyzer_PathDirection(){}

void Analyzer_PathDirection::AnalyzeStd(Play* _play)
{
	Database<Hitobject>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
	timing.data = 0;

	osu::TIMING prevTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, hitobjects[0]->getTime());
	osu::TIMING currTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, prevTickPoint.time);

	while(true)
	{
		vector2d<double> dir = currTickPoint.pos - prevTickPoint.pos;
		dir.normalize();

		double absAngle;
		//if (dir.X == 0)	absAngle = 0;//(dir.Y <= 0) ? M_PI : 2 * M_PI;
		/*else*/			absAngle = -atan(dir.Y / dir.X);
		if (dir.Y < 0) absAngle = -absAngle;
		if (dir.X < 0) absAngle = -absAngle;
		//if (absAngle < 0) absAngle = M_PI - absAngle;

		timing.data  = absAngle;
		timing.pos   = currTickPoint.pos;
		timing.press = currTickPoint.press;   // records if it is at mid hold object or not
		timing.time  = currTickPoint.time;

		data.push_back(timing);

		prevTickPoint = currTickPoint;
		currTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, currTickPoint.time);

		if (currTickPoint.isInvalid()) break;
	}
}

void Analyzer_PathDirection::AnalyzeCatch(Play* _play)
{

}

void Analyzer_PathDirection::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_PathDirection::AnalyzeMania(Play* _play)
{

}

void Analyzer_PathDirection::AnalyzeDodge(Play* _play)
{

}