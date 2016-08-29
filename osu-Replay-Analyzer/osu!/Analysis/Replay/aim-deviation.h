#ifndef AIMDEVIATION_H
#define AIMDEVIATION_H

#include "../Analyzer.h"

class Analyzer_AimDeviation : public Analyzer
{
	public:
		Analyzer_AimDeviation();
		virtual ~Analyzer_AimDeviation();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};

#endif // !AIMDEVIATION_H