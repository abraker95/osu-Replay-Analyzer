#ifndef BEATMAP_H
#define BEATMAP_H

#include <vector>
#include <string>

#include "osuEnums.h"
#include "TimingPoint.h"
#include "Hitobject.h"
#include "Hitcircle.h"

class Beatmap
{
	friend class Play;

	public:
		struct Diff
		{
			double ar = -1;
			double cs = -1;
			double od = -1;
			double hp = -1;
			double sm = -1;     // slider multiplier
			double st = -1;     // slider tick rate
		};

		struct Modifier
		{
			bool hidden = false;
			bool fadeIn = false;
			bool flashlight = false;
		};

		struct Metadata
		{
			int format = -1;	 // *.osu format
			std::string artist;
			std::string title;
			std::string version; // difficulty name
			std::string creator;
			std::string name;	 // Artist - Title (Creator) [Difficulty]
		};

		Beatmap(std::string file);
		virtual ~Beatmap();

		void Process();

		std::vector<Hitobject*> hitObjects;
		std::string getName();
		
		GAMEMODE getGamemode();

		Diff& getDiff();
		void setDiff(Diff _diff);
		void resetDiff();

		Modifier& getModifiers();
		void setModifiers(Modifier _modifier);
		void resetModifiers();

		int getNumHitobjectsVisibleAt(int _index, double _opacity);
		std::pair<int, int> getIndicesVisibleAt(int _time, double _opacity);

		TimingPoint* getTimingPointAt(int _time);
		Hitobject* getHitobjectAt(int _time);
		int FindHitobjectAt(int _time);

	private:
		enum SECTION
		{
			SECTION_NONE,
			SECTION_GENERAL,
			SECTION_EDITOR,
			SECTION_METADATA,
			SECTION_DIFFICULTY,
			SECTION_EVENTS,
			SECTION_TIMINGPOINTS,
			SECTION_HITOBJECTS
		};

		Diff modifiedDiff, origDiff;
		Modifier modifiedMod, origMod;
		Metadata metadata;
		GAMEMODE gamemode;

		std::vector<TimingPoint> timingPoints;
		std::vector<std::pair<Hitobject*, int>> hitObjectsTimeStart;
		std::vector<std::pair<Hitobject*, int>> hitObjectsTimeEnd;

		// prepared data
		double bpmMin;
		double bpmMax;

		int spinners = 0;

		bool ParseBeatmap(std::ifstream &_filepath);
		void ParseBeatmapFormat(std::ifstream &_filepath);
		bool ParseBeatmapData(std::ifstream &_filepath);
		void ParseSection(std::ifstream &_filepath, SECTION _section, std::string &_line);

		bool ParseGeneralSection(std::ifstream &_filepath, std::string &_line);
		bool ParseEditorSection(std::ifstream &_filepath, std::string &_line);
		bool ParseMetadataSection(std::ifstream &_filepath, std::string &_line);
		bool ParseDifficultysection(std::ifstream &_filepath, std::string &_line);
		bool ParseEventsSection(std::ifstream &_filepath, std::string &_line);
		bool ParseTimingPointsSection(std::ifstream &_filepath, std::string &_line);
		bool ParseHitobjectsSection(std::ifstream &_filepath, std::string &_line);
	
		void PrepareTimingPoints();

		int ReadTimingpoints(std::string line);
		int ReadHitobjects(std::string line);

		//int getRepeatTimes(Hitobject* _hitObject);

		void PrepareSliderData();
		void GenerateSliderPoints();

		void SortEndTimes(int _left, int _right);
};

#endif