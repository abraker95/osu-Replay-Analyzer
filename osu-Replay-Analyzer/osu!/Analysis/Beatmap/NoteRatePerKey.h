#ifndef NOTERATEPERKEY_H
#define NOTERATEPERKEY_H

#include "../Analyzer.h"

class Analyzer_NoteRatePerKey : public Analyzer
{
	public:
		Analyzer_NoteRatePerKey();
		virtual ~Analyzer_NoteRatePerKey();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};

#endif // !NOTERATEPERKEY_H