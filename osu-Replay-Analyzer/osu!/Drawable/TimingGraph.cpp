#include "TimingGraph.h"

#include "../../irrlicht/include/irrlicht.h"
#include "../../utils/mathUtils.h"
#include "../../utils/Geometry.h"

#include "../osuCalc.h"
#include "../osu_standard.h"

#include <iostream>

using namespace irr;
using namespace core;

TimingGraph::TimingGraph(int _xpos, int _ypos, Window& _win, Play* _play) : GuiObj(_xpos, _ypos, _win.getDimentions().Width, 0)
{
	guiType = "Timing_Graph";
	play = _play;
	zoom = 1;
	autoPlay = false;

	height = HEIGHT;

	layerMask = 0;
	layerMask = REFRESHRATE_TICKS | HITOBJECT_HIT_TIMINGS | HITOBJECT_VISIBILTITY;

	GenerateVisibilityTimings();
}

TimingGraph::~TimingGraph()
{
	for (TimingObject* timingObject : timingObjects)
	{
		if (timingObject != nullptr)
			delete timingObject;
	}
}

void TimingGraph::EnableLayer(LAYER _layer)
{
	layerMask |= _layer;
}

void TimingGraph::DisableLayer(LAYER _layer)
{
	layerMask &= (~_layer);
}

std::pair<int, int> TimingGraph::getViewTimes()
{
	int timeRange = this->width/zoom;
	int markerPos = this->getMarkerPos();
	double percent = getPercent(0, markerPos, this->width);

	return std::pair<int, int>(currViewTime - timeRange*percent, currViewTime + timeRange*(1 - percent));
}

double TimingGraph::getZoom()
{
	return zoom;
}

int TimingGraph::getViewTime()
{
	return currViewTime;
}

bool TimingGraph::ViewTimeChanged()
{
	return (currViewTime != prevViewTime);
}

// ---------- [PRIVATE] -----------

void TimingGraph::GenerateVisibilityTimings()
{
	Beatmap* beatmap = play->beatmap;
	if (beatmap == nullptr) return;

	int layer = 1;

	timingObjects.resize(beatmap->getHitobjects().size());
	for (int i = 0; i < timingObjects.size(); i++)
	{
		std::pair<int, int> visibilityTimes = OSUSTANDARD::getVisiblityTimes(*beatmap->getHitobjects()[i], play->getMod()->getAR(), play->getMod()->getModifier().HD, 0.1, 0.1);

		if (layer <= 1)
		{
			int index1 = beatmap->FindHitobjectAt(visibilityTimes.first);
			int index2 = beatmap->FindHitobjectAt(visibilityTimes.second);

			layer = MAX(OSUSTANDARD::getNumHitobjectsVisibleAt(play, index1, 0.1) + 1,
						OSUSTANDARD::getNumHitobjectsVisibleAt(play, index2, 0.1) + 1);
		}

		int layerPos = -5 * layer;
		const int layerHeight = 4;

		//timingObjects[i] = new TimingObject(startPos, layerPos, endPos-startPos, layerHeight, beatmap->hitObjects[i], this, this);
		timingObjects[i] = new TimingObject(0, layerPos, 0, layerHeight, beatmap->getHitobjects()[i], play->getMod(), this);
		layer--;
	}
 }

void TimingGraph::drawRefreshRateTimings(Window &_win, double _FPS)
{
	double updateLatency = (1000.0 / _FPS);

	std::pair<int, int> viewTimes = this->getViewTimes();
		double ticksPassedAtStart = floor(viewTimes.first / updateLatency); // number of ticks passed from t=0 to t=the stars of the viewTime
		double ticksPassedAtEnd = floor(viewTimes.second / updateLatency);  // number of ticks passed from t=0 to t=the end of the viewTime

	// Optimization. The ticks look like a fuking rectangle when zoomed out enough
	if(2*updateLatency*zoom < 1.0) // <-- this is actually the distance (px) between each tick
	{
		_win.driver->draw2DRectangle(SColor(255, 0, 0, 255), rect<s32>(absXpos, absYpos, absXpos + width, absYpos + height));
	}
	else
	{
		for (int t = 0; t < ticksPassedAtEnd - ticksPassedAtStart; t++)
		{
			int tickTime = (ticksPassedAtStart + t)*updateLatency - viewTimes.first;
			int tickPos = tickTime*zoom + updateLatency*zoom;

			_win.driver->draw2DLine(vector2di(absXpos + tickPos, absYpos), vector2di(absXpos + tickPos, absYpos + height), SColor(255, 0, 0, 255));
		}
	}
}

void TimingGraph::drawHitobjectHitTimings(Window &_win)
{
	Beatmap* beatmap = play->beatmap;
	if (beatmap == nullptr) return;

	std::pair<int, int> viewTimes = this->getViewTimes();
	int startIndex = -1, endIndex = -1;

	Database<Hitobject>& hitobjects = beatmap->getHitobjects();

	if (viewTimes.first >= hitobjects[0]->getTime())
		startIndex = beatmap->FindHitobjectAt(viewTimes.first);

	if (viewTimes.second <= hitobjects[hitobjects.size() - 1]->getTime())
		endIndex = beatmap->FindHitobjectAt(viewTimes.second);

	if (startIndex == -1 && endIndex == -1) return;
	if (startIndex == -1 && endIndex != -1) startIndex = 0;
	if (startIndex != -1 && endIndex == -1) endIndex = hitobjects.size() - 1;

	for (int i = startIndex; i < endIndex; i++)
	{
		int hitobjectBegTime = hitobjects[i]->getTime();
		int hitobjectEndTime = hitobjectBegTime;
		
		// get the actual end time if it is a slider
		if (hitobjects[i]->IsHitObjectType(SLIDER))
			hitobjectEndTime = hitobjects[i]->getSlider()->getEndTime();
		
		int posBeg = (hitobjectBegTime - viewTimes.first)*zoom;
		int posEnd = (hitobjectEndTime - viewTimes.first)*zoom;

		// if not slider, make sure it's at least 1 pixel width
		if (posBeg == posEnd)
			posEnd++;

		_win.driver->draw2DRectangle(SColor(255, 255, 0, 255), rect<s32>(absXpos + posBeg, absYpos, absXpos + posEnd, absYpos + height/5));
	}
}

void TimingGraph::drawHitobjectVisibilityTimings(Window &_win)
{
	Beatmap* beatmap = play->beatmap;
	if (beatmap == nullptr) return;

	std::pair<int, int> viewTimes = this->getViewTimes();
	int startIndex = 0, endIndex = beatmap->getHitobjects().size() - 1;

	if (viewTimes.first > beatmap->getHitobjects()[0]->getTime())
		startIndex = beatmap->FindHitobjectAt(viewTimes.first);

	if (viewTimes.second < beatmap->getHitobjects()[beatmap->getHitobjects().size() - 1]->getTime())
		endIndex = beatmap->FindHitobjectAt(viewTimes.second + AR2ms(play->getMod()->getAR()));

	if (endIndex <= startIndex) endIndex = beatmap->getHitobjects().size() - 1;
	for (int i = startIndex; i <= endIndex; i++)
	{
		timingObjects[i]->Update(_win);
	}
}

/*void drawHumanReactionTimings(Window &_win, int _xoffset, int _yoffset, std::vector<Hitcircle> &_hitcircles, int _shift, double _px_ms, double _FPS, double _AR)
{
	const int HUMAN_REACTION = 240; // ms

	// Show human reaction timings
	for (double i = 0; i < _hitcircles.size() - 1; i++)
	{
		double time = _hitcircles[i + 1].getTime() - AR2ms(_AR);
		int xpos = time * _px_ms + _xoffset;
		int ypos = 0 + _yoffset;
		int width = HUMAN_REACTION*_px_ms;

		if (xpos < _shift - AR2ms(_AR) * _px_ms)
			continue;
		else if (xpos - width >= (_shift + 811))
			break;

		_win.driver->draw2DRectangle(SColor(255, 0, 255, 0), rect<s32>(xpos - _shift, ypos+3, xpos + width - _shift, ypos+8));
	}
}*/

/*void drawEyeLatencyTimings(Window &_win, int _xoffset, int _yoffset, Beatmap* _beatmap, int _shift, double _px_ms, double _FPS, double _AR)
{
	const int HUMAN_REACTION = 240; // ms
	std::vector<Hitobject*> hitobjects = _beatmap->hitObjects;

	// Show eye latency timings
	for (double i = 1; i < hitobjects.size() - 1; i++)
	{
		// assume that the player begins to look at the next note when the 
		// note is 2 times the CS distance away from the circle for now
		// assume that the player doesn't snap to note for now
		double distPrev = getDist(hitobjects[i]->getPos(), hitobjects[i - 1]->getPos());
		double timePrev = hitobjects[i]->getTime() - hitobjects[i - 1]->getTime();
		double vel = distPrev / timePrev;

		double timeCertainty = hitobjects[i - 1]->getTime();
		if (vel != 0.0)
			timeCertainty += MAX(distPrev - hitobjects[i].getSizePx()*2.0, 0.0) / vel;

		// Assume player looks at the next note as soon as the player percieves it
		/// \TODO: This needs fixing when dealing with more than 1 note
		double timeNextVisible = _hitcircles[i + 1].getTime() - AR2ms(_AR) + HUMAN_REACTION;

		double time = 0.0;
		if (timeCertainty < timeNextVisible)
			time = timeNextVisible;
		else
			time = timeCertainty;

		int xpos = time * _px_ms + _xoffset;
		int ypos = 0 + _yoffset;
		double distNext = getDist(_hitcircles[i].getPos(), _hitcircles[i + 1].getPos());
		int width = EyeTime(px2Deg(distNext, 640, 50.0))*_px_ms;

		if (xpos < _shift - AR2ms(_AR)*_px_ms)
			continue;
		else if (xpos - width >= (_shift + 811))
			break;

		_win.driver->draw2DRectangle(SColor(255, 0, 155, 0), rect<s32>(xpos - _shift, ypos + 3, xpos + width - _shift, ypos+8));
	}
}*/

int TimingGraph::getVirtualxPos()
{
	return currViewTime*zoom;
}

int TimingGraph::getMarkerPos()
{
	return BOUND(0, getVirtualxPos(), (this->width / 2));
}

void TimingGraph::drawViewTimeMarker(Window &_win)
{
	// \TODO: Figure out why the displayed width shrinks one pixel when marker reaches middle
	int markerPos = this->getMarkerPos();
	int width = MAX(zoom, 2.0);
	_win.driver->draw2DRectangle(SColor(255, 255, 255, 255), rect<s32>(absXpos + markerPos, absYpos, absXpos + markerPos + width, absYpos + height));
}

void TimingGraph::drawTimingsText(Window& _win)
{
	std::pair<int, int> viewTimes = this->getViewTimes();
	int markerPos = this->getMarkerPos();

	_win.font->draw(core::stringw(viewTimes.first), core::rect<s32>(absXpos, absYpos + height - 10, 100, 10), video::SColor(255, 255, 255, 255));
	_win.font->draw(core::stringw(currViewTime), core::rect<s32>(absXpos + markerPos - 5, absYpos + height, 100, 10), video::SColor(255, 255, 255, 255));
	_win.font->draw(core::stringw(viewTimes.second), core::rect<s32>(absXpos + width - 25, absYpos + height - 10, 100, 10), video::SColor(255, 255, 255, 255));
}

void TimingGraph::UpdateInternal(Window& _win)
{
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

void TimingGraph::Draw(Window& _win)
{
	const double FPS = 60.0;

	if (layerMask & REFRESHRATE_TICKS)
	{
		this->drawRefreshRateTimings(_win, FPS);
	}

	if (layerMask & HITOBJECT_VISIBILTITY)
	{
		if(play->beatmap != nullptr)
			if(play->beatmap->getHitobjects().size() != 0)
				this->drawHitobjectVisibilityTimings(_win);
	}

	if (layerMask & HITOBJECT_HIT_TIMINGS)
	{
		if (play->beatmap != nullptr)
			if (play->beatmap->getHitobjects().size() != 0)
				this->drawHitobjectHitTimings(_win);
	}

	drawViewTimeMarker(_win);
	drawTimingsText(_win);


	//drawHumanReactionTimings(_win, _xoffset, _yoffset, _hitcircles, shift, px_ms, FPS, _AR);
	//drawEyeLatencyTimings(_win, _xoffset, _yoffset, _hitcircles, shift, px_ms, FPS, _AR);

	// TODO: simulate reaction time latency

	// TODO: Model player's cursor movement given parameters and time those

	// TODO: simulate button press/release timing + instability

	_win.driver->draw2DRectangleOutline(recti(absXpos, absYpos, absXpos + width - 1, absYpos + height), edgeCol);
}

void TimingGraph::UpdateUserControls(Window& _win)
{
	// mouse wheel time control
	if (_win.reciever.IsKeyDown(KEY_KEY_Z))
	{
		if (this->isMouseOnObj(_win))
		{
			UpdateZoom(_win);
		}
	}

	if (_win.reciever.IsKeyEdgeDown(KEY_SPACE))
	{
		autoPlay = !autoPlay;
	}

	if (!_win.reciever.IsKeyDown(KEY_KEY_Z) || (autoPlay == true))
	{
		UpdateViewTime(_win);
	}

	// \TODO: Update hitobject timing marks if dragged
	/*if (user press on hitobject timing)
	{
		UpdateHitobjectTimings(_win);
	}*/

	// \TODO: Allow timeGraph dragging to view time
}

void TimingGraph::UpdateZoom(Window& _win)
{
	double step = -zoom / 10.0; // amount of units to zoom in/out by
	double newRes = zoom + step*_win.reciever.getWheel();
	if (newRes >= 0)
		zoom = newRes;
}

void TimingGraph::UpdateViewTime(Window& _win)
{
	if (autoPlay == false)
	{
		prevViewTime = currViewTime;

		double const step = -10; // amount of px to move by
		double newTime_ms = currViewTime + (step / zoom) * _win.reciever.getWheel();
		if (newTime_ms >= 0)
			currViewTime = newTime_ms;
	}
	else
	{
		prevViewTime = currViewTime;
		
		double newTime_ms = currViewTime + 1000.0/_win.getFPS(); // 1000 Frames / (Frames/second)
		if (newTime_ms >= 0)
			currViewTime = newTime_ms;
	}
}

void TimingGraph::IdleLogic(Window &_win)
{
	if (this->isMouseOnObj(_win, true))
		state = HIGHLIGHTED;
	else
		edgeCol = IDLE_COLOR;
}

void TimingGraph::HighlightedLogic(Window &_win)
{
	if (!this->isMouseOnObj(_win, true))
		state = IDLE;
	else
		edgeCol = HIGHLIGHTED_COLOR;
}