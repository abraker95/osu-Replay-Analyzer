#include "GamemodeRenderer.h"
#include "../../ui/drawUtils.h"

double _widthRatio_, _heightRatio_;

GamemodeRenderer::GamemodeRenderer(int _xpos, int _ypos, int _width, int _height, Play* _play, int* _viewTime) : GuiObj(_xpos, _ypos, _width, _height)
{
	gamemode = _play->beatmap->getGamemode();
	InitRenderer(_play, _viewTime);
}

GamemodeRenderer::~GamemodeRenderer()
{
	if(stdRenderer != nullptr)
		delete stdRenderer;

	/*
	if(taikoRenderer != nullptr)
		delete taikoRenderer;
	*/

	/*
	if(catchRenderer != nullptr)
		delete chatchRenderer;
	*/

	if (maniaRenderer != nullptr)
		delete maniaRenderer;

	/*
	if(dodgeRenderer != nullptr)
		delete dodgeRenderer;
	*/
}

void GamemodeRenderer::UpdateRatios()
{
	_widthRatio_ = (double)this->width / 640.0;
	_heightRatio_ = (double)this->height / 480.0;
}

void GamemodeRenderer::setGamemode(GAMEMODE _gamemode)
{
	// \TODO
	gamemode = _gamemode;
}

// --------- [PRIVATE] -----------

void GamemodeRenderer::InitRenderer(Play* _play, int* _viewTime)
{
	switch (gamemode)
	{
		case OSU_STANDARD:
			stdRenderer = new OsuStdRenderer(_play, _viewTime, this);
				stdRenderer->addClipDimTo(GuiObj::RIGHT);
				stdRenderer->addClipDimTo(GuiObj::BTM);
				stdRenderer->SetLayers(OsuStdRenderer::VISIBLE | OsuStdRenderer::PATHS | OsuStdRenderer::REPLAY);
			break;

		case OSU_TAIKO:
			// \TODO
			break;

		case OSU_CATCH:
			// \TODO
			break;

		case OSU_MANIA:
			maniaRenderer = new OsuManiaRenderer(_play, _viewTime, this);
				maniaRenderer->addClipDimTo(GuiObj::CENTERX);
				maniaRenderer->addClipDimTo(GuiObj::BTM);
				maniaRenderer->setPos(this->width / 2 - 300, 0);  /// \TODO: Set width and pos based on key amount
				maniaRenderer->SetLayers(OsuManiaRenderer::VISIBLE | OsuManiaRenderer::REPLAY | OsuManiaRenderer::TIMINGS | OsuManiaRenderer::HITIMINGS | OsuManiaRenderer::TAPPINGDIFFS);
			break;

		case OSU_DODGE:
			// ;-;
			break;

		default:
			break;
	}
}

void GamemodeRenderer::UpdateInternal(Window& _win)
{
	UpdateRatios();
}

void GamemodeRenderer::Draw(Window& _win)
{
	_win.driver->draw2DRectangleOutline(core::rect<s32>(absXpos, absYpos, absXpos + width, absYpos + height),
				video::SColor(255, 255, 255, 255));

	if (gamemode == GAMEMODE::GAMEMODE_ERROR)
	{
		dimension2d<u32> dim = _win.font->getDimension(core::stringw("BEATMAP NOT LOADED!!!").c_str());
		_win.font->draw(core::stringw("BEATMAP NOT LOADED!!!"), core::rect<s32>(absXpos + width / 2 - dim.Width/2, absYpos + height / 2 - dim.Height / 2, 100, 10), video::SColor(255, 255, 255, 255));
		return;
	}
		
	switch (gamemode)
	{
		case OSU_STANDARD:
			RenderOsuStandard(_win);
			break;

		case OSU_CATCH:
			RenderOsuCatch(_win);
			break;

		case OSU_TAIKO:
			RenderOsuTaiko(_win);
			break;

		case OSU_MANIA:
			RenderOsuMania(_win);
			break;

		case OSU_DODGE:
			RenderOsuDodge(_win);
			break;
		
		default:
			break;
	}
}

void GamemodeRenderer::RenderOsuStandard(Window& _win)
{
	if(stdRenderer != nullptr)
		stdRenderer->Update(_win);
}

void GamemodeRenderer::RenderOsuCatch(Window& _win)
{
	/*
	if (catchRenderer != nullptr)
		catchRenderer->Update(_win);
	*/
}

void GamemodeRenderer::RenderOsuTaiko(Window& _win)
{
	/*
	if (taikoRenderer != nullptr)
		taikoRenderer->Update(_win);
	*/
}

void GamemodeRenderer::RenderOsuMania(Window& _win)
{
	if (maniaRenderer != nullptr)
		maniaRenderer->Update(_win);
}

void GamemodeRenderer::RenderOsuDodge(Window& _win)
{
	/*
	if (dodgeRenderer != nullptr)
		dodgeRenderer->Update(_win);
	*/
}