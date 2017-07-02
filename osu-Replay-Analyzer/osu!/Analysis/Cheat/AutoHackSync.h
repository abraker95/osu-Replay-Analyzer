#ifndef AUTOHACKSYNC_H
#define AUTOHACKSYNC_H

#include "../Analyzer.h"

class Analyzer_AutoHackSync : public Analyzer
{
	public:
		Analyzer_AutoHackSync();
		virtual ~Analyzer_AutoHackSync();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};


#endif // !AUTOHACKSYNC_H