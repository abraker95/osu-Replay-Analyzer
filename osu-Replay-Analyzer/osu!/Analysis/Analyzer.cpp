#include "Analyzer.h"

Analyzer::Analyzer(std::string _name)
{
	name = _name;
}

Analyzer::~Analyzer()
{
	Clear();
}

void Analyzer::Analyze(Play* _play)
{
	if (_play->beatmap == nullptr) return;
	switch (_play->beatmap->getGamemode())
	{
		case GAMEMODE::OSU_STANDARD:
			AnalyzeStd(_play);
			break;

		case GAMEMODE::OSU_CATCH:
			AnalyzeCatch(_play);
			break;

		case GAMEMODE::OSU_TAIKO:
			AnalyzeTaiko(_play);
			break;

		case GAMEMODE::OSU_MANIA:
			AnalyzeMania(_play);
			break;

		case GAMEMODE::OSU_DODGE:
			AnalyzeDodge(_play);
			break;

		default:
			break;
	}
}

std::vector<osu::TIMING>* Analyzer::getData()
{
	return &data;
}

std::string Analyzer::getName() const
{
	return name;
}

void Analyzer::Clear()
{
	data.clear();
	std::vector<osu::TIMING>().swap(data);
}