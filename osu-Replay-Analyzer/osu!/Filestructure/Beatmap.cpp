#include "Beatmap.h"
#include "../osuCalc.h"

#include <fstream>
#include <assert.h>
#include <memory>
#include "../../irrlicht/include/vector2d.h"
#include "SliderHitObject.h"

#include "filereader.h"

Beatmap::Beatmap() { gamemode = GAMEMODE_ERROR; }

Beatmap::~Beatmap()
{
	ClearModified();
	ClearObjects();
}

void Beatmap::Read(std::string _beatmapfile)
{
	std::ifstream beatmapFile(_beatmapfile);
	if (beatmapFile.is_open())
	{
		ClearObjects();

		bool success = this->ParseBeatmap(beatmapFile);

		if (gamemode == GAMEMODE_ERROR) gamemode = GAMEMODE::OSU_STANDARD; // old maps didn't specify gamemodes
		if (!success)	gamemode = GAMEMODE_ERROR;						   // make sure everything went ok while parsing

		beatmapFile.close();
	}
	else
	{
		gamemode = GAMEMODE_ERROR;
	}

	// for old maps which had AR as part as OD
	if (this->mods.getAR() == -1)
		this->mods.setAR(mods.getOD());
	this->mods.setTM(1.0);

	//SortEndTimes(0, hitObjectsTimeEnd.size());
	//modifiedDiff = origDiff;
}

void Beatmap::Process()
{
	// Make sure the beat maps is loaded
	if (!isValid()) return;

	PrepareTimingPoints();

	/// \TODO: Is there a way to put these into the hitobject class?
	PrepareSliderData();
	GenerateSliderPoints();
}

bool Beatmap::isValid()
{
	return (getGamemode() != GAMEMODE::GAMEMODE_ERROR);
}

GAMEMODE Beatmap::getGamemode()
{
	return gamemode;
}

Beatmap::Diff& Beatmap::getDiff()
{
	return diff;
}

void Beatmap::setDiff(Diff _diff)
{
	diff = _diff;
}

Mods Beatmap::getMods()
{
	return mods;
}

void Beatmap::ClearModified()
{
	if (modHitobjects.size() == 0) return;
	for (Hitobject* hitobject : modHitobjects)
		delete hitobject;

	modHitobjects.clear();
	std::vector<Hitobject*>().swap(modHitobjects);

	modTimingPoints.clear();
	std::vector<TimingPoint>().swap(modTimingPoints);
}

void Beatmap::ResetModified()
{
	// reset diffs, timingpoints, and modified hitobjects
	ClearModified();
	modTimingPoints = origTimingPoints;

	for (Hitobject* hitobject : origHitobjects)
	{
		if (hitobject->isHitobjectLong())
			modHitobjects.push_back(new SliderHitObject(*hitobject->getSlider()));
		else
			modHitobjects.push_back(new Hitobject(*hitobject));
	}
}

std::vector<Hitobject*>& Beatmap::getHitobjects()
{
	return modHitobjects;
}

int Beatmap::getNumHitobjectsVisibleAt(int _index, double _opacity)
{
	std::pair<int, int> indices = getIndicesVisibleAt(modHitobjects[_index]->getTime(), _opacity);
	return indices.second - indices.first;
}

std::pair<int, int> Beatmap::getIndicesVisibleAt(int _time, double _opacity)
{
	int index = this->FindHitobjectAt(_time) - 1;
	std::pair<int, int> range;

	// Find first note visible
	for (index = max(index, 0); index < modHitobjects.size(); index++)
	{
		if (modHitobjects[index]->isVisibleAt(_time, mods.getAR(), mods.getModifier().HD))
			break;
	}
	range.first = index;

	// Find last note visible
	for (; index < modHitobjects.size(); index++)
	{
		if (!(modHitobjects[index]->isVisibleAt(_time, mods.getAR(), mods.getModifier().HD)))
			break;
	}
	range.second = index;

	return range;
}

std::string Beatmap::getName()
{
	return metadata.artist + " - " + metadata.title + " (" + metadata.creator + ") [" + metadata.version + "]";
}

TimingPoint* Beatmap::getTimingPointAt(int _time)
{
	int start = 0;
	int end = this->modTimingPoints.size() - 2;
	int mid;

	while (start <= end)
	{
		mid = (start + end) / 2;
		if (BTWN(this->modTimingPoints[mid].offset, _time, this->modTimingPoints[mid + 1].offset - 1))
			return &(this->modTimingPoints[mid]);
		else if (_time < this->modTimingPoints[mid].offset)
			end = mid - 1;
		else start = mid + 1;
	}

	/// \TODO: Test the fuck out of this. I think it's bugged 
	return &this->modTimingPoints[modTimingPoints.size() - 1];
}

Hitobject* Beatmap::getHitobjectAt(int _time)
{
	int index = this->FindHitobjectAt(_time);
	return this->modHitobjects[index];
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
			else if (line == "[Colours]")		section = SECTION_COLOURS;
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

		case SECTION_COLOURS:
			ParseColourSection(_filepath, _line);
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
			diff.hp = atof(data.data());
			continue;
		}

		data = getStrAfter(_line, "CircleSize:");
		if (data != "")
		{
			mods.setCS(atof(data.data()));
			continue;
		}

		data = getStrAfter(_line, "OverallDifficulty:");
		if (data != "")
		{
			mods.setOD(atof(data.data()));
			continue;
		}

		data = getStrAfter(_line, "ApproachRate:");
		if (data != "")
		{
			mods.setAR(atof(data.data()));
			continue;
		}

		data = getStrAfter(_line, "SliderMultiplier:");
		if (data != "")
		{
			diff.sm = atof(data.data());
			continue;
		}

		data = getStrAfter(_line, "SliderTickRate:");
		if (data != "")
		{
			diff.st = atof(data.data());
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

bool Beatmap::ParseColourSection(std::ifstream &_filepath, std::string &_line)
{
	do
	{
		if (_line == "")
			break;

		/* ignore */

	} while (getline(_filepath, _line));

	return 1;
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

	// Make sure we got the correct data
	if (tokens.size() < 2) return 0;

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

	this->origTimingPoints.push_back(tPoint);
	return 1;
}

int Beatmap::ReadHitobjects(std::string line)
{
	std::vector<std::string> objectData, sliderData;
	FileReader::tokenize(line, objectData, ",");
	int type = atoi(objectData[HITOBJECTNORMAL::TYPE].data());

	/// \TODO: color hacks types
	if (type & HITOBJECTYPE::CIRCLE)
	{
		this->origHitobjects.push_back(new Hitobject(objectData));

		//this->hitObjectsTimeStart.push_back(std::pair<Hitobject*, int>(hitObject, hitObject->getTime()));
		//this->hitObjectsTimeEnd.push_back(std::pair<Hitobject*, int>(hitObject, hitObject->getTime()));
		return 1;
	}

	if (type & HITOBJECTYPE::SLIDER)
	{
		FileReader::tokenize(objectData[5], sliderData, "|");
		this->origHitobjects.push_back(new SliderHitObject(objectData, sliderData));

		//this->hitObjectsTimeStart.push_back(std::pair<Hitobject*, int>(hitObject, hitObject->getTime()));
		//this->hitObjectsTimeEnd.push_back(std::pair<Hitobject*, int>(hitObject, hitObject->slider->getEndTime()));
		return 1;
	}

	if (type & HITOBJECTYPE::SPINNER)
	{
		this->spinners++;
		return 0;
	}

	if (type & HITOBJECTYPE::MANIALONG)
	{
		FileReader::tokenize(objectData[5], sliderData, "|");
		this->origHitobjects.push_back(new SliderHitObject(objectData, sliderData));

		//this->hitObjectsTimeStart.push_back(std::pair<Hitobject*, int>(hitObject, hitObject->getTime()));
		//this->hitObjectsTimeEnd.push_back(std::pair<Hitobject*, int>(hitObject, hitObject->slider->getEndTime()));
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

	for (auto& TP : this->modTimingPoints)
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
	for (auto& hitObject : this->modHitobjects)
	{
		// Make sure this is a slider
		if (!hitObject->isHitobjectLong()) continue;

		// nothing to do here ._.
		if (hitObject->getHitobjectType() & HITOBJECTYPE::MANIALONG) break;

		// Generate the slider types
		SliderHitObject* slider = hitObject->getSlider();
		switch (slider->curveType)
		{
			case 'B':
				hitObject->getSlider()->newSlider(false, false);
				break;

			case 'P':
				if (hitObject->getSlider()->curves.size() == 2)
					hitObject->getSlider()->newSlider(false, true);
				else
					hitObject->getSlider()->newSlider(false, false);

				break;

			case 'L': case 'C':
				hitObject->getSlider()->newSlider(true, false);
				break;
		}

		slider->endPoint = (slider->repeat % 2) ? slider->curves.back() : slider->curves.front();
	}
}

void Beatmap::GenerateSliderPoints()
{
	for (auto& hitObject : this->modHitobjects)
	{
		// Make sure this is a slider
		if (!hitObject->IsHitObjectType(HITOBJECTYPE::SLIDER))
			continue;

		// Generate timepoint dependent slider info
		SliderHitObject *slider = hitObject->getSlider();
		TimingPoint* tp = getTimingPointAt(slider->getTime());
		double BPM = tp->getBPM();

		slider->toRepeatTime = round((double)(((-600.0 / BPM) * slider->pixelLength * tp->sm) / (100.0 * diff.sm)));
		slider->endTime = slider->getTime() + slider->toRepeatTime * slider->repeat;
		slider->RecordRepeatTimes();

		double tickInterval = tp->beatLength / diff.st;
		slider->RecordTickIntervals(tickInterval);
	}
}



// ------------ [Internal] -------------

int Beatmap::FindHitobjectAt(int _time)
{
	int start = 0;
	int end = this->origHitobjects.size() - 2;
	int mid;

	while (start <= end)
	{
		mid = (start + end) / 2;

		// if time is exactly at the object, return that object
		if (this->origHitobjects[mid]->getTime() == _time)
		{
			// make sure there is are no following objects at the same time
			for (int i = mid; i < this->origHitobjects.size(); i++)
				if (this->origHitobjects[mid]->getTime() == _time)
					mid = i;

			return mid;
		}
		// if the time is between 2 objects, return the next object
		if(BTWN(this->origHitobjects[mid]->getTime(), _time, this->origHitobjects[mid + 1]->getTime()))
		{
			// make sure there is are no following objects at the same time
			for (int i = mid + 1; i < this->origHitobjects.size(); i++)
				if (BTWN(this->origHitobjects[mid]->getTime(), _time, this->origHitobjects[mid + 1]->getTime()))
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
		
		int gettime = this->origHitobjects[mid]->getTime();
		if (_time < this->origHitobjects[mid]->getTime())
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


void Beatmap::ClearObjects()
{
	if (origHitobjects.size() == 0) return;
	for (auto* hitobject : origHitobjects)
		delete hitobject;
	
	origHitobjects.clear();
	std::vector<Hitobject*>().swap(origHitobjects);

	origTimingPoints.clear();
	std::vector<TimingPoint>().swap(origTimingPoints);
}