#ifndef TAPEVENINTERVALS_H
#define TAPEVENINTERVALS_H

#include "../Analyzer.h"

class Analyzer_TapEventIntervals : public Analyzer
{
	public:
		Analyzer_TapEventIntervals();
		virtual ~Analyzer_TapEventIntervals();
		
		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};

#endif