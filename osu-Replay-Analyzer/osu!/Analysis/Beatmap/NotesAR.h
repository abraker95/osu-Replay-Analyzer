#ifndef NOTESAR_H
#define NOTESAR_H

#include "../Analyzer.h"

class Analyzer_NotesAR : public Analyzer
{
	public:
		Analyzer_NotesAR();
		virtual ~Analyzer_NotesAR();
		
		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};

#endif // !NOTERATE_H