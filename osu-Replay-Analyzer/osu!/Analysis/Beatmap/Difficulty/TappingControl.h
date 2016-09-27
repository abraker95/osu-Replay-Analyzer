#ifndef TAPPINGCONTROL_H
#define TAPPINGCONTROL_H

#include "../../Analysis.h"

class Analyzer_TappingControl : public Analyzer
{
	public:
		Analyzer_TappingControl();
		virtual ~Analyzer_TappingControl();

		virtual void AnalyzeStd(Play* _play);
		virtual void AnalyzeCatch(Play* _play);
		virtual void AnalyzeTaiko(Play* _play);
		virtual void AnalyzeMania(Play* _play);
		virtual void AnalyzeDodge(Play* _play);
};


#endif