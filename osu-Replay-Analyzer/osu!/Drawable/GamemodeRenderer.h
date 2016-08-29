#ifndef GAMEMODERENDERER
#define GAMEMODERENDERER

#include "../../ui/Window.h"
#include "../../ui/GuiObj.h"
#include "../osuEnums.h"
#include "../Filestructure/Play.h"
#include "Hitcircle.h"

#include "OsuStdRenderer.h"
#include "OsuManiaRenderer.h"
//#include "OsuCatchRenderer.h"
//#include "OsuTaikoRenderer.h"
//#include "OsuDodgeRenderer.h"

extern double _widthRatio_, _heightRatio_;

class GamemodeRenderer : public GuiObj 
{
	public:
		GamemodeRenderer(int _xpos, int _ypos, int _width, int _hight, Play* _play, int* _viewTime);
		void InitRenderer(Play* _play, int* _viewTime);
		
		virtual ~GamemodeRenderer();
		void setGamemode(GAMEMODE _gamemode);

	private:
		virtual void UpdateInternal(Window& _win);
		void UpdateRatios();

		virtual void Draw(Window& _win);

		void RenderOsuStandard(Window& _win);
		void RenderOsuCatch(Window& _win);
		void RenderOsuTaiko(Window& _win);
		void RenderOsuMania(Window& _win);
		void RenderOsuDodge(Window& _win);

		void DestroyRenderers();

		GAMEMODE gamemode;
		OsuStdRenderer* stdRenderer;
		OsuManiaRenderer* maniaRenderer;
};

#endif