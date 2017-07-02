#include "AgilitySim.h"

#include "../../osuCalc.h"
#include "../../../utils/geometry.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

Analyzer_AgilitySim::Analyzer_AgilitySim() : Analyzer("[] AutoPilot Sim") {}
Analyzer_AgilitySim::~Analyzer_AgilitySim() {}

void Analyzer_AgilitySim::AnalyzeStd(Play* _play)
{
	Database<Hitobject>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
		timing.data = 0;

	double timeDelta = 1; // ms
	double currTime = hitobjects[0]->getTime() - 1;

	double size = CS2px(_play->getMod()->getCS());
	osu::TIMING prevDuePoint;
	osu::TIMING duePoint = OSUSTANDARD::getNextTickPoint(hitobjects, hitobjects[0]->getTime() - 1);

	double autoPilotWeight = 1.0;	// mass
	vector2d<double> autoPilotVel = vector2d<double>(0, 0);  // osu!px/ms
	vector2d<double> autoPilotPos = hitobjects[0]->getPos();
	vector2d<double> autoPilotForce = vector2d<double>(0, 0);
	vector2d<double> targetDir = vector2d<double>(0, 0);

	while (!duePoint.isInvalid())
	{
		//double dist1 = getDist(prevDuePoint.pos, duePoint.pos)/8.0;		// get mid distance
		//double dist2 = getDist(autoPilotPos + autoPilotVel, duePoint.pos);
		//double percentDist = getPercent(0, dist2, dist1);
		//if (getDist(prevDuePoint.pos, duePoint.pos) == 0) percentDist = 1.0;

		autoPilotForce = -targetDir / ((duePoint.time - prevDuePoint.time) / timeDelta);

		if (currTime >= duePoint.time)
		{
			prevDuePoint = duePoint;
			duePoint = OSUSTANDARD::getNextTickPoint(hitobjects, duePoint.time);

			targetDir = duePoint.pos - autoPilotPos;
			targetDir.normalize();

			autoPilotForce = targetDir;

			timing.pos = autoPilotPos;
			timing.time = currTime;

			data.push_back(timing);
		}

		if ((long)currTime % 100)
		{
			timing.pos = autoPilotPos;
			timing.time = currTime;

			data.push_back(timing);
		}

		autoPilotVel.X += (autoPilotForce.X * timeDelta) / autoPilotWeight;
		autoPilotVel.Y += (autoPilotForce.Y * timeDelta) / autoPilotWeight;

		autoPilotPos += autoPilotVel;


		currTime += timeDelta;
	}
}

void Analyzer_AgilitySim::AnalyzeCatch(Play* _play)
{

}

void Analyzer_AgilitySim::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_AgilitySim::AnalyzeMania(Play* _play)
{

}

void Analyzer_AgilitySim::AnalyzeDodge(Play* _play)
{

}