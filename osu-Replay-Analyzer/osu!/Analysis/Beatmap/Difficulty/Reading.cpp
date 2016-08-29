#include "Reading.h"

#include "../../../osu_standard.h"
#include "../../../../utils/geometry.h"
#include "../../../osuCalc.h"

#include "../../AnalysisStruct.h"

Analyzer_Reading::Analyzer_Reading() : Analyzer("reading diff") {}
Analyzer_Reading::~Analyzer_Reading() {}

void Analyzer_Reading::AnalyzeStd(Play* _play)
{
	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;

	std::vector<osu::TIMING>& timingsRate = *AnalysisStruct::beatmapAnalysis.getAnalyzer("note rate")->getData();
	std::vector<osu::TIMING>& timingsVel = *AnalysisStruct::beatmapAnalysis.getAnalyzer("velocity (px/ms)")->getData();

	vector2df avgCoor, distractor;
	int i = 0, previ = 0;
	double delay = 10.0;
	double avgCoeff = 0.4; // higher coeff = more precise

	for (int ms = 0; ms < hitobjects[hitobjects.size() - 1]->getTime(); ms += delay)
	{
		// Let index catch up to the time. Bail if out of bounds
		while (hitobjects[i]->getEndTime() < ms) i++;
		if (i >= hitobjects.size()) break;

		// Out of bound check
		if (!BTWN(1, i, hitobjects.size() - 1))
			continue;

		previ = i;

		int iNoteRate = FindTimingAt(timingsRate, ms);
		int iNoteVel = FindTimingAt(timingsVel, ms);

		double noteRateVel = timingsVel[iNoteVel].data*timingsRate[iNoteRate].data;
		const double diffCoeff = 1.02; // interpreted easiness of the note rate velocity. Use this to adjust interpreted reading difficulty. (higher = easier, SENSITIVE to nearest 0.01)

		if (timingsVel[iNoteVel].press == false)
			noteRateVel /= 1.01;
		else
			noteRateVel /= 1.03;

		avgCoeff = 1.0 / ((noteRateVel/*/diffCoeff*/) + 1.0);

		// Apply cursor movement averaging
		if (!hitobjects[i]->isHitobjectLong())
		{
			avgCoor.X = avgCoeff*((double)hitobjects[i]->getPos().X + distractor.X) + (1.0 - avgCoeff)*avgCoor.X;
			avgCoor.Y = avgCoeff*((double)hitobjects[i]->getPos().Y + distractor.Y) + (1.0 - avgCoeff)*avgCoor.Y;
		}
		else
		{
			avgCoor.X = avgCoeff*(double)(hitobjects[i]->getSlider()->GetSliderPos(ms).X + distractor.X) + (1.0 - avgCoeff)*avgCoor.X;
			avgCoor.Y = avgCoeff*(double)(hitobjects[i]->getSlider()->GetSliderPos(ms).Y + distractor.Y) + (1.0 - avgCoeff)*avgCoor.Y;
		}

		timing.press = false;
		if (!hitobjects[i]->isHitobjectLong()) // hitcircle
		{
			if (BTWN(ms - delay / 2.0, hitobjects[i]->getTime(), ms + delay / 2.0))
			{
				double dist = getDist(hitobjects[i]->getPos(), vector2d<double>(avgCoor.X, avgCoor.Y));
				double radius = CS2px(_play->getMod()->getCS()) / 2.0;
				
				const double divider = 1.0;			// SENSITIVE to nearest 1.0
				const double distSensitivity = 1.4; // shifts distance by x radii (1.0 = 1.0*CSpx). Use this to fix imbalance due to interpreted slider reading difficulty. Higher = easier. SENSITIVE to nearest 0.01

				//                  formula											 set x=0 to y=0
				timing.data = exp(dist - radius*distSensitivity)/divider - exp(0 - radius*distSensitivity)/divider;
				timing.press = true;
			}
		}
		else  // slider
		{
			if (BTWN(hitobjects[i]->getTime(), ms, hitobjects[i]->getEndTime()))
			{
				vector2d<double> pos = hitobjects[i]->getSlider()->GetSliderPos(ms);
				double dist = getDist(vector2d<double>(pos.X, pos.Y), vector2d<double>(avgCoor.X, avgCoor.Y));
				double radius = CS2px(_play->getMod()->getCS()) / 2.0;
				
				const double divider = 1.0;          // SENSITIVE to nearest 1.0
				const double distSensitivity = 1.36; // shifts distance by x radii (1.0 = 1.0*CSpx). Use this to adjust interpreted slider reading difficulty. Higher = easier. SENSITIVE to nearest 0.01
	
				//                  formula											 set x=0 to y=0
				timing.data = exp(dist - radius*distSensitivity)/divider - exp(0 - radius*distSensitivity)/divider;
				timing.press = true;
			}
		}

		timing.pos = avgCoor;
		timing.time = ms;

		// i > 1 to remove averaging errors when starting
		if (i > 1)
			data.push_back(timing);
	}
}

void Analyzer_Reading::AnalyzeCatch(Play* _play)
{

}

void Analyzer_Reading::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_Reading::AnalyzeMania(Play* _play)
{

}

void Analyzer_Reading::AnalyzeDodge(Play* _play)
{

}