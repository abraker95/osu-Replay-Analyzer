#include "SpatialChaos.h"

#include "../AnalysisStruct.h"

#include "../../osuCalc.h"
#include "../../osu_standard.h"
#include "../../../utils/geometry.h"

Analyzer_SpatialChaos::Analyzer_SpatialChaos() : Analyzer("Spatial Chaos (notes * px/ms)"){}
Analyzer_SpatialChaos::~Analyzer_SpatialChaos(){}

void Analyzer_SpatialChaos::AnalyzeStd(Play* _play)
{
	Analyzer* analyzer = AnalysisStruct::beatmapAnalysis.getAnalyzer("velocity (px/ms)");
	if (analyzer == nullptr) return;

	std::vector<osu::TIMING>& velocities = *(analyzer->getData());
	if (velocities.size() == 0) return;

	// ---------------------

	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
		timing.data = 0;
		timing.key = 0;

	double ARms = AR2ms(_play->getMod()->getAR());
	
	osu::TIMING itrTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, 0);
	osu::TIMING currTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, itrTickPoint.time);
	
	std::vector<double> magnitudes;
	std::vector<double> dirMagXs, dirMagYs;

	while(true)
	{
		while (itrTickPoint.time < currTickPoint.time + ARms)
		{
			double dirMagX, dirMagY;
			
			double dist = getDist(currTickPoint.pos, itrTickPoint.pos);
			magnitudes.push_back(dist);

			if (currTickPoint.pos.X == itrTickPoint.pos.X) dirMagX = 0.0;
			else dirMagX = acos((currTickPoint.pos.X - itrTickPoint.pos.X) / dist);

			if (currTickPoint.pos.Y == itrTickPoint.pos.Y) dirMagY = 0.0;
			else dirMagY = acos((currTickPoint.pos.Y - itrTickPoint.pos.Y) / dist);

			dirMagXs.push_back(dirMagX);
			dirMagYs.push_back(dirMagY);

			itrTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, itrTickPoint.time);
			if (itrTickPoint.isInvalid()) break;
		}

		double distVar = Var(magnitudes);
		double dirVar = CirclularVar(dirMagXs, dirMagYs);
		int numNotes = OSUSTANDARD::getNumHitobjectsVisibleAt(_play, OSUSTANDARD::FindHitobjectAt(hitobjects, currTickPoint.time), 0.3);

		int index = osu::FindTimingAt(velocities, currTickPoint.time);
		double vel = velocities[index].data;

		// \TODO: Directional variance fucks up when there is a stream going in a circle shape. 
		timing.data = ABS(dirVar) * (double)numNotes * vel * 10.0; //sqrt(distVar*distVar + dirVar*dirVar);
		timing.key = MAX(timing.data, timing.key);
		timing.pos   = currTickPoint.pos;
		timing.press = currTickPoint.press;   // records if it is at mid hold object or not
		timing.time  = currTickPoint.time;

		data.push_back(timing);

		itrTickPoint = currTickPoint;
		currTickPoint = OSUSTANDARD::getNextTickPoint(hitobjects, currTickPoint.time);
		
		dirMagXs.clear();
		dirMagYs.clear();
		magnitudes.clear();

		if (currTickPoint.isInvalid()) break;
	}
}

void Analyzer_SpatialChaos::AnalyzeCatch(Play* _play)
{

}

void Analyzer_SpatialChaos::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_SpatialChaos::AnalyzeMania(Play* _play)
{

}

void Analyzer_SpatialChaos::AnalyzeDodge(Play* _play)
{

}