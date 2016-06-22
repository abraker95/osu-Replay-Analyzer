#include "TimingPoint.h"

TimingPoint::TimingPoint()
{

}

double TimingPoint::getBPM(bool raw)
{
	if (raw)
		return 60000 / this->beatInterval;
	else
		return this->BPM;
}