#include "OsuStdRenderer.h"
#include "../Filestructure/Play.h"
#include "../../ui/drawUtils.h"

OsuStdRenderer::OsuStdRenderer(Play* _play, int* _viewTime, GuiObj* _parent) : GuiObj(0, 0, _parent->getDim().Width, _parent->getDim().Height, _parent)
{
	play = _play;
	viewTime = _viewTime;

	hitCircles.resize(_play->beatmap->hitObjects.size());

	GenerateHitcircles();
}

OsuStdRenderer::~OsuStdRenderer()
{
	hitCircles.clear();
}

void OsuStdRenderer::SetLayers(int _layer)
{
	layerState = (Layer)_layer;
}


// ---------- [PRIVATE] ----------

void OsuStdRenderer::GenerateHitcircles()
{
	Beatmap* beatmap = play->beatmap;
	hitCircles.resize(beatmap->hitObjects.size());

	for (int i = 0; i < hitCircles.size(); i++)
	{
		hitCircles[i] = new Hitcircle(beatmap, beatmap->hitObjects[i], viewTime);
		hitCircles[i]->setParent(this);
	}
}

void OsuStdRenderer::UpdateInternal(Window& _win)
{
	
}

void OsuStdRenderer::Draw(Window& _win)
{
	if (layerState & VISIBLE)
	{
		this->RenderVisible(_win);
	}

	if (layerState & PATHS)
	{
		this->RenderPaths(_win);
	}

	if (layerState & DENSITY)
	{
		this->RenderDensities(_win);
	}	

	if (layerState & REPLAY)
	{
		this->RenderReplay(_win);
	}
}

void OsuStdRenderer::RenderVisible(Window& _win)
{
	Beatmap* beatmap = play->beatmap;
	std::vector<Hitobject*>& hitobjects = beatmap->hitObjects;
	std::pair<int, int> visibilityTimes = beatmap->getIndicesVisibleAt(*viewTime, 0.3);

	for (int i = visibilityTimes.first; i < visibilityTimes.second; i++)
	{
		hitCircles[i]->Update(_win);
	}
}

void OsuStdRenderer::RenderPaths(Window& _win)
{
	Beatmap* beatmap = play->beatmap;
	std::vector<Hitobject*>& hitobjects = beatmap->hitObjects;
	std::pair<int, int> visibilityTimes = beatmap->getIndicesVisibleAt(*viewTime, 0.3);

	// \TODO: Restore the getPattern function
	// \TODO: Non visible hitcircles don't update, causing the path to get locations of stuff that hasn't been applied to the aspect ratio
	int i = std::max(1, visibilityTimes.first);
	vector2di prevPos, currPos, nextPos;

	if (BTWN(0, i, hitCircles.size() - 1))
		currPos = vector2di(hitCircles[i + 0]->getMid().X, hitCircles[i + 0]->getMid().Y);
	else
		currPos = vector2di(-1, -1);

	if (i - 1 > 0)
		prevPos = vector2di(hitCircles[i - 1]->getMid().X, hitCircles[i - 1]->getMid().Y);
	else
		prevPos = currPos;

	if (i + 1 < hitCircles.size())
		nextPos = vector2di(hitCircles[i + 1]->getMid().X, hitCircles[i + 1]->getMid().Y);
	else
		nextPos = currPos;

	_win.driver->draw2DLine(prevPos, currPos, SColor(255, 255, 0, 0));
	_win.driver->draw2DLine(currPos, nextPos, SColor(255, 255, 0, 0));
}

void OsuStdRenderer::RenderDensities(Window& _win)
{
	// \TODO
}

void OsuStdRenderer::RenderReplay(Window& _win)
{
	Replay* replay = play->replay;
	std::tuple<irr::core::vector2df, int> data = replay->getDataAt(*viewTime);

	double widthRatio = getDim().Width / 640.0;
	double heightRatio = getDim().Height / 480.0;

	int cursorXpos = (double)std::get<0>(data).X*widthRatio + this->absXpos;
	int cursorYpos = (double)std::get<0>(data).Y*heightRatio + this->absYpos;

	DrawArc(_win, cursorXpos, cursorYpos, 5, SColor(255, 100, 100, 255));

	if((std::get<1>(data) == 1) || (std::get<1>(data) == 5))
		_win.driver->draw2DRectangle(SColor(255, 100, 100, 100), rect<s32>(600, 300, 610, 310));

	if((std::get<1>(data) == 2) || (std::get<1>(data) == 10))
		_win.driver->draw2DRectangle(SColor(255, 100, 100, 100), rect<s32>(620, 300, 630, 310));
}