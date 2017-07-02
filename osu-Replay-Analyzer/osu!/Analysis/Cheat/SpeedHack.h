#ifndef SPEEDHACK_H
#define SPEEDHACK_H

#include "../Analyzer.h"

class Analyzer_SpeedHack : public Analyzer
{
	public:
		Analyzer_SpeedHack();
		virtual ~Analyzer_SpeedHack();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};


#endif // !SPEEDHACK_H