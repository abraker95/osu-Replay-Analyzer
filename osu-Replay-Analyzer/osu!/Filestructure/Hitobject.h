#ifndef HITOBJECT_H
#define HITOBJECT_H

#include <vector>
#include <memory>

#include "../../irrlicht/include/vector2d.h"

#include "../../utils/mathUtils.h"
#include "../osuEnums.h"

#include "Bezier.h"

class Beatmap;
class SliderHitObject;

enum CurveType
{
	PerfectCurve = 'P',
	BezierCurve  = 'B',
	LinearCurve  = 'L',
	CatmullCurve = 'C'
};


class Hitobject
{
	friend class Play;

	public:
		Hitobject(std::vector<std::string> &_objectData);
		Hitobject(Hitobject&&) = default;
		Hitobject(Hitobject const&) = default;

		virtual ~Hitobject();

		bool IsHitObjectType(int type);
		bool isHitobjectLong();

		int getHitobjectType();
		long Hitobject::getCloserTime(long _time);

		SliderHitObject* getSlider();

		irr::core::vector2d<double> getPos() const;
		void setPos(irr::core::vector2d<double> _pos);

		int getTime() const;
		int getEndTime();
		void setTime(Beatmap* _beamap, int _time);

		double GetCircleOverlapPercentage(int CS, Hitobject &c1);

	protected:
		int time, index;
		irr::core::vector2d<double> pos;
		int type;
		Beatmap* beatmap;

		void ProcessHitobjectData(std::vector<std::string> &_objectData);
};

#endif