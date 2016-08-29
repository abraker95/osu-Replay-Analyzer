#ifndef NOTERATEPEYPRECISE_H
#define NOTERATEPEYPRECISE_H

#include "../Analyzer.h"

class Analyzer_NoteRatePerKeyPrec : public Analyzer
{
	public:
		Analyzer_NoteRatePerKeyPrec();
		virtual ~Analyzer_NoteRatePerKeyPrec();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};


#endif // !NOTERATEPEYPRECISE_H