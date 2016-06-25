#include "HitTimingGraph.h"
#include "../Filestructure/ScoreEngine.h"

HitTimingGraph::HitTimingGraph(int _xpos, int _ypos, Play* _play) : GuiObj(_xpos, _ypos, 0, 0, nullptr)
{
	bins.resize(6);
		bins[0] = bins[1] = bins[2] = bins[3] = bins[4] = bins[5] = 1;

	auto& accTimings = _play->scoreEngine->accTimings;

	for (auto& timing : accTimings)
	{
		if (BTWN(-3.125, timing.data, 0) || BTWN(0, timing.data, +3.125))
			bins[0]++;

		else if (BTWN(-6.25, timing.data, -3.125) || BTWN(+3.125, timing.data, +6.25))
			bins[1]++;

		else if (BTWN(-12.5, timing.data, -6.25) || BTWN(+6.25, timing.data, +12.5))
			bins[2]++;

		else if (BTWN(-25.0, timing.data, -12.5) || BTWN(+12.5, timing.data, +25.0))
			bins[3]++;

		else if (BTWN(-50.0, timing.data, -25.0) || BTWN(+25.0, timing.data, +50.0))
			bins[4]++;

		else if (BTWN(-100.0, timing.data, -50.0) || BTWN(+50.0, timing.data, +100.0))
			bins[5]++;
	}
}

void HitTimingGraph::Draw(Window& _win)
{
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
		if (i == 0)
		{
			color = SColor(255, 255, 255, 255);
			text = core::stringw("0 - 3");
		}
		else if (i == 1)
		{
			color = SColor(255, 128, 128, 255);
			text = core::stringw("3 - 6");
		}
		else if (i == 2)
		{
			color = SColor(255, 128, 255, 128);
			text = core::stringw("6 - 12");
		}
		else if (i == 3)
		{
			color = SColor(255, 255, 255, 128);
			text = core::stringw("12 - 25");
		}
		else if (i == 4)
		{
			color = SColor(255, 255, 128, 128);
			text = core::stringw("25 - 50");
		}
		else if (i == 5)
		{
			color = SColor(255, 128, 128, 128);
			text = core::stringw("50 - 100");
		}
			

		_win.driver->draw2DRectangle(color, rect<s32>(absXpos + xScale*i, absYpos - yScale*bins[i], absXpos + xScale*i + seperation, absYpos));
		_win.font->draw(text, core::rect<s32>(absXpos + xScale*i, absYpos, 100, 10), video::SColor(255, 255, 255, 255));
		_win.font->draw(core::stringw(bins[i]), core::rect<s32>(absXpos + xScale*i, absYpos - yScale*bins[i] - 10, 100, 10), video::SColor(255, 255, 255, 255));
	}
}

void HitTimingGraph::UpdateInternal(Window& _win)
{

}