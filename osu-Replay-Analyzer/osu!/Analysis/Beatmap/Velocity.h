#ifndef VELOCITY_H
#define VELOCITY_H

#include "../Analyzer.h"

class Analyzer_Velocity : public Analyzer
{
	public:
		Analyzer_Velocity();
		virtual ~Analyzer_Velocity();
		
		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};

#endif // !VELOCITY_H