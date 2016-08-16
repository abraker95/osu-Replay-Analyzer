#include "HitTimingGraph.h"
#include "../Score/ScoreEngine.h"

#include "../Score/osu-std/score.h"
#include "../Score/osu-catch/score.h"
#include "../Score/osu-taiko/score.h"
#include "../Score/osu-mania/score.h"
#include "../Score/osu-dodge/score.h"

HitTimingGraph::HitTimingGraph(int _xpos, int _ypos, Play* _play) : GuiObj(_xpos, _ypos, 195, 200.0, nullptr)
{
	play = _play;

	colorTable.AddValueMap(0, SColor(255, 255, 255, 255));
	colorTable.AddValueMap(1, SColor(255, 128, 128, 255));
	colorTable.AddValueMap(2, SColor(255, 128, 255, 128));
	colorTable.AddValueMap(3, SColor(255, 255, 255, 128));
	colorTable.AddValueMap(4, SColor(255, 255, 128, 128));
	colorTable.AddValueMap(5, SColor(255, 128, 128, 128));
	colorTable.AddValueMap(6, SColor(255, 255, 255, 255));
	colorTable.AddValueMap(7, SColor(255, 255, 255, 255));
	colorTable.AddValueMap(8, SColor(255, 255, 255, 255));
}

HitTimingGraph::~HitTimingGraph()
{
	Clear();
}


void HitTimingGraph::Init()
{
	Clear();

	if (play->beatmap == nullptr) return;
	switch (play->beatmap->getGamemode())
	{
		case GAMEMODE::OSU_STANDARD:
			this->genStdBins();
			break;

		case GAMEMODE::OSU_CATCH:
			this->genCtbBins();
			break;

		case GAMEMODE::OSU_TAIKO:
			this->genTaikoBins();
			break;

		case GAMEMODE::OSU_MANIA:
			this->genManiaBins();
			break;

		case GAMEMODE::OSU_DODGE:
			this->genDodgeBins();
			break;

		default:
			break;
	}
}

void HitTimingGraph::setBinTable(std::vector<double> _binTable)
{
	binTable = _binTable;
}

// ----------- PRIVATE ----------------


void HitTimingGraph::Draw(Window& _win)
{
	// Make sure there is something to draw
	if (bins.size() == 0) return;

	// get highest bin for normalization
	int max = 1;
	for (int i = 0; i < bins.size(); i++)
		if (bins[i] > max)
			max = bins[i];
	
	int seperation = 2;
	double xScale = ((double)width/(double)bins.size()) - ((double)seperation/(double)bins.size());
	double yScale = (double)height / (double)max;
	
	_win.driver->draw2DLine(vector2di(absXpos, absYpos), vector2di(absXpos + xScale*bins.size(), absYpos), SColor(255, 255, 255, 255));

	SColor color;
	core::stringw text;
	for (int i = 0; i < bins.size(); i++)
	{
		text = textBins[i];
		color = colorTable.getColor(i);

		_win.driver->draw2DRectangle(color, rect<s32>(absXpos + xScale*i, absYpos - yScale*bins[i], absXpos + xScale*(i + 1) - seperation, absYpos));
		_win.font->draw(text, core::rect<s32>(absXpos + xScale*i, absYpos, 100, 10), video::SColor(255, 255, 255, 255));
		_win.font->draw(core::stringw(bins[i]), core::rect<s32>(absXpos + xScale*i, absYpos - yScale*bins[i] - 10, 100, 10), video::SColor(255, 255, 255, 255));
	}
}

void HitTimingGraph::UpdateInternal(Window& _win)
{

}


// Bin Generation

void HitTimingGraph::genStdBins()
{
	setBinTable(
	{ 
		0.000,
		4.500,
		10.000,
		20.000,
		40.000,
		80.500,
		160.000,
		320.000 
	});

	bins.resize(binTable.size() + 1);
	for (int& bin : bins)
		bin = 0;

	for (auto& timing : OSUSTANDARD::accTimings)
	{
		for (int i = 0; i < binTable.size() - 1; i++)
		{
			// miss
			if (timing.data > binTable[binTable.size() - 1])
			{
				bins[binTable.size()]++;
				break;
			}

			// hit
			if (BTWN(binTable[i], abs(timing.data), binTable[i + 1]))
			{
				bins[i]++;
				break;
			}
		}
	}

	textBins.resize(binTable.size() + 1);
	for (int i = 0; i < binTable.size(); i++)
		textBins[i] = (std::to_string((int)binTable[i])/* + " - " + std::to_string((int)binTable[i + 1])*/).data();
	textBins[binTable.size()] = "MISSES";
}

void HitTimingGraph::genCtbBins()
{

}

void HitTimingGraph::genTaikoBins()
{

}

void HitTimingGraph::genManiaBins()
{
	setBinTable(
	{
		0.000,
		20.000,
		40.000,
		80.500,
		160.000,
		320.000
	});

	bins.resize(binTable.size() + 1);
	for (int& bin : bins)
		bin = 0;

	for (auto& timing : OSUMANIA::accTimings)
	{
		for (int i = 0; i < binTable.size() - 1; i++)
		{
			// miss
			if (timing.data > binTable[binTable.size() - 1])
			{
				bins[binTable.size()]++;
				break;
			}

			// hit
			if (BTWN(binTable[i], abs(timing.data), binTable[i + 1]))
			{
				bins[i]++;
				break;
			}
		}
	}

	textBins.resize(binTable.size() + 1);
	for (int i = 0; i < binTable.size(); i++)
		textBins[i] = (std::to_string((int)binTable[i])/* + " - " + std::to_string((int)binTable[i + 1])*/).data();
	textBins[binTable.size()] = "MISSES";
}

void HitTimingGraph::genDodgeBins()
{

}

void HitTimingGraph::Clear()
{
	bins.clear();
	binTable.clear();
	textBins.clear();

	std::vector<int>().swap(bins);
	std::vector<double>().swap(binTable);
	std::vector<core::stringw>().swap(textBins);
}