#ifndef TIMINGPOINT_H
#define TIMINGPOINT_H

class TimingPoint
{
	friend class Beatmap;

	public:
		TimingPoint();

		int offset = -1;
		double beatInterval = 0;
		double beatLength = 0;
		int meter = -1;
		bool inherited = 0;

		double sm = -1; // processed

		double getBPM(bool raw = false);

	private:
		double BPM;
};

#endif