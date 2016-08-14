#include "GuiObj.h"
#include "../utils/mathUtils.h"

#include <vector>

//	#define DEBUG

std::vector<GuiObj*> guiEnv;
GuiObj* top;

void UpdateGuiObjs(Window& _win)
{
	for (int i = 0; i < guiEnv.size(); i++)
	{
		if (guiEnv[i] == nullptr) continue;				// make sure the guiObj is not deleted
		if (guiEnv[i]->hasParent() != false)  continue; // make sure it is not a child of another guiObj. Those are to be updated by the parent
			
		guiEnv[i]->Update(_win);
	}
}

void SetRelativeGuiLayer(GuiObj* _top, GuiObj* _btm)
{
	// \TODO: Object's children must be applied too
	if (_top->getID() < _btm->getID())
	{
		swap(guiEnv[_top->getID()], guiEnv[_btm->getID()]);
		_top->UpdateID();
		_btm->UpdateID();
	}
}

bool sortGui(GuiObj* i, GuiObj* j)
{
	return i < j;
}


void SortGuiObjs()
{
	std::sort(guiEnv.begin(), guiEnv.end(), sortGui);
}

int FindGuiObj(GuiObj* _guiObj)
{
	int start = 0;
	int end = guiEnv.size() - 1;
	int mid;

	while (start <= end)
	{
		mid = (start + end) / 2;

		if (guiEnv[mid] == _guiObj)
			return mid;

		if (_guiObj < guiEnv[mid])
			end = mid - 1;
		else
			start = mid + 1;
	}

	return -1;
}


GuiObj::GuiObj(int _xpos, int _ypos, int _width, int _height, GuiObj* _parent)
{
	xpos = _xpos;	ypos = _ypos;
	absXpos = 0;	absYpos = 0;
	width = _width;	height = _height;

	parent = _parent;
	visible = true;
	clipPos = CLIPPOS::NONEPOS;
	clipDim = CLIPDIM::NONEDIM;

	marginRight = 0;
	marginBtm = 0;

	guiEnv.push_back(this);
	id = guiEnv.size() - 1;
}

GuiObj::~GuiObj()
{
	int i = FindGuiObj(this);
	if (i == -1) return;

	if (guiEnv[i] == this)
		guiEnv.erase(guiEnv.begin() + i);
}

void GuiObj::Update(Window& _win)
{
	if (this->visible)
	{
		this->UpdateAbsPos(_win);
		this->UpdateAbsDim(_win);
		this->Draw(_win);
		this->UpdateInternal(_win);

		#ifdef DEBUG
			_win.driver->draw2DRectangleOutline(core::rect<s32>(absXpos, absYpos, absXpos + width, absYpos + height),
												video::SColor(255, 255, 0, 0));

			if (this->isMouseOnObj(_win))
				_win.driver->draw2DRectangleOutline(core::rect<s32>(absXpos, absYpos, absXpos + width, absYpos + height),
													video::SColor(255, 255, 150, 0));
			if (this->isMouseOnObj(_win, true))
				_win.driver->draw2DRectangleOutline(core::rect<s32>(absXpos, absYpos, absXpos + width, absYpos + height),
													video::SColor(255, 255, 255, 0));
			_win.font->draw(core::stringw(this->id), core::rect<s32>(absXpos+1, absYpos+1, 100, 10), video::SColor(255, 0, 0, 0));
		#endif
	}
}

void GuiObj::setPos(int _xpos, int _ypos)
{
	xpos = _xpos;
	ypos = _ypos;
}

void GuiObj::setMargin(int _right, int _btm)
{
	marginRight = _right;
	marginBtm = _btm;
}

void GuiObj::setParent(GuiObj* _parent)
{
	parent = _parent;
	//_parent->children.push_back(this);
}

core::vector2di GuiObj::getPos() const
{
	return vector2di(absXpos, absYpos);
}

core::vector2di GuiObj::getMid() const
{
	return vector2di(absXpos + width/2, absYpos + height/2);
}

core::dimension2di GuiObj::getDim() const
{
	return dimension2di(width, height);
}

bool GuiObj::hasParent()
{
	return !(this->parent == nullptr);
}

bool GuiObj::isParent(GuiObj* _obj)
{
	if (parent == nullptr)
		return false;
	else
		return (parent == _obj) || parent->isParent(_obj);
}

void GuiObj::ClipPosTo(CLIPPOS _area)
{
	clipPos = _area;
}

void GuiObj::addClipDimTo(CLIPDIM _dir)
{
	clipDim |= _dir;
}

void GuiObj::setVisible(bool _visible)
{
	visible = _visible;
}

int GuiObj::getID() const
{
	return id;
}

void GuiObj::UpdateID()
{
	for (int i = 0; i < guiEnv.size(); i++)
	{
		if (guiEnv[i] == this)
		{
			this->id = i;
			break;
		}
	}

	// \TODO: detect ones that are not found
}

/// \TODO: This is VERY ineffecient
bool GuiObj::isMouseOnObj(Window& _win, bool _only)
{
	position2di pos = _win.reciever.GetMouseState().positionCurr;
	bool onObj = (BTWN(this->absXpos, pos.X, this->absXpos + width) &&
				  BTWN(this->absYpos, pos.Y, this->absYpos + height));

	if (onObj)
	{
		if (_only == true)
		{
			if (top == nullptr)
				top = this;
			else
			{
				// if there is on another object on top of it, then
				// the cursor is not on this object
				bool isOnAnotherObj = false;
				for (int i = 0; i < guiEnv.size(); i++)
				{
					if (guiEnv[i] == this) continue; // Don't check against itself
					isOnAnotherObj |= guiEnv[i]->isMouseOnObj(_win);
				}
					
				if (isOnAnotherObj)
					onObj = false;
			}
		}
	}
		
	return onObj;
}

void GuiObj::UpdateAbsPos(Window& _win)
{
	if (parent != nullptr)
	{
		switch (this->clipPos)
		{
			case NONEPOS:
				absXpos = parent->absXpos + this->xpos;
				absYpos = parent->absYpos + this->ypos;
				break;

			case TOPLEFT:
				absXpos = parent->absXpos + this->xpos;
				absYpos = parent->absYpos + this->ypos;
				break;

			case TOPCENTER:
				absXpos = parent->absXpos + parent->width/2 + this->xpos - this->width/2;
				absYpos = parent->absYpos + this->ypos;
				break;

			case TOPRIGHT:
				absXpos = parent->absXpos + parent->width + this->xpos - this->width;
				absYpos = parent->absYpos + this->ypos;
				break;
				
			case MIDLEFT:
				absXpos = parent->absXpos + this->xpos;
				absYpos = parent->absYpos + parent->height/2 + this->ypos - this->height/2;
				break;

			case MIDCENTER:
				absXpos = parent->absXpos + parent->width/2 + this->xpos - this->width/2;
				absYpos = parent->absYpos + parent->height/2 + this->ypos - this->height/2;
				break;

			case MIDRIGHT:
				absXpos = parent->absXpos + parent->width + this->xpos - this->width;
				absYpos = parent->absYpos + parent->height/2 + this->ypos - this->height/2;
				break;

			case BTMLEFT:
				absXpos = parent->absXpos + this->xpos;
				absYpos = parent->absYpos + parent->height + this->ypos - this->height;
				break;
			
			case BTMCENTER:
				absXpos = parent->absXpos + parent->width/2 + this->xpos - this->width/2;
				absYpos = parent->absYpos + parent->height + this->ypos - this->height;
				break;
				
			case BTMRIGHT:
				absXpos = parent->absXpos + parent->width + this->xpos - this->width;
				absYpos = parent->absYpos + parent->height + this->ypos - this->height;
				break;
			
			default:
				absXpos = parent->absXpos + this->xpos;
				absYpos = parent->absYpos + this->ypos;
				break;
		}
	}
	else
	{
		switch (clipPos)
		{
			case NONEPOS:
				absXpos = this->xpos;
				absYpos = this->ypos;
				break;

			case TOPLEFT:
				absXpos = this->xpos;
				absYpos = this->ypos;
				break;

			case TOPCENTER:
				absXpos = _win.getDimentions().Width/2 + this->xpos - this->width/2;
				absYpos = this->ypos;
					break;

			case TOPRIGHT:
				absXpos = _win.getDimentions().Width + this->xpos - this->width;
				absYpos = this->ypos;
				break;
				
			case MIDLEFT:
				absXpos = this->xpos;
				absYpos = _win.getDimentions().Height/2 + this->ypos - this->height/2;
				break;

			case MIDCENTER:
				absXpos = _win.getDimentions().Width/2 + this->xpos - this->width/2;
				absYpos = _win.getDimentions().Height/2 + this->ypos - this->height/2;
				break;

			case MIDRIGHT:
				absXpos = _win.getDimentions().Width + this->xpos - this->width;
				absYpos = _win.getDimentions().Height/2 + this->ypos - this->height/2;
				break;

			case BTMLEFT:
				absXpos = this->xpos;
				absYpos = _win.getDimentions().Height + this->ypos - this->height;
				break;
			
			case BTMCENTER:
				absXpos = _win.getDimentions().Width/2 + this->xpos - this->width/2;
				absYpos = _win.getDimentions().Height + this->ypos - this->height;
				break;
			
			case BTMRIGHT:
				absXpos = _win.getDimentions().Width + this->xpos - this->width;
				absYpos = _win.getDimentions().Height + this->ypos - this->height;
				break;
			
			default:
				absXpos = this->xpos;
				absYpos = this->ypos;
				break;
		}
	}
}

void GuiObj::UpdateAbsDim(Window& _win)
{
	if (parent != nullptr)
	{
		if (this->clipDim & BTM)
		{
			this->height = (parent->absYpos + parent->height) - this->absYpos - this->marginBtm;
		}

		if (this->clipDim & CENTERY)
		{
			this->height = (parent->absYpos + parent->height/2) - this->absYpos - this->marginBtm;
		}

		if (this->clipDim & CENTERX)
		{
			this->width = (parent->absXpos + parent->width/2) - this->absXpos - this->marginRight;
		}

		if (this->clipDim & RIGHT)
		{
			this->width = (parent->absXpos + parent->width) - this->absXpos - this->marginRight;
		}
	}
	else
	{
		if (this->clipDim & BTM)
		{
			this->height = _win.getDimentions().Height - this->absYpos - this->marginBtm;
		}

		if (this->clipDim & CENTERY)
		{
			this->height = _win.getDimentions().Height/2 - this->absYpos - this->marginBtm;
		}

		if (this->clipDim & CENTERX)
		{
			this->width = _win.getDimentions().Width/2 - this->absXpos - this->marginRight;
		}

		if (this->clipDim & RIGHT)
		{
			this->width = _win.getDimentions().Width - this->absXpos - this->marginRight;
		}
	}
}