#ifndef TAPDEVIATION_H
#define TAPDEVIATION_H

#include "../Analyzer.h"

class Analyzer_TapDeviation : public Analyzer
{
	public:
		Analyzer_TapDeviation();
		virtual ~Analyzer_TapDeviation();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};


#endif
