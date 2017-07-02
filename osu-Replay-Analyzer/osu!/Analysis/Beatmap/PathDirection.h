#ifndef PATHDIRECTION_H
#define PATHDIRECTION_H

#include "../Analyzer.h"

class Analyzer_PathDirection : public Analyzer
{
	public:
		Analyzer_PathDirection();
		virtual ~Analyzer_PathDirection();
		
		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};


#endif // !PATHDIRECTION_H