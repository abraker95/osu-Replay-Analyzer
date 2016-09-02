#ifndef APPERANTDIFFICULTY_H
#define APPERANTDIFFICULTY_H

#include "../Analyzer.h"

class Analyzer_ApperantDifficulty : public Analyzer
{
	public:
		Analyzer_ApperantDifficulty();
		virtual ~Analyzer_ApperantDifficulty();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};

#endif // !APPERANTDIFFICULTY_H