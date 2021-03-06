#include "Analysis.h"
#include <iostream>

Analysis::Analysis()
{

}

Analysis::~Analysis()
{
	for (Analyzer* analyzer : analyzers)
		delete analyzer;
}


void Analysis::AddAnalyzer(Analyzer* _analyzer)
{
	analyzers.push_back(_analyzer);
}


void Analysis::StartAnalysis(Play* _play, std::string _analyzer)
{
	if (_analyzer == "")
	{
		for (Analyzer* analyzer : analyzers)
		{
			std::cout << "\t Starting " << analyzer->getName() << " analysis..." << std::endl;

			analyzer->Clear();
			analyzer->Analyze(_play);
		}
	}
	else
	{
		Analyzer* analyzer = getAnalyzer(_analyzer);
		if (analyzer == nullptr) return;

		std::cout << "\t Starting " << analyzer->getName() << " analysis..." << std::endl;

		analyzer->Clear();
		analyzer->Analyze(_play);
	}
}


Analyzer* Analysis::getAnalyzer(std::string _analyzer)
{
	/// \TODO: Binary search?
	for (Analyzer* analyzer : analyzers)
	{
		if (analyzer->getName() == _analyzer)
			return analyzer;
	}

	std::cout << "[Warning] Analyzer \"" << _analyzer << "\"not found!" << std::endl;
	return nullptr;
}

std::vector<std::string> Analysis::getValidAnalyzers()
{
	std::vector<std::string> list;
	for (Analyzer* analyzer : analyzers)
	{
		if (analyzer->getData()->size() != 0)
			list.push_back(analyzer->getName());
	}

	return list;
}

void Analysis::Clear()
{
	for (Analyzer* analyzer : analyzers)
		analyzer->Clear();
}