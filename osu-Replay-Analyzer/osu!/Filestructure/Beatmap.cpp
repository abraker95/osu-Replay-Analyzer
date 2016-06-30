#include "Beatmap.h"
#include "../osuCalc.h"

#include <fstream>
#include <assert.h>
#include <memory>
#include "../../irrlicht/include/vector2d.h"

#include "filereader.h"

Beatmap::Beatmap(std::string file)
{
	std::ifstream beatmapFile(file);
	if (beatmapFile.is_open())
	{
		bool success = this->ParseBeatmap(beatmapFile);
		assert(success == true);
		beatmapFile.close();
	}
	else
	{
		/// \TODO: Handle this 
	}

	//SortEndTimes(0, hitObjectsTimeEnd.size());
	modifiedDiff = origDiff;
}

Beatmap::~Beatmap()
{
	/*for (auto hitobject : this->hitObjects)
	{
		if (hitobject != nullptr)
			delete hitobject;
	}*/
}

void Beatmap::Process()
{
	PrepareTimingPoints();

	/// \TODO: Refractor these and take account other gamemodes
	PrepareSliderData();
	GenerateSliderPoints();
}

GAMEMODE Beatmap::getGamemode()
{
	return gamemode;
}

Beatmap::Diff& Beatmap::getDiff()
{
	return modifiedDiff;
}

void Beatmap::setDiff(Diff _diff)
{
	modifiedDiff = _diff;
}

void Beatmap::resetDiff()
{
	modifiedDiff = origDiff;
}

Beatmap::Modifier& Beatmap::getModifiers() 
{
	return modifiedMod;
}

void Beatmap::setModifiers(Modifier _modifier)
{
	modifiedMod = _modifier;
}

void Beatmap::resetModifiers()
{
	modifiedMod = origMod;
}

int Beatmap::getNumHitobjectsVisibleAt(int _index, double _opacity)
{
	std::pair<int, int> indices = getIndicesVisibleAt(hitObjects[_index]->getTime(), _opacity);
	return indices.second - indices.first;
}

std::pair<int, int> Beatmap::getIndicesVisibleAt(int _time, double _opacity)
{
	int index = this->FindHitobjectAt(_time);
	std::pair<int, int> range;

	if (index >= 0)
	{
		// Find first note visible
		for (; index < hitObjects.size(); index++)
		{
			if (hitObjects[index]->isVisibleAt(_time, modifiedDiff.ar, modifiedMod.hidden))
				break;
		}
		range.first = index;

		// Find last note visible
		for (; index < hitObjects.size(); index++)
		{
			if (!(hitObjects[index]->isVisibleAt(_time, modifiedDiff.ar, modifiedMod.hidden)))
				break;
		}
		range.second = index;
	}

	return range;
}

std::string Beatmap::getName()
{
	return metadata.artist + " - " + metadata.title + " (" + metadata.creator + ") [" + metadata.version + "]";
}

TimingPoint* Beatmap::getTimingPointAt(int _time)
{
	int start = 0;
	int end = this->timingPoints.size() - 2;
	int mid;

	while (start <= end)
	{
		mid = (start + end) / 2;
		if (BTWN(this->timingPoints[mid].offset, _time, this->timingPoints[mid + 1].offset - 1))
			return &(this->timingPoints[mid]);
		else if (_time < this->timingPoints[mid].offset)
			end = mid - 1;
		else start = mid + 1;
	}

	/// \TODO: Test the fuck out of this. I think it's bugged 
	return &this->timingPoints[timingPoints.size() - 1];
}

Hitobject* Beatmap::getHitobjectAt(int _time)
{
	int index = this->FindHitobjectAt(_time);
	return this->hitObjects[index];
}

//---------------- [PRIVATE] ----------------------



// ------------ [ReadFile/Parse] -------------

bool Beatmap::ParseBeatmap(std::ifstream &_filepath)
{
	ParseBeatmapFormat(_filepath);
	return ParseBeatmapData(_filepath);
}

void Beatmap::ParseBeatmapFormat(std::ifstream &_filepath)
{
	std::string line = "";
	getline(_filepath, line);
	line = getStrAfter(line, "osu file format v").data();

	if (line != "")
		metadata.format = atoi(line.data());
	else
		metadata.format = -1;
}

bool Beatmap::ParseBeatmapData(std::ifstream &_filepath)
{
	if (metadata.format != -1)
	{
		SECTION section = SECTION_NONE;
		std::string line = "";

		while (getline(_filepath, line))
		{
				 if (line == "[General]")		section = SECTION_GENERAL;
			else if (line == "[Editor]")		section = SECTION_EDITOR;
			else if (line == "[Metadata]")		section = SECTION_METADATA;
			else if (line == "[Difficulty]")	section = SECTION_DIFFICULTY;
			else if (line == "[Events]")		section = SECTION_EVENTS;
			else if (line == "[TimingPoints]")	section = SECTION_TIMINGPOINTS;
			else if (line == "[HitObjects]")	section = SECTION_HITOBJECTS;
			else ParseSection(_filepath, section, line);
		}

		return 1;
	}
	else
	{
		return 0;
	}
}


void Beatmap::ParseSection(std::ifstream &_filepath, SECTION _section, std::string &_line)
{
	switch (_section)
	{
		case SECTION_GENERAL:
			ParseGeneralSection(_filepath, _line);
			break;

		case SECTION_EDITOR:
			ParseEditorSection(_filepath, _line);
			break;

		case SECTION_METADATA:
			ParseMetadataSection(_filepath, _line);
			break;

		case SECTION_DIFFICULTY:
			ParseDifficultysection(_filepath, _line);
			break;

		case SECTION_EVENTS:
			ParseEventsSection(_filepath, _line);
			break;

		case SECTION_TIMINGPOINTS:
			ParseTimingPointsSection(_filepath, _line);
			break;

		case SECTION_HITOBJECTS:
			ParseHitobjectsSection(_filepath, _line);
			break;

		default:
			break;
	};
}


bool Beatmap::ParseGeneralSection(std::ifstream &_filepath, std::string &_line)
{
	std::string data = "";

	do
	{
		if (_line == "")
			break;

		data = getStrAfter(_line, "AudioFilename: ");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "AudioHash: ");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "AudioLeadIn: ");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "PreviewTime: ");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "Countdown: ");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "SampleSet: ");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "StackLeniency: ");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "Mode: ");
		if (data != "")
		{
			gamemode = (GAMEMODE)atoi(data.data());
			continue;
		}

		data = getStrAfter(_line, "LetterboxInBreaks: ");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "SpecialStyle: ");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "WidescreenStoryboard: ");
		if (data != "")
		{
			/* ignore */
			continue;
		}
	} while (getline(_filepath, _line));

	return 1;
}

bool Beatmap::ParseEditorSection(std::ifstream &_filepath, std::string &_line)
{
	std::string data = "";
	
	do
	{
		if (_line == "")
			break;

		data = getStrAfter(_line, "DistanceSpacing: ");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "BeatDivisor: ");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "GridSize: ");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "TimelineZoom: ");
		if (data != "")
		{
			/* ignore */
			continue;
		}
	} while (getline(_filepath, _line));

	return 1;
}

bool Beatmap::ParseMetadataSection(std::ifstream &_filepath, std::string &_line)
{
	std::string data = "";
	
	do
	{
		if (_line == "")
			break;

		data = getStrAfter(_line, "Title:");
		if (data != "")
		{
			metadata.title = data;
			continue;
		}

		data = getStrAfter(_line, "TitleUnicode:");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "Artist:");
		if (data != "")
		{
			metadata.artist = data;
			continue;
		}

		data = getStrAfter(_line, "ArtistUnicode:");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "Creator:");
		if (data != "")
		{
			metadata.creator = data;
			continue;
		}

		data = getStrAfter(_line, "Version:");
		if (data != "")
		{
			metadata.version = data;
			continue;
		}

		data = getStrAfter(_line, "Source:");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "Tags:");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "BeatmapID:");
		if (data != "")
		{
			/* ignore */
			continue;
		}

		data = getStrAfter(_line, "BeatmapSetID:");
		if (data != "")
		{
			/* ignore */
			continue;
		}
	} while (getline(_filepath, _line));

	return 1;
}

bool Beatmap::ParseDifficultysection(std::ifstream &_filepath, std::string &_line)
{
	std::string data = "";
	
	do
	{
		if (_line == "")
			break;

		data = getStrAfter(_line, "HPDrainRate:");
		if (data != "")
		{
			origDiff.hp = atof(data.data());
			continue;
		}

		data = getStrAfter(_line, "CircleSize:");
		if (data != "")
		{
			origDiff.cs = atof(data.data());
			continue;
		}

		data = getStrAfter(_line, "OverallDifficulty:");
		if (data != "")
		{
			origDiff.od = atof(data.data());
			continue;
		}

		data = getStrAfter(_line, "ApproachRate:");
		if (data != "")
		{
			origDiff.ar = atof(data.data());
			continue;
		}

		data = getStrAfter(_line, "SliderMultiplier:");
		if (data != "")
		{
			origDiff.sm = atof(data.data());
			continue;
		}

		data = getStrAfter(_line, "SliderTickRate:");
		if (data != "")
		{
			origDiff.st = atof(data.data());
			continue;
		}
	} while (getline(_filepath, _line));

	return 1;
}

bool Beatmap::ParseEventsSection(std::ifstream &_filepath, std::string &_line)
{	
	do
	{
		if (_line == "")
			break;

		/* ignore */

	} while (getline(_filepath, _line));
	
	return 1;
}

bool Beatmap::ParseTimingPointsSection(std::ifstream &_filepath, std::string &_line)
{
	do
	{
		if (_line == "")
			break;

		int status = this->ReadTimingpoints(_line);
		if (status == -1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	} while (getline(_filepath, _line));
}

bool Beatmap::ParseHitobjectsSection(std::ifstream &_filepath, std::string &_line)
{
	do
	{
		if (_line == "")
			break;
	
		int status = this->ReadHitobjects(_line);
		if (status == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	} while (getline(_filepath, _line));
}


int Beatmap::ReadTimingpoints(std::string line)
{
	TimingPoint tPoint;
	std::vector<std::string> tokens;
	FileReader::tokenize(line, tokens, ",");

	if (tokens.size() < 2)
		return 0;

	tPoint.offset = atoi(tokens[0].c_str());
	tPoint.beatInterval = atof(tokens[1].c_str());

	if (tokens.size() > 6)		tPoint.inherited = !atoi(tokens[6].c_str());
	else						tPoint.inherited = 0;

	// Old maps don't have metres
	if (tokens.size() >= 3)		tPoint.meter = atoi(tokens[2].c_str());
	else						tPoint.meter = 4;

	if (tPoint.offset == -1 || tPoint.meter == -1)
	{
		// Wrong timing point data!
		return -1;
	}

	this->timingPoints.push_back(tPoint);
	return 1;
}

int Beatmap::ReadHitobjects(std::string line)
{
	std::vector<std::string> objectData, sliderData;
	FileReader::tokenize(line, objectData, ",");

	if(atoi(objectData[HITOBJECTNORMAL::TYPE].data()) & HITOBJECTYPE::SLIDER)
		FileReader::tokenize(objectData[5], sliderData, "|");

	Hitobject* hitObject = new Hitobject(objectData, sliderData);

	/// \TODO: color hacks types
	if (hitObject->getHitobjectType() & HITOBJECTYPE::CIRCLE)
	{
		this->hitObjects.push_back(hitObject);
		this->hitObjectsTimeStart.push_back(std::pair<Hitobject*, int>(hitObject, hitObject->getTime()));
		this->hitObjectsTimeEnd.push_back(std::pair<Hitobject*, int>(hitObject, hitObject->getTime()));
		return 1;
	}

	if (hitObject->getHitobjectType() & HITOBJECTYPE::SLIDER)
	{
		this->hitObjects.push_back(hitObject);
		this->hitObjectsTimeStart.push_back(std::pair<Hitobject*, int>(hitObject, hitObject->getTime()));
		this->hitObjectsTimeEnd.push_back(std::pair<Hitobject*, int>(hitObject, hitObject->slider->getEndTime()));
		return 1;
	}

	if (hitObject->getHitobjectType() & HITOBJECTYPE::SPINNER)
	{
		this->spinners++;
		return 0;
	}

	if (hitObject->getHitobjectType() & HITOBJECTYPE::MANIALONG)
	{
		this->hitObjects.push_back(hitObject);
		this->hitObjectsTimeStart.push_back(std::pair<Hitobject*, int>(hitObject, hitObject->getTime()));
		this->hitObjectsTimeEnd.push_back(std::pair<Hitobject*, int>(hitObject, hitObject->slider->getEndTime()));
		return 0;
	}

	return 0; // what is this object?
}

// ------------ [Object Generation] -------------

void Beatmap::PrepareTimingPoints()
{
	// Get min and maxbpm
	this->bpmMin = DBL_MAX;
	this->bpmMax = DBL_MIN;

	double BPM = 0;
	double SliderMult = -100;
	double oldbeat = -100;
	int base = 0;

	for (auto& TP : this->timingPoints)
	{
		if (TP.inherited)
		{
			TP.beatLength = base;

			if (TP.beatInterval <= 0)
			{
				SliderMult = TP.beatInterval;
				oldbeat = TP.beatInterval;
			}
			else
				SliderMult = oldbeat;
		}
		else
		{
			SliderMult = -100;
			BPM = TP.getBPM(true);
			TP.beatLength = TP.beatInterval;
			base = TP.beatInterval;

			if (this->bpmMin > BPM)	this->bpmMin = BPM;
			if (this->bpmMax < BPM)	this->bpmMax = BPM;
		}

		TP.BPM = BPM;
		TP.sm = SliderMult;
	}
}

void Beatmap::PrepareSliderData()
{
	for (auto& hitObject : this->hitObjects)
	{
		if (hitObject->IsHitObjectType(HITOBJECTYPE::SLIDER))
		{
			switch (hitObject->slider->curveType)
			{
				case 'B':
				{
					assert(hitObject->slider != nullptr);
					hitObject->slider->newSlider(hitObject, false, false);
					break;
				}

				case 'P':
				{
					if (hitObject->slider->curve.size() == 2)
					{
						assert(hitObject->slider != nullptr);
						hitObject->slider->newSlider(hitObject, false, true);
					}
					else
					{
						assert(hitObject->slider != nullptr);
						hitObject->slider->newSlider(hitObject, false, false);
					}

					break;
				}

				case 'L': case 'C':
				{
					assert(hitObject->slider != nullptr);
					hitObject->slider->newSlider(hitObject, true, false);
					break;
				}
			}
			hitObject->slider->endPoint = (hitObject->slider->repeat % 2) ? hitObject->slider->curve.back() : hitObject->slider->curve.front();
		}
	}
}

void Beatmap::GenerateSliderPoints()
{
	for (auto& hitObject : this->hitObjects)
	{
		if (hitObject->IsHitObjectType(HITOBJECTYPE::SLIDER))
		{
			TimingPoint* tp = getTimingPointAt(hitObject->getTime());
			double BPM = tp->getBPM();

			hitObject->slider->toRepeatTime = round((double)(((-600.0 / BPM) * hitObject->slider->pixelLength * tp->sm) / (100.0 * modifiedDiff.sm)));
			hitObject->slider->endTime = hitObject->slider->getEndTime();
			hitObject->slider->RecordRepeatTimes();

			double tickInterval = tp->beatLength / modifiedDiff.st;
			hitObject->slider->RecordTickIntervals(tickInterval);
		}
	}
}



// ------------ [Internal] -------------

int Beatmap::FindHitobjectAt(int _time)
{
	int start = 0;
	int end = this->hitObjects.size() - 2;
	int mid;

	while (start <= end)
	{
		mid = (start + end) / 2;

		// if time is exactly at the object, return that object
		if (this->hitObjects[mid]->getTime() == _time)
		{
			// make sure there is are no following objects at the same time
			for (int i = mid; i < this->hitObjects.size(); i++)
				if (this->hitObjects[mid]->getTime() == _time)
					mid = i;

			return mid;
		}
		// if the time is between 2 objects, return the next object
		if(BTWN(this->hitObjects[mid]->getTime(), _time, this->hitObjects[mid + 1]->getTime()))
		{
			// make sure there is are no following objects at the same time
			for (int i = mid + 1; i < this->hitObjects.size(); i++)
				if (BTWN(this->hitObjects[mid]->getTime(), _time, this->hitObjects[mid + 1]->getTime()))
					mid = i;

			return mid;
		}

	/*	// check if the object ends somewhere else
		int objectType = hitObjects[mid]->getHitobjectType();
		if (objectType & (HITOBJECTYPE::SLIDER | HITOBJECTYPE::MANIALONG))
		{
			if (BTWN(this->hitObjects[mid]->getTime(), _time, this->hitObjects[mid]->slider->getEndTime()))
			{
				return mid;
			}
		}*/
		
		int gettime = this->hitObjects[mid]->getTime();
		if (_time < this->hitObjects[mid]->getTime())
			end = mid - 1;
		else start = mid + 1;
	}

	return 0;
}

/*
int Beatmap::FindHitobjectAt(int _time, bool _begEnd)
{
	int start = 0;
	int end = this->hitObjects.size() - 2;
	int mid;

	while (start <= end)
	{
		mid = (start + end) / 2;

		// if time is exactly at the object, return that object
		if (this->hitObjects[mid]->getTime() == _time)
		{
			// make sure there is are no following objects at the same time
			for (int i = mid; i < this->hitObjects.size(); i++)
				if (this->hitObjects[mid]->getTime() == _time)
					mid = i;

			return mid;
		}

		// if the time is between 2 objects, return the next object
		if (BTWN(this->hitObjects[mid]->getTime(), _time, this->hitObjects[mid + 1]->getTime()))
		{
			// make sure there is are no following objects at the same time
			for (int i = mid + 1; i < this->hitObjects.size(); i++)
				if (BTWN(this->hitObjects[mid]->getTime(), _time, this->hitObjects[mid + 1]->getTime()))
					mid = i;

			return mid;
		}

		// check if the object ends somewhere else
		int objectType = hitObjects[mid]->getHitobjectType();
		if (objectType & (HITOBJECTYPE::SLIDER | HITOBJECTYPE::MANIALONG))
		{
			if (BTWN(this->hitObjects[mid]->getTime(), _time, this->hitObjects[mid]->slider->getEndTime()))
			{
				return mid;
			}
		}

		int gettime = this->hitObjects[mid]->getTime();
		if (_time < this->hitObjects[mid]->getTime())
			end = mid - 1;
		else start = mid + 1;
	}

	return 0;
}*/

/*
void Beatmap::SortEndTimes(int _left, int _right)
{
	int i = _left, int j = _right;
	int pivot = hitObjectsTimeEnd[(_left + _right) / 2].second;

	// partition 
	while (i <= j)
	{
		while (hitObjectsTimeEnd[i].second < pivot) i++;
		while (hitObjectsTimeEnd[i].second > pivot) j--;

		if (i <= j)
		{
			swap(hitObjectsTimeEnd[i], hitObjectsTimeEnd[j]);
			j--;   i++;
		}
	}

	// recursion
	if (_left < j) SortEndTimes(_left, j);
	if (_left < j) SortEndTimes(i, _right);
}*/