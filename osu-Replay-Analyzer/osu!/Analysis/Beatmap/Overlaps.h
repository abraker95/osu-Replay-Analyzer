#ifndef OVERLAPS_H
#define OVERLAPS_H

#include "../Analyzer.h"

class Analyzer_Overlaps : public Analyzer
{
	public:
		Analyzer_Overlaps();
		virtual ~Analyzer_Overlaps();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};

#endif // !OVERLAPS_H