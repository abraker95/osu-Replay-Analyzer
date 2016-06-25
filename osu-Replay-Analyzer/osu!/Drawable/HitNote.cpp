#include "HitNote.h"
#include "GamemodeRenderer.h"

#include "../osu_mania.h"
#include "../../utils/mathUtils.h"
#include "../../ui/drawUtils.h"
#include "../osuCalc.h"

#include <iostream>
#include <tuple>

HitNote::HitNote(Beatmap* _beatmap, Hitobject* _hitobject, int* _viewTime, double* _zoom)
	: GuiObj(log2(_hitobject->getPos().X) * 300, 0, 50, 15)
{
	beatmap = _beatmap;
	hitobject = _hitobject;
	viewTime = _viewTime;
	zoom = _zoom;

	state = IDLE;
	edgeCol = IDLE_COLOR;
}

// ------ [PRIVATE] ------

void HitNote::Draw(Window &_win)
{
	_win.driver->draw2DRectangle(SColor(255, 120, 0, 50), rect<s32>(absXpos, absYpos, absXpos + width, absYpos + height));
	_win.driver->draw2DRectangleOutline(recti(absXpos, absYpos, absXpos + width - 1, absYpos + height), SColor(255, 255, 0, 0));
}

void HitNote::IdleLogic(Window &_win)
{
	if (this->isMouseOnObj(_win, true))
		state = HIGHLIGHTED;
	else
		edgeCol = IDLE_COLOR;
}

void HitNote::HighlightedLogic(Window &_win)
{
	if (!this->isMouseOnObj(_win, true))
		state = IDLE;
	else
	{
		bool leftEdge = _win.reciever.GetMouseState().LeftButtonEdge;
		if (leftEdge)
		{
			state = SELECTED;
		}

		edgeCol = HIGHLIGHTED_COLOR;
	}
}

void HitNote::SelectedLogic(Window &_win)
{
	position2di mouseDelta = _win.reciever.getCursorDelta();
	bool leftButton = _win.reciever.GetMouseState().LeftButtonDown;
	bool leftEdge = _win.reciever.GetMouseState().LeftButtonEdge;

	/// \TODO: Slider points; See if slider gen and update code can be simplified

	if (!this->isMouseOnObj(_win, true) && leftEdge)
		state = IDLE;
	else if (leftButton)
	{
		// has to be += due the fact that x - xoffset = 0, 
		// resolving the assignment to x = pos.x
		// this->x = pos.X - this->xOffset + this->x  => this->x = pos.X - 0;
		//vector2d<double> newPos(hitobject->getPos().X + (double)mouseDelta.X / this->getWidthRatio(), hitobject->getPos().Y + (double)mouseDelta.Y / this->getHeightRatio());
		//hitobject->setPos(newPos);

		edgeCol = SELECTED_COLOR;
	}
}

void HitNote::UpdateInternal(Window &_win)
{
	switch (state)
	{
	case IDLE:
		IdleLogic(_win);
		break;

	case HIGHLIGHTED:
		HighlightedLogic(_win);
		break;

	case SELECTED:
		SelectedLogic(_win);
		break;

	default:
		// ???
		break;
	}
}

/// \TODO: These use (OsuManiaRenderer*)parent)->getStartTime() which is a dirty hack and dangerous (bad coding practice). Find a better solution for this.
///		   Suggestion: use a globals like _widthRatio_ and _heightRatio_ in GamemodeRenderer.h

void HitNote::UpdateAbsPos(Window& _win)
{
	int numKeys = beatmap->getDiff().cs;
	int key = OSUMANIA::getKey(hitobject->getPos().X, numKeys);
	int xpos = key * (parent->getDim().Width / numKeys) + (parent->getDim().Width / (4.0 * numKeys));

	int hitPos = (*viewTime - hitobject->getTime());
	int correction = std::max(0.0, (double)(((OsuManiaRenderer*)parent)->getStartTime() - hitobject->getTime()) * (*zoom));

	ypos = (hitPos * (*zoom) + parent->getDim().Height) - height - correction;

	absXpos = parent->getPos().X + xpos;
	absYpos = parent->getPos().Y + ypos;
}

void HitNote::UpdateAbsDim(Window& _win)
{
	if (hitobject->getHitobjectType() & HITOBJECTYPE::MANIALONG)
	{
		width = 50 * _widthRatio_;

		int startTime = std::max(hitobject->getTime(), ((OsuManiaRenderer*)parent)->getStartTime());
		int endTime = std::min(hitobject->slider->getEndTime(), ((OsuManiaRenderer*)parent)->getEndTime());

		height = (double)(endTime - startTime) * (*zoom) + 15;
	}
	else
	{
		width = 50 * _widthRatio_;
		height = 15;
	}
}