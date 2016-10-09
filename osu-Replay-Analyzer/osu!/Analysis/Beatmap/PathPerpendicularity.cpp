#include "PathPerpendicularity.h"

#include "../../osuCalc.h"
#include "../../osu_standard.h"

#include "../../../utils/Geometry.h"

Analyzer_PathPerpendicularity::Analyzer_PathPerpendicularity() : Analyzer("Path perpendicularity (%)") {}
Analyzer_PathPerpendicularity::~Analyzer_PathPerpendicularity() {}

void Analyzer_PathPerpendicularity::AnalyzeStd(Play* _play)
{
	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
		timing.data = 0;

	double arMs = AR2ms(_play->getMod()->getAR());
	double csPx = CS2px(_play->getMod()->getCS());

	std::vector<osu::TIMING> points = OSUSTANDARD::getPattern(_play, 3, hitobjects[0]->getTime() - 1);
	while (points.size() == 3)
	{
		double perpendicularity = ABS(1.0 - cos(2.0*getAngle(points[0].pos, points[1].pos, points[2].pos)))/2.0;

		timing.data = perpendicularity;
		timing.pos = points[2].pos;
		timing.press = false;
		timing.time = points[2].time;

		data.push_back(timing);

		points = OSUSTANDARD::getPattern(_play, 3, points[0].time);
	}
}

void Analyzer_PathPerpendicularity::AnalyzeCatch(Play* _play) {}

void Analyzer_PathPerpendicularity::AnalyzeTaiko(Play* _play) {}

void Analyzer_PathPerpendicularity::AnalyzeMania(Play* _play) {}

void Analyzer_PathPerpendicularity::AnalyzeDodge(Play* _play)
{

}