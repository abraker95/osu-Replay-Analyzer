#include "ColorTable.h"
#include <algorithm>

ColorTable::ColorTable() {}

ColorTable::~ColorTable() {}

bool sortTable(std::pair<double, irr::video::SColor> _val1, std::pair<double, irr::video::SColor> _val2)
{
	return _val1.first < _val2.first;
}

void ColorTable::AddValueMap(double _val, irr::video::SColor _color)
{
	table.push_back(std::pair<double, irr::video::SColor>(_val, _color));
	std::sort(table.begin(), table.end(), sortTable);
}

irr::video::SColor ColorTable::getColor(double _val)
{
	for (int i = table.size() - 1; i >= 0; i--)
	{
		if (_val >= table[i].first)
			return table[i].second;
	}
}