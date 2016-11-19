#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <string>
#include <vector>

#include "Analyzer.h"
#include "../Filestructure/Play.h"

class Analysis
{
	public:
		Analysis();
		virtual ~Analysis();

		void AddAnalyzer(Analyzer* _analyzer);
		void StartAnalysis(Play* _play, std::string _analyzer = "", bool _print = true);
		
		Analyzer* getAnalyzer(std::string _analyzer);
		std::vector<std::string> getValidAnalyzers();
		void Clear();

	private:
		std::vector<Analyzer*> analyzers;		
};

#endif // !ANALYSIS_H