#ifndef NOTERATE_H
#define NOTERATE_H

#include "../Analyzer.h"

class Analyzer_NoteRate : public Analyzer
{
	public:
		Analyzer_NoteRate();
		virtual ~Analyzer_NoteRate();
		
		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};

#endif // !NOTERATE_H