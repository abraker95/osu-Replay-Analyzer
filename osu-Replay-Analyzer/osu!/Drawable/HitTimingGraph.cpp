#include "HitTimingGraph.h"
#include "../Score/ScoreEngine.h"

#include "../Score/osu-std/score.h"
#include "../Score/osu-catch/score.h"
#include "../Score/osu-taiko/score.h"
#include "../Score/osu-mania/score.h"
#include "../Score/osu-dodge/score.h"

HitTimingGraph::HitTimingGraph(int _xpos, int _ypos, Play* _play) : GuiObj(_xpos, _ypos, 0, 0, nullptr)
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
	
	int xScale = 37;
	double yScale = 200.0 / max;
	int seperation = xScale - 2;

	_win.driver->draw2DLine(vector2di(absXpos, absYpos), vector2di(absXpos + xScale*bins.size(), absYpos), SColor(255, 255, 255, 255));

	SColor color;
	core::stringw text;
	for (int i = 0; i < bins.size(); i++)
	{
		text = textBins[i];
		color = colorTable.getColor(i);

		_win.driver->draw2DRectangle(color, rect<s32>(absXpos + xScale*i, absYpos - yScale*bins[i], absXpos + xScale*i + seperation, absYpos));
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
	const int NUM_BINS = 5;
	bins.resize(NUM_BINS + 1);
	for (int& bin : bins)
		bin = 0;

	const double BIN_TABLE[] =
	{
		  0.000,
		 20.000,
		 40.000,
		 80.500,
		160.000,
		320.000
	};

	for (auto& timing : OSUSTANDARD::accTimings)
	{
		for (int i = 0; i < NUM_BINS; i++)
		{
			// miss
			if (timing.data > BIN_TABLE[NUM_BINS])
			{
				bins[NUM_BINS]++;
				break;
			}

			// hit
			if (BTWN(-BIN_TABLE[i + 1], timing.data, -BIN_TABLE[i]) || BTWN(BIN_TABLE[i], timing.data, BIN_TABLE[i + 1]))
			{
				bins[i]++;
				break;
			}
		}
	}

	textBins.resize(NUM_BINS + 1);
	for (int i = 0; i < NUM_BINS; i++)
		textBins[i] = (std::to_string((int)BIN_TABLE[i]) + " - " + std::to_string((int)BIN_TABLE[i + 1])).data();
	textBins[NUM_BINS] = "MISSES";
}

void HitTimingGraph::genCtbBins()
{

}

void HitTimingGraph::genTaikoBins()
{

}

void HitTimingGraph::genManiaBins()
{
	const int NUM_BINS = 5;
	bins.resize(NUM_BINS + 1);
	for (int& bin : bins)
		bin = 0;

	const double BIN_TABLE[] =
	{
		 0.000,
		 3.125,
		 6.350,
		12.500,
		25.000,
		50.000
	};

	for (auto& timing : OSUMANIA::accTimings)
	{
		for (int i = 0; i < NUM_BINS; i++)
		{
			// miss
			if (timing.data > BIN_TABLE[NUM_BINS])
			{
				bins[NUM_BINS]++;
				break;
			}

			// hit
			if (BTWN(-BIN_TABLE[i + 1], timing.data, -BIN_TABLE[i]) || BTWN(BIN_TABLE[i], timing.data, BIN_TABLE[i + 1]))
			{
				bins[i]++;
				break;
			}
		}
	}

	textBins.resize(NUM_BINS + 1);
	for (int i = 0; i < NUM_BINS; i++)
		textBins[i] = (std::to_string((int)BIN_TABLE[i]) + " - " + std::to_string((int)BIN_TABLE[i + 1])).data();
	textBins[NUM_BINS] = "MISSES";
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