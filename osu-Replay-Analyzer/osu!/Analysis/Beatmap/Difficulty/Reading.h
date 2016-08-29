#ifndef READING_H
#define READING_H

#include "../../Analyzer.h"

class Analyzer_Reading : public Analyzer
{
public:
	Analyzer_Reading();
	virtual ~Analyzer_Reading();

	virtual void AnalyzeStd(Play* _play);
	virtual void AnalyzeCatch(Play* _play);
	virtual void AnalyzeTaiko(Play* _play);
	virtual void AnalyzeMania(Play* _play);
	virtual void AnalyzeDodge(Play* _play);
};

#endif // READING_H