#ifndef PATHPERPENDICULARITY_H
#define PATHPERPENDICULARITY_H

#include "../Analyzer.h"

class Analyzer_PathPerpendicularity : public Analyzer
{
	public:
		Analyzer_PathPerpendicularity();
		virtual ~Analyzer_PathPerpendicularity();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};

#endif // !PATHPERPENDICULARITY_H