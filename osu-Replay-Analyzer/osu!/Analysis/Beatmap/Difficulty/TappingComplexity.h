#ifndef TAPPINGCOMPLEXITY_H
#define TAPPINGCOMPLEXITY_H

#include "../../Analyzer.h"

class Analyzer_TappingComplexity : public Analyzer
{
	public:
		Analyzer_TappingComplexity();
		virtual ~Analyzer_TappingComplexity();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};

#endif