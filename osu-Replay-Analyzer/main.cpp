#include <vector>
#include <fstream>
#include <string>
#include <tuple>
#include <functional>
#include <thread>

#include "irrlicht/include/irrlicht.h"

#include "utils/mathUtils.h"

#include "ui/Window.h"
#include "ui/drawUtils.h"
#include "ui/Slider.h"
#include "ui/Button.h"
#include "ui/Scrollbar.h"
#include "ui/Pane.h"
#include "ui/Dialog.h"

#include "osu!/osuCalc.h"

#include "osu!/Filestructure/Play.h"
#include "osu!/Score/ScoreEngine.h"

#include "osu!/Drawable/GamemodeRenderer.h"
#include "osu!/Drawable/Hitcircle.h"
#include "osu!/Drawable/TimingGraph.h"
#include "osu!/Drawable/HitTimingGraph.h"
#include "osu!/Drawable/HitTimingMarker.h"
#include "osu!/Drawable/StatGraph.h"

#include "osu!/Skills/SkillEngine.h"

using namespace std;

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

#ifdef _IRR_WINDOWS_
	#pragma comment(lib, "irrlicht/lib/Win32-visualstudio/Irrlicht.lib")
	//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")  // show/hide console
#endif

/*
void DrawDebug(Window &_win, std::vector<Hitcircle> &_hitcircles, int _time, double _AR, double _CS, bool _hidden)
{
	int index = getHitcircleAt(_hitcircles, _time) + 1;

	if (index > _hitcircles.size() - 1) 	// if it's beyond the end
	{
		//
	}
	else if (index < 3) // if it's at or before the beggining
	{
		//
	}
	else
	{
		std::vector<std::tuple<int, int, int, int>> pattern;

		if (_hitcircles[index - 1].isSlider())
			if (_time < _hitcircles[index - 1].getEndTime())
				index--;

		pattern = getPattern(_hitcircles, _time, index, _CS, 3);

		DrawArc(_win, std::get<Hitcircle::XPOS>(pattern[0]), std::get<Hitcircle::YPOS>(pattern[0]), 5, SColor(255, 0, 255, 128), 0.0, 2.0*M_PI);
		DrawArc(_win, std::get<Hitcircle::XPOS>(pattern[1]), std::get<Hitcircle::YPOS>(pattern[1]), 5, SColor(255, 0, 255, 128), 0.0, 2.0*M_PI);
		DrawArc(_win, std::get<Hitcircle::XPOS>(pattern[2]), std::get<Hitcircle::YPOS>(pattern[2]), 5, SColor(255, 0, 255, 128), 0.0, 2.0*M_PI);
	}



	std::pair<int, int> range = getIndicesVisibleAt(_hitcircles, _time, _AR, _hidden, 0.3);

	/// \TODO: to be applied when doing slider intersection check
	//getPattern(_hitcircles, timeEnd, _index, _CS, getNumVisibleAt(_hitcircles, _AR, _hidden, 0.1));

	for (int i = range.first + 1; i < range.second; i++)
	{
		std::pair<vector2di, vector2di> path;
			path.first = _hitcircles[i - 1].getPos();
			path.second = _hitcircles[i].getPos();

		for (int j = i + 1; j < range.second; j++)
		{
			std::pair<vector2di, vector2di> checkPath;
				checkPath.first = _hitcircles[j - 1].getPos();
				checkPath.second = _hitcircles[j].getPos();

			_win.driver->draw2DLine(path.first, path.second, SColor(255, 0, 255, 128));
			_win.driver->draw2DLine(checkPath.first, checkPath.second, SColor(255, 0, 255, 128));
		}
	}


}*/

std::pair<std::string, std::string> getAnalyzerTXT()
{
	const std::string file = "analyze.txt";
	std::string beatmapFile, replayFile;

	std::ifstream analyzeFile(file);
	if (analyzeFile.is_open())
	{
		getline(analyzeFile, beatmapFile);
		getline(analyzeFile, replayFile);
		analyzeFile.close();
	}
	else
	{
		beatmapFile = "";
		replayFile = "";
	}

	return std::pair<std::string, std::string>(beatmapFile, replayFile);
}

int main()
{
	const int RESX = 171+640;
	const int RESY =  50+480;

	Window win(RESX, RESY);
		win.device->setWindowCaption(L"osu! Replay Analyzer");
		win.device->setResizable(true);

	double CS = 4;
	double AR = 9;
	int viewTime = 0;

	bool hidden = false;

	Slider csSlider(-75, 100, 120, 10);
		csSlider.setRange(0, 10);
		csSlider.ClipPosTo(GuiObj::TOPRIGHT);

	Slider arSlider(-75, 150, 120, 10);
		arSlider.setRange(0, 11);
		arSlider.setVal(10 + 1.0 / 3.0);
		arSlider.ClipPosTo(GuiObj::TOPRIGHT);

	Slider hdSlider(-75, 200, 120, 10);
		hdSlider.setRange(0, 1);
		hdSlider.ClipPosTo(GuiObj::TOPRIGHT);

	Button btnBeatmap(-130, 10, 100, 10);
		btnBeatmap.ClipPosTo(GuiObj::TOPRIGHT);
		btnBeatmap.setDepth(1);

	Button btnReplay(-25, 10, 100, 10);
		btnReplay.ClipPosTo(GuiObj::TOPRIGHT);
		btnReplay.setDepth(1);

	/// \TODO: Open file dialog box

	//Scrollbar scrollbar(10, 10, Scrollbar::VERTICAL, 300, 550);
	//Pane pane(50, 50, 500, 300, 500, 500);
	//	pane.addObj(&scrollbar);	*/

	//Dialog dialogBox(20, 20, 500, 300);

	std::pair<std::string, std::string> toAnalyze;
	Play play;
	SkillEngine skillEngine(&play);

	if (play.replay->getNumFrames() == 0)
		cout << "No Replay loaded!" << endl;

	double speedScore = skillEngine.getScore(SkillEngine::ANY_SPEEDCONTROL);
	double speedSkill = skillEngine.getSkill(SkillEngine::ANY_SPEEDCONTROL);
	double speedDiff = skillEngine.getDiff(SkillEngine::ANY_SPEEDCONTROL);

	cout << "Tapping | Score: " << speedScore << "   Skill: " << speedSkill << "   Max diff: " << speedDiff << endl;

	TimingGraph timingGraph(0, -20, win, &play);
		timingGraph.ClipPosTo(GuiObj::BTMLEFT);
		timingGraph.addClipDimTo(GuiObj::RIGHT);
		timingGraph.DisableLayer(TimingGraph::LAYER::HITOBJECT_VISIBILTITY);

	HitTimingGraph hitTimingGraph(-235, -100, &play);
		hitTimingGraph.ClipPosTo(GuiObj::BTMRIGHT);

	HitTimingMarker hitTimingMarker(0, 0, &play, &viewTime);
		hitTimingMarker.ClipPosTo(GuiObj::BTMLEFT);

	GamemodeRenderer renderer(10, 10, win.getDimentions().Width, win.getDimentions().Height, &play, &viewTime);
		renderer.ClipPosTo(GuiObj::TOPLEFT);
		renderer.addClipDimTo(GuiObj::BTM);
		renderer.addClipDimTo(GuiObj::RIGHT);
		renderer.setMargin(250, 100);		

	StatGraph graphs(0, -50, &viewTime);
		graphs.ClipPosTo(GuiObj::BTMLEFT);
		graphs.addClipDimTo(GuiObj::RIGHT);
		graphs.Init();

	/// \TODO: Are there no memory leaks?
	/// \TODO: Implement a textbox gui object (OH BOY... >.>)
	while (win.device->run())
	{
		win.reciever.Update();
		win.updateFPS();

		// update stuff
		AR = arSlider.getVal();
		CS = csSlider.getVal();
		hidden = hdSlider.getVal() < 0.5? false: true;

		viewTime = timingGraph.getViewTime();

		// render stuff
		win.driver->beginScene(true, true, SColor(255, 0, 0, 0));

			/// \TODO: Angle drawing screws up on sliders
			//DrawLine(win, circles[0], circles[1]);
			/*int time2index = getHitcircleAt(circles, time_ms) + 1;
			if (BTWN(1, time2index, circles.size() - 2))
				DrawAngle(win, circles[time2index - 1], circles[time2index], circles[time2index + 1]);*/
			
			//cout << getNumIntersections(circles, time2index, AR2ms(AR)) <<endl;
			//getNumIntersections(circles, time2index, AR2ms(AR));
			win.font->draw(core::stringw(win.reciever.GetMouseState().positionCurr.X) + ", " + core::stringw(win.reciever.GetMouseState().positionCurr.Y), core::rect<s32>(RESX - 100, 40, 100, 10), video::SColor(255, 255, 255, 255));


			double readDiff  = skillEngine.getDiff(SkillEngine::ANY_READING);
			win.font->draw(core::stringw("Reading diff: ") + core::stringw(readDiff), core::rect<s32>(RESX - 225, 20, 100, 10), video::SColor(255, 255, 255, 255));

			UpdateGuiObjs(win);

			if (btnBeatmap.isTriggered())
			{
				play.LoadBeatmap(getAnalyzerTXT().first);
				skillEngine.Generate(&play);
				renderer.InitRenderer(&play, &viewTime);
				graphs.Init();
			}

			if (btnReplay.isTriggered())
			{
				play.LoadReplay(getAnalyzerTXT().second);
				skillEngine.Generate(&play);
				renderer.InitRenderer(&play, &viewTime);
				graphs.Init();
				hitTimingGraph.Init();
			}
				

		win.driver->endScene();
		win.device->yield();
	}

	win.device->drop();
	return 0;
}
