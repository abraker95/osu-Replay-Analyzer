#include "HitTimingMarker.h"
#include "../Analysis/AnalysisStruct.h"

HitTimingMarker::HitTimingMarker(int _xpos, int _ypos, Play* _play, long* _viewTime) : GuiObj(_xpos, _ypos, 0, 10, nullptr)
{
	viewTime = _viewTime;
	play = _play;
}

void HitTimingMarker::Draw(Window& _win)
{
	_win.driver->draw2DRectangleOutline(recti(absXpos, absYpos, absXpos + width - 1, absYpos + height), edgeCol);
	if (play->beatmap == nullptr) return;

	switch (play->beatmap->getGamemode())
	{
		case GAMEMODE::OSU_STANDARD:
			this->DrawStdTimings(_win);
			break;

		case GAMEMODE::OSU_CATCH:
			this->DrawCtBTimings(_win);
			break;

		case GAMEMODE::OSU_TAIKO:
			this->DrawTaikoTimings(_win);
			break;

		case GAMEMODE::OSU_MANIA:
			this->DrawManiaTimings(_win);
			break;

		case GAMEMODE::OSU_DODGE:
			this->DrawDodgeTimings(_win);
			break;

		default:
			break;
	}
}

void HitTimingMarker::UpdateInternal(Window& _win)
{
	width = _win.getDimentions().Width;
	UpdateUserControls(_win);

	switch (state)
	{
		case IDLE:
			IdleLogic(_win);
			break;

		case HIGHLIGHTED:
			HighlightedLogic(_win);
			break;

		default:
			// ???
			break;
	}
}


// ------------------- Draw ------------------

void HitTimingMarker::DrawStdTimings(Window& _win)
{
	Analyzer* analyzer = AnalysisStruct::beatmapAnalysis.getAnalyzer("Tap Deviation (ms)");
	if (analyzer == nullptr) return;

	for (auto& timing : *(analyzer->getData()))
	{
		if (BTWN(*viewTime - timeZoom, timing.time, *viewTime))
		{
			double markXpos = width/2.0 * (double)timing.data / viewZoom + width/2.0;
			_win.driver->draw2DLine(vector2di(absXpos + markXpos, absYpos), vector2di(absXpos + markXpos, absYpos + height), SColor(255, 255, 255, 255));
		}	
	}
}

void HitTimingMarker::DrawCtBTimings(Window& _win)
{

}

void HitTimingMarker::DrawTaikoTimings(Window& _win)
{

}

void HitTimingMarker::DrawManiaTimings(Window& _win)
{
	Analyzer* analyzer = AnalysisStruct::beatmapAnalysis.getAnalyzer("Tap Deviation (ms)");
	if (analyzer == nullptr) return;

	for (auto& timing : *(analyzer->getData()))
	{
		if (BTWN(*viewTime - timeZoom, timing.time, *viewTime))
		{
			double markXpos = width / 2.0 * (double)timing.data / viewZoom + width / 2.0;
			_win.driver->draw2DLine(vector2di(absXpos + markXpos, absYpos), vector2di(absXpos + markXpos, absYpos + height), SColor(255, 255, 255, 255));
		}
	}
}

void HitTimingMarker::DrawDodgeTimings(Window& _win)
{

}


// ------------- Controls -----------

void HitTimingMarker::UpdateUserControls(Window& _win)
{
	// mouse wheel time control
	if (_win.reciever.IsKeyDown(KEY_KEY_Z))
	{
		if (this->isMouseOnObj(_win))
		{
			UpdateZoomView(_win);
		}
	}

	if (_win.reciever.IsKeyDown(KEY_KEY_X))
	{
		if (this->isMouseOnObj(_win))
		{
			UpdateZoomTime(_win);
		}
	}
}

void HitTimingMarker::UpdateZoomTime(Window& _win)
{
	double step = -timeZoom / 10.0; // amount of units to zoom in/out by
	double newRes = timeZoom + step*_win.reciever.getWheel();
	if (newRes >= 0)
		timeZoom = newRes;
}

void HitTimingMarker::UpdateZoomView(Window& _win)
{
	double step = -viewZoom / 10.0; // amount of units to zoom in/out by
	double newRes = viewZoom + step*_win.reciever.getWheel();
	if (newRes >= 0)
		viewZoom = newRes;
}

void HitTimingMarker::IdleLogic(Window &_win)
{
	if (this->isMouseOnObj(_win, true))
		state = HIGHLIGHTED;
	else
		edgeCol = IDLE_COLOR;
}

void HitTimingMarker::HighlightedLogic(Window &_win)
{
	if (!this->isMouseOnObj(_win, true))
		state = IDLE;
	else
		edgeCol = HIGHLIGHTED_COLOR;
}