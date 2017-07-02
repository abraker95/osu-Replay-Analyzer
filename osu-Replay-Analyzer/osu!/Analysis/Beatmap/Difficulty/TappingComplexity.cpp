#include "TappingComplexity.h"
#include "../../AnalysisStruct.h"

#include "../../../osu_standard.h"
#include "../../../osu_mania.h"

Analyzer_TappingComplexity::Analyzer_TappingComplexity() : Analyzer("Tapping Complexity") {}
Analyzer_TappingComplexity::~Analyzer_TappingComplexity() {}

void Analyzer_TappingComplexity::AnalyzeStd(Play* _play)
{
	Analyzer* analyzer = AnalysisStruct::beatmapAnalysis.getAnalyzer("tap event intervals (ms)");
	std::vector<osu::TIMING>& tapIntervals = *(analyzer->getData());
	if (tapIntervals.size() == 0) return;

	osu::TIMING timing;
		timing.data = 0;
		timing.key = 0;

	data = tapIntervals;
	std::vector<long> TapIntervalDaltas;

	for (osu::TIMING& data : data)
		data.data = 1.0 / data.data;		

	for (int i = 1; i < data.size(); i++)
	{
		long count = 1;
		if (data[i].data != data[i - 1].data)
		{
			for (int j = 0; j < count; j++)
				data[i - j].data /= (double)count;

			TapIntervalDaltas.push_back(count);
			count = 1;
		}
	}

	int origPos = TapIntervalDaltas[0];
	for (int i = 1; i < TapIntervalDaltas.size(); i++)
	{
		origPos += TapIntervalDaltas[i];
		long count = 1;

		if ((TapIntervalDaltas[i] != TapIntervalDaltas[i - 1]) && (TapIntervalDaltas[i] != 1))
		{
			for (int j = 0; j < count * TapIntervalDaltas[i]; j++)
				data[origPos - j].data /= (double)count;

			count = 1;
		}
	}

	Normalize(data, false);
	data[data.size() - 1].key = osu::GetMax(data);
}

void Analyzer_TappingComplexity::AnalyzeCatch(Play* _play) {}

void Analyzer_TappingComplexity::AnalyzeTaiko(Play* _play) {}

void Analyzer_TappingComplexity::AnalyzeMania(Play* _play)
{
	
}

void Analyzer_TappingComplexity::AnalyzeDodge(Play* _play)
{

}