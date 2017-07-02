#ifndef AGILITYSIM_H
#define AGILITYSIM_H

#include "../Analyzer.h"

class Analyzer_AgilitySim : public Analyzer
{
	public:
		Analyzer_AgilitySim();
		virtual ~Analyzer_AgilitySim();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};

#endif // !AGILITYSIM_H