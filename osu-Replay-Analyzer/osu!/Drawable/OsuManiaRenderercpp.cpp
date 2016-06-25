#include "OsuManiaRenderer.h"
#include "GamemodeRenderer.h"

#include "../Filestructure/ScoreEngine.h"
#include "../Filestructure/Play.h"
#include "../../ui/drawUtils.h"

OsuManiaRenderer::OsuManiaRenderer(Play* _play, int* _viewTime, GuiObj* _parent) : GuiObj(0, 0, _parent->getDim().Width, _parent->getDim().Height, _parent)
{
	play = _play;
	viewTime = _viewTime;
	zoom = 1.0;

	GenerateHitNotes();
}

OsuManiaRenderer::~OsuManiaRenderer()
{
	hitNotes.clear();
}

void OsuManiaRenderer::SetLayers(int _layer)
{
	layerState = (Layer)_layer;
}


// ---------- [PRIVATE] ----------

void OsuManiaRenderer::GenerateHitNotes()
{
	Beatmap* beatmap = play->beatmap;
	hitNotes.resize(beatmap->hitObjects.size());

	for (int i = 0; i < hitNotes.size(); i++)
	{
		hitNotes[i] = new HitNote(beatmap, beatmap->hitObjects[i], viewTime, &zoom);
			hitNotes[i]->setParent(this);
	}
}

void OsuManiaRenderer::UpdateInternal(Window& _win)
{
	UpdateUserControls(_win);
}

void OsuManiaRenderer::Draw(Window& _win)
{
	_win.driver->draw2DRectangleOutline(recti(absXpos, absYpos, absXpos + width, absYpos + height), SColor(255, 255, 255, 255));

	if (layerState & VISIBLE)
	{
		this->RenderVisible(_win);
	}

	if (layerState & DENSITY)
	{
		this->RenderDensities(_win);
	}

	if (layerState & REPLAY)
	{
		this->RenderReplay(_win);
	}

	if (layerState & TIMINGS)
	{
		this->RenderTimings(_win);
	}

	if (layerState & HITIMINGS)
	{
		this->RenderHitTimings(_win);
	}

	if (layerState & TAPPINGDIFFS)
	{
		this->RenderTappingDiffs(_win);
	}
}

void OsuManiaRenderer::RenderVisible(Window& _win)
{
	Beatmap* beatmap = play->beatmap;
	std::vector<Hitobject*>& hitobjects = beatmap->hitObjects;

	/// \TODO: Faster object search
	for (int i = 0; i < hitobjects.size(); i++)
	{
		bool HoldStart = BTWN(hitobjects[i]->getTime(), getStartTime(), hitobjects[i]->getEndTime());
		bool HoldEnd = BTWN(hitobjects[i]->getTime(), getEndTime(), hitobjects[i]->getEndTime());
		bool circle = BTWN(getStartTime(), hitobjects[i]->getTime(), getEndTime());
		
		if (HoldStart || HoldEnd || circle)
			hitNotes[i]->Update(_win);
	}
}

void OsuManiaRenderer::RenderDensities(Window& _win)
{
	/// \TODO
}

void OsuManiaRenderer::RenderReplay(Window& _win)
{
	Replay* replay = play->replay;
	std::tuple<irr::core::vector2df, int> data = replay->getDataAt(*viewTime);
	int hitYpos = this->height;
	const int KEYS = play->beatmap->getDiff().cs;

	for (int key = 0; key < KEYS; key++)
	{
		if ((int)std::get<0>(data).X & (1 << key))
		{
			int hitXpos = key * (this->width / KEYS) + (this->width / (4.0 * KEYS));
			_win.driver->draw2DRectangle(SColor(255, 100, 100, 100), rect<s32>(absXpos + hitXpos, absYpos + hitYpos, absXpos + hitXpos + 50, absYpos + hitYpos + 15));
		}
	}
}

void OsuManiaRenderer::RenderTimings(Window& _win)
{
	_win.font->draw(core::stringw(getEndTime()), core::rect<s32>(absXpos + width, absYpos, 100, 10), video::SColor(255, 255, 255, 255));
	_win.font->draw(core::stringw(getStartTime()), core::rect<s32>(absXpos + width, absYpos + height, 100, 10), video::SColor(255, 255, 255, 255));
}

void OsuManiaRenderer::RenderHitTimings(Window& _win)
{
	const int KEYS = play->beatmap->getDiff().cs;

	for (ScoreEngine::TIMING timing : play->scoreEngine->accTimings)
	{
		if (BTWN(getStartTime(), timing.time + timing.data, getEndTime())) // hits
		{
			int hitXpos = timing.key * (this->width / KEYS) + (this->width / (4.0 * KEYS));

			int hitPos = (*viewTime) - timing.time - timing.data;
			int hitYpos = hitPos*zoom + height;

			_win.driver->draw2DRectangle(SColor(255, 255, 0, 255), rect<s32>(absXpos + hitXpos, absYpos + hitYpos, absXpos + hitXpos + 50, absYpos + hitYpos + 2));
		}
		else if (BTWN(getStartTime(), timing.time, getEndTime())) // misses
		{
			if (timing.data == INT_MAX)
			{
				int hitXpos = timing.key * (this->width / KEYS) + (this->width / (4.0 * KEYS));

				int hitPos = (*viewTime) - timing.time;
				int hitYpos = hitPos*zoom + height;

				_win.driver->draw2DRectangle(SColor(255, 255, 0, 0), rect<s32>(absXpos + hitXpos, absYpos + hitYpos, absXpos + hitXpos + 50, absYpos + hitYpos + 2));
			}	
		}
	}
}

void OsuManiaRenderer::RenderTappingDiffs(Window& _win)
{
	const int KEYS = play->beatmap->getDiff().cs;

	for (ScoreEngine::TIMING timing : play->scoreEngine->tappingStrains)
	{
		if (BTWN(getStartTime(), timing.time, getEndTime())) // hits
		{
			int hitXpos = timing.key * (this->width / KEYS) + (this->width / (4.0 * KEYS));

			int hitPos = (*viewTime) - timing.time;
			int hitYpos = hitPos*zoom + height;

			_win.font->draw(core::stringw(timing.data), core::rect<s32>(absXpos + hitXpos, absYpos + hitYpos, 100, 10), video::SColor(255, 255, 255, 255));
		}
	}
}

void OsuManiaRenderer::UpdateAbsDim(Window& _win)
{
	width = parent->getDim().Width / 2.0;
	height = parent->getDim().Height;
}

void  OsuManiaRenderer::UpdateAbsPos(Window& _win)
{
	xpos = parent->getDim().Width / 2.0 - this->width / 2.0;
	ypos = 0;

	absXpos = parent->getPos().X + xpos;
	absYpos = parent->getPos().Y + ypos;
}

void OsuManiaRenderer::UpdateUserControls(Window& _win)
{
	if (_win.reciever.IsKeyDown(KEY_KEY_Z))
	{
		if (this->isMouseOnObj(_win))
		{
			UpdateZoom(_win);
		}
	}
}

void OsuManiaRenderer::UpdateZoom(Window& _win)
{
	double step = -zoom / 10.0; // amount of units to zoom in/out by
	double newRes = zoom + step*_win.reciever.getWheel();
	if (newRes >= 0)
		zoom = newRes;
}

int OsuManiaRenderer::getStartTime()
{
	return *viewTime;  // what gets hit at the receptor
}

int OsuManiaRenderer::getEndTime()
{
	const int NOTE_THICKNESS = 15;
	const int VIRT_STAGE_HEIGHT = 480;

	return *viewTime + px2Time(VIRT_STAGE_HEIGHT*_heightRatio_ - NOTE_THICKNESS);  // What appears at the top
}

int OsuManiaRenderer::Time2px(double _time)
{
	return _time*zoom;
}

int OsuManiaRenderer::px2Time(double _px)
{
	return _px/zoom;
}