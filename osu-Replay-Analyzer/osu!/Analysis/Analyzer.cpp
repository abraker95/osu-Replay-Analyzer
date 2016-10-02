#include "Analyzer.h"
#include <fstream>

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

bool Analyzer::isValid()
{
	// Todo: Check for wierd values that may crash the display graph and attempt to fix them
	// Validate();

	return data.size() > 0;
}

std::vector<osu::TIMING>* Analyzer::getData()
{
	return &data;
}

std::string Analyzer::getName() const
{
	return name;
}

void Analyzer::ExportData(Analyzer::FORMAT _format)
{
	if (!isValid()) return;

	switch (_format)
	{
		case XLM:
			//ExportXLM();
			break;

		case CVS:
			//ExportCVS();
			break;

		default:
			// UNSUPPORTED FORMAT!
			break;
	}
}

void Analyzer::Clear()
{
	data.clear();
	std::vector<osu::TIMING>().swap(data);
}

void Analyzer::ExportXLM()
{
	/// \TODO
}

void Analyzer::ExportCVS()
{
	std::fstream file(name + ".cvs");
	for (osu::TIMING timing : data)
		file << timing.data << "," << timing.time << std::endl;
	file.close();
}