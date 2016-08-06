#ifndef COLORTABLE_H
#define COLORTABLE_H

#include <vector>
#include "../irrlicht/include/SColor.h"

class ColorTable
{
	public:
		ColorTable();
		virtual ~ColorTable();

		void AddValueMap(double _val, irr::video::SColor _color);
		irr::video::SColor getColor(double _val);

	private:
		std::vector<std::pair<double, irr::video::SColor>> table; // higher than currValue, but before next value corresponds to the color
};

#endif // !COLORTABLE_H