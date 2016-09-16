#ifndef ENDURANCE_H
#define Endurance_H

#include "../../Analyzer.h"

class Analyzer_Endurance : public Analyzer
{
	public:
		Analyzer_Endurance();
		virtual ~Analyzer_Endurance();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};


#endif // !ENDURANCE_H
