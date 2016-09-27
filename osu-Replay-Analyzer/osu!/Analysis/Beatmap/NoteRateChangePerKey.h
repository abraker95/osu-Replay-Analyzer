#ifndef NOTERATECHANGEPERKEY_H
#define NOTERATECHANGEPERKEY_H

#include "../Analyzer.h"

class Analyzer_NoteRateChangePerKey : public Analyzer
{
	public:
		Analyzer_NoteRateChangePerKey();
		virtual ~Analyzer_NoteRateChangePerKey();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};


#endif // !NOTERATECHANGEPERKEY_H