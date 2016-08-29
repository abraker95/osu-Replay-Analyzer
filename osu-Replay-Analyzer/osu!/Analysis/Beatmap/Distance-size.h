#ifndef DISTSIZE_H
#define DISTSIZE_H

#include "../Analyzer.h"

class Analyzer_DistSize : public Analyzer
{
	public:
		Analyzer_DistSize();
		virtual ~Analyzer_DistSize();
		
		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};

#endif // 