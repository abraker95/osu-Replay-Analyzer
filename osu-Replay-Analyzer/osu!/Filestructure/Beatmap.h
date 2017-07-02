#ifndef BEATMAP_H
#define BEATMAP_H

#include <vector>
#include <string>

#include "../../utils/DB/Database.h"
#include "../osuEnums.h"
#include "TimingPoint.h"
#include "Hitobject.h"
#include "Mods.h"

class Beatmap
{
	friend class Play;

	public:
		struct Diff
		{
			double hp = -1;
			double sm = -1;     // slider multiplier
			double st = -1;     // slider tick rate
		};

		struct Metadata
		{
			int format = -1;	 // *.osu format
			std::string artist;
			std::string title;
			std::string version; // difficulty name
			std::string creator;
			std::string name;	 // Artist - Title (Creator) [Difficulty]
			std::string BeatmapMD5; // generated
		};

		Beatmap();
		virtual ~Beatmap();

		void Read(std::string _beatmapfile);
		void Process();
		bool isValid();
		
		std::string getName();
		std::string getMD5();
		GAMEMODE getGamemode();
		Database<Hitobject>& getHitobjects();

		Mods getMods();
		Diff& getDiff();
		void setDiff(Diff _diff);

		void ClearModified();
		void ResetModified();

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
			SECTION_COLOURS,
			SECTION_HITOBJECTS
		};

		Diff diff;
		Mods mods;
		Metadata metadata;
		GAMEMODE gamemode;

		std::vector<TimingPoint> origTimingPoints, modTimingPoints;
		
		//std::vector<Hitobject*> origHitobjects, modHitobjects;
		Database<Hitobject> origHitobjects, modHitobjects;

		/// \NOTE: unused for now
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
		bool ParseColourSection(std::ifstream &_filepath, std::string &_line);
		bool ParseHitobjectsSection(std::ifstream &_filepath, std::string &_line);
	
		void PrepareTimingPoints();

		int ReadTimingpoints(std::string line);
		int ReadHitobjects(std::string line);

		void PrepareSliderData();
		void GenerateSliderMetadata();

		void ClearObjects();
};

#endif