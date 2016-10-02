#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>
#include <vector>

#include "../osuStructs.h"
#include "../Filestructure/Play.h"

class Analyzer
{
	public:
		enum FORMAT
		{
			CVS = 1,
			XLM = 2
		};

		Analyzer(std::string _name);
		virtual ~Analyzer();

		void Analyze(Play* _play);
		bool isValid();

		std::vector<osu::TIMING>* getData();
		std::string getName() const;

		void Clear();
		void ExportData(FORMAT _format);

	protected:
		std::string name;
		std::vector<osu::TIMING> data;

		virtual void AnalyzeStd(Play* _play) = 0;
		virtual void AnalyzeCatch(Play* _play) = 0;
		virtual void AnalyzeTaiko(Play* _play) = 0;
		virtual void AnalyzeMania(Play* _play) = 0;
		virtual void AnalyzeDodge(Play* _play) = 0;

		void ExportXLM();
		void ExportCVS();
};

#endif // !ANALYZER_H