#ifndef TAPSCORE_H
#define TAPSCORE_H

#include "../Analyzer.h"

class Analyzer_TapScore : public Analyzer
{
	public:
		Analyzer_TapScore();
		virtual ~Analyzer_TapScore();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};

#endif