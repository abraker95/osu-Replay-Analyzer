#ifndef READINGRATE_H
#define READINGREATE_H

#include "../Analyzer.h"

class Analyzer_ReadingIntensity : public Analyzer
{
	public:
		Analyzer_ReadingIntensity();
		virtual ~Analyzer_ReadingIntensity();
		
		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};


#endif // !READINGRATE_H