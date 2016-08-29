#ifndef AIMSCORE_H
#define AIMSCORE_H

#include "../Analyzer.h"

class Analyzer_AimScore : public Analyzer
{
	public:
		Analyzer_AimScore();
		virtual ~Analyzer_AimScore();
		
		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};

#endif // !AIMSCORE_H