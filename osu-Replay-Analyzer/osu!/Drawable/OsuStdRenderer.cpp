#include "OsuStdRenderer.h"
#include "../Filestructure/Play.h"
#include "../../ui/drawUtils.h"

OsuStdRenderer::OsuStdRenderer(Play* _play, int* _viewTime, GuiObj* _parent) : GuiObj(0, 0, _parent->getDim().Width, _parent->getDim().Height, _parent)
{
	play = _play;
	viewTime = _viewTime;

	hitCircles.resize(_play->beatmap->getHitobjects().size());

	GenerateHitcircles();
}

OsuStdRenderer::~OsuStdRenderer()
{
	ClearHitcircles();
}

void OsuStdRenderer::SetLayers(int _layer)
{
	layerState = (Layer)_layer;
}


// ---------- [PRIVATE] ----------

void OsuStdRenderer::ClearHitcircles()
{
	for (Hitcircle* circle : hitCircles)
		if (circle != nullptr)
			delete circle;

	hitCircles.clear();
	std::vector<Hitcircle*>().swap(hitCircles);
}

void OsuStdRenderer::GenerateHitcircles()
{
	ClearHitcircles();

	Beatmap* beatmap = play->beatmap;
	hitCircles.resize(beatmap->getHitobjects().size());

	for (int i = 0; i < hitCircles.size(); i++)
	{
		hitCircles[i] = new Hitcircle(play->getMod(), beatmap->getHitobjects()[i], viewTime);
		hitCircles[i]->setParent(this);
	}
}

void OsuStdRenderer::UpdateInternal(Window& _win)
{
	
}

void OsuStdRenderer::Draw(Window& _win)
{
	if (play->beatmap->getGamemode() == GAMEMODE::GAMEMODE_ERROR)
		return;

	if (layerState & VISIBLE)    this->RenderVisible(_win);
	if (layerState & PATHS)      this->RenderPaths(_win);
	if (layerState & DENSITY)    this->RenderDensities(_win);
	if (layerState & REPLAY)     this->RenderReplay(_win);
	if (layerState & REPLAYPATH) this->RenderReplayPath(_win);
}

void OsuStdRenderer::RenderVisible(Window& _win)
{
	Beatmap* beatmap = play->beatmap;
	std::pair<int, int> visibilityTimes = beatmap->getIndicesVisibleAt(*viewTime, 0.3);

	for (int i = visibilityTimes.first; i < visibilityTimes.second; i++)
	{
		hitCircles[i]->Update(_win);
	}
}

void OsuStdRenderer::RenderPaths(Window& _win)
{
	std::vector<Hitobject*>& hitobjects = play->beatmap->getHitobjects();
	
	int i = std::min(OSUSTANDARD::FindHitobjectAt(hitobjects, *viewTime) + 1, (int) hitobjects.size() - 2);
	if (i >= (int)hitobjects.size() - 1)
		return;

	int numIter = 3;
	if (hitobjects[i + 1]->isHitobjectLong()) numIter++;

	std::vector<std::pair<irr::core::vector2d<double>, double>> points = OSUSTANDARD::getPattern(&hitobjects, numIter, 100, hitobjects[i + 1]->getTime(), true);

	double widthRatio = getDim().Width / 512.0;
	double heightRatio = getDim().Height / 386.0;

	if (points.size() < 2)
		return;

	for (int i = 0; i < points.size() - 1; i++)
	{
		vector2di p1, p2;
			p1 = vector2di(points[i].first.X*widthRatio + this->absXpos, points[i].first.Y*heightRatio + this->absYpos);
			p2 = vector2di(points[i + 1].first.X*widthRatio + this->absXpos, points[i + 1].first.Y*heightRatio + this->absYpos);
		
		_win.driver->draw2DLine(p1, p2, SColor(255, 255, 0, 0));
	}
}

void OsuStdRenderer::RenderDensities(Window& _win)
{
	// \TODO
}

void OsuStdRenderer::RenderReplay(Window& _win)
{
	Replay* replay = play->replay;
	osu::TIMING frame = replay->getFrameAt(*viewTime);

	double widthRatio = getDim().Width / 512.0;
	double heightRatio = getDim().Height / 386.0;

	int cursorXpos = (double)frame.pos.X*widthRatio + this->absXpos;
	int cursorYpos = (double)frame.pos.Y*heightRatio + this->absYpos;

	DrawArc(_win, cursorXpos, cursorYpos, 5, SColor(255, 100, 100, 255));	
}

void OsuStdRenderer::RenderReplayPath(Window& _win)
{
	Replay* replay = play->replay;
	osu::TIMING frame = replay->getFrameAt(*viewTime - 1000);

	double widthRatio = getDim().Width / 512.0;
	double heightRatio = getDim().Height / 386.0;

	struct CursorData
	{
		SColor color;
		vector2di pos;
	};

	const int samplePeriod = 1;

	for (int i = *viewTime - 1000; i < *viewTime; i += samplePeriod)
	{
		// get sample
		osu::TIMING prevData = replay->getFrameAt(i - samplePeriod);
		osu::TIMING currData = replay->getFrameAt(i);

		// cursor pos
		CursorData prev, curr;
			curr.pos = vector2di((double)frame.pos.X*widthRatio   + this->absXpos,
								 (double)frame.pos.Y*heightRatio  + this->absYpos);
			prev.pos = vector2di((double)frame.pos.X*widthRatio   + this->absXpos,
								 (double)frame.pos.Y*heightRatio  + this->absYpos);

		// mouse buttons
		bool left  = (frame.key == 1) || (frame.key == 5);
		bool right = (frame.key == 2) || (frame.key == 10);

			 if (left && right) curr.color = SColor(100, 255, 150, 255);
		else if (left)	  	    curr.color = SColor(100, 255, 150, 150);
		else if (right) 		curr.color = SColor(100, 150, 150, 255); 
		else				    curr.color = SColor(100, 0, 0, 0);		   // non

		_win.driver->draw2DLine(prev.pos, curr.pos, curr.color);
	}
}