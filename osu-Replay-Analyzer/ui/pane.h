#ifndef PANE_H
#define PANE_H

#include <vector>

#include "Window.h"
#include "GuiObj.h"
#include "Scrollbar.h"

class Pane: public GuiObj
{
	public:
		Pane(int _xpos, int _ypos, int _width, int _height, int _virtWidth, int _virtHight, GuiObj* _parent = nullptr);
		virtual ~Pane();

		void addObj(GuiObj* _obj);

		void setSize(int _width, int _height);
		void setVirtSize(int _width, int _height);

	private:
		Scrollbar *verScrollbar, *horScrollbar;
		int virtWidth, virtHeight;

		std::vector<GuiObj*> objs;

		virtual void Draw(Window& _win);
		virtual void UpdateInternal(Window& _win);
};

#endif