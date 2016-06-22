#ifndef GUIOBJ_H
#define GUIOBJ_H

#include <string>
#include "Window.h"

class GuiObj
{
//	friend class GuiObj;

	public:
		enum CLIPPOS
		{
							  NONE = 0,
			TOPLEFT = 1,    TOPCENTER = 2,  TOPRIGHT = 3,
			MIDLEFT = 4,	MIDCENTER = 5,	MIDRIGHT = 6,
			BTMLEFT = 7,	BTMCENTER = 8,	BTMRIGHT = 9,
		};
		
		enum CLIPDIM
		{
			BTM = 1, CENTERY = 2, CENTERX = 4, RIGHT = 8
		};

		void Update(Window& _win);

		void setPos(int _xpos, int _ypos);
		void setMargin(int _right, int _btm);
		void setParent(GuiObj* _obj);
		void setVisible(bool _visible);

		core::vector2di getPos() const;
		core::vector2di getMid() const;
		core::dimension2di getDim() const;


		bool isMouseOnObj(Window& _win, bool _only = false);
		bool hasParent();
		bool isParent(GuiObj* _obj);
		
		int getID() const; 
		void UpdateID();

		void ClipPosTo(CLIPPOS _area);  // \TODO: Allow cliping to any object
		void addClipDimTo(CLIPDIM _dir);

	protected:
		GuiObj(int _xpos, int _ypos, int _width, int _height, GuiObj* _parent = nullptr);
		virtual ~GuiObj();

		int xpos, ypos;
		int absXpos, absYpos;
		int width, height;
		int marginRight, marginBtm;
		
		GuiObj* parent;
		//std::vector<GuiObj*> children;
		int id;


		CLIPPOS clipPos;
		int clipDim;
		bool visible;
		std::string guiType;

	private:
		virtual void UpdateInternal(Window& _win) = 0;
		virtual void Draw(Window& _win) = 0;

		virtual void UpdateAbsPos(Window& _win);
		virtual void UpdateAbsDim(Window& _win);
};


void UpdateGuiObjs(Window& _win);
void SetRelativeGuiLayer(GuiObj* _top, GuiObj* _btm);

#endif