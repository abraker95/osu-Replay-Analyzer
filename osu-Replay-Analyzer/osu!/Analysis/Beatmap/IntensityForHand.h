#ifndef INTESITYFORHAND_H
#define INTESITYFORHAND_H

#include "../Analyzer.h"

class Analyzer_IntensityForHand : public Analyzer
{
	public:
		Analyzer_IntensityForHand();
		virtual ~Analyzer_IntensityForHand();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};

#endif // !INTESITYFORHAND_H