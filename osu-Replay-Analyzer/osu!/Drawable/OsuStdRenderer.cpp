#include "OsuStdRenderer.h"
#include "../Filestructure/Play.h"
#include "../../ui/drawUtils.h"

#include "../osu_standard.h"

OsuStdRenderer::OsuStdRenderer(Play* _play, long* _viewTime, GuiObj* _parent) : GuiObj(0, 0, _parent->getDim().Width, _parent->getDim().Height, _parent)
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
	std::pair<int, int> visibilityTimes = OSUSTANDARD::getIndicesVisibleAt(play, *viewTime, 0.3);
	for (int i = visibilityTimes.first; i < visibilityTimes.second; i++)
	{
		hitCircles[i]->Update(_win);
	}

	double widthRatio = getDim().Width / 512.0;
	double heightRatio = getDim().Height / 386.0;
	
	int i = play->beatmap->getHitobjects().Find(*viewTime, true)[0];
	DrawArc(_win, play->beatmap->getHitobjects()[i]->getPos().X*widthRatio + this->absXpos, play->beatmap->getHitobjects()[i]->getPos().Y*heightRatio + this->absYpos, 5, SColor(255, 255, 0, 0));
}

void OsuStdRenderer::RenderPaths(Window& _win)
{
	Database<Hitobject>& hitobjects = play->beatmap->getHitobjects();
	
	int i = hitobjects.Find(*viewTime, true)[0];
	if (i - 1 < 0)	return;

	double widthRatio = getDim().Width / 512.0;
	double heightRatio = getDim().Height / 386.0;

	long time = hitobjects[i - 1]->getTime() - 1;
	osu::TIMING tickPoints[5];
		tickPoints[0] = OSUSTANDARD::getNextTickPoint(hitobjects, time, &time);
		tickPoints[1] = OSUSTANDARD::getNextTickPoint(hitobjects, time, &time);
		tickPoints[2] = OSUSTANDARD::getNextTickPoint(hitobjects, time, &time);
		tickPoints[3] = OSUSTANDARD::getNextTickPoint(hitobjects, time, &time);
		tickPoints[4] = OSUSTANDARD::getNextTickPoint(hitobjects, time, &time);

	for (int i = 0; i < 4; i++)
	{
		vector2di p1, p2;
		p1 = vector2di(tickPoints[i].pos.X*widthRatio + this->absXpos, tickPoints[i].pos.Y*heightRatio + this->absYpos);
		p2 = vector2di(tickPoints[i + 1].pos.X*widthRatio + this->absXpos, tickPoints[i + 1].pos.Y*heightRatio + this->absYpos);

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

	SColor color;
	if (frame.key != 0) color = SColor(255, 200, 200, 255);
	else				color = SColor(255, 100, 100, 255);

	DrawArc(_win, cursorXpos, cursorYpos, 5, color);	
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