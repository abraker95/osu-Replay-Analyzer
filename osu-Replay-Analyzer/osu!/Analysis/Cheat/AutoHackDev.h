#ifndef AUTOHACK_H
#define AUTOHACK_H

#include "../Analyzer.h"

class Analyzer_AutoHackDev : public Analyzer
{
	public:
		Analyzer_AutoHackDev();
		virtual ~Analyzer_AutoHackDev();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};


#endif // !AUTOHACK_H