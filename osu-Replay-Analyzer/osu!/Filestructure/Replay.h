#ifndef REPLAY_H
#define REPLAY_H

#include <string>
#include <vector>
#include <tuple>

#include "../../irrlicht/include/vector2d.h"

#include "../osuEnums.h"
#include "../osuStructs.h"
#include "Beatmap.h"


class Replay
{
	friend class Play;

	public:
		Replay();
		virtual ~Replay();

		void ProcessReplay(std::string _filepath, Beatmap* _beatmap);
		
		osu::TIMING getFrameAt(long _time);
		osu::TIMING getFrame(int _frame) const;
		int getNumFrames();

		Mods getMods();

		bool isValid();
		bool isBeatmap(std::string* _MD5);
		bool isGamemode(GAMEMODE _gamemode);

	private:
		enum DATA
		{
			TIME = 0,
			MOUSE = 1,
			KEYBRD = 2
		};

		// Generated
		std::vector<osu::TIMING> replayStream;

		// Extracted from replay data
		std::string beatmapMD5;
		char gameMode;
		int version;
		std::string player;
		std::string replayMD5;
		short num300;
		short num200;
		short num50;
		short numMax;
		short num100;
		short numMiss;
		int score;
		short combo;
		char perfect;
		int mods;
		std::string graph;
		long long timestamp;
		
		Mods mod;

		// *.osr file reading
		bool ParseFile(std::ifstream &_replayFile);
		void ParseReplayData(unsigned char* _data, size_t _length, char _gamemode);
		std::string ReadOsuStr(std::ifstream &_replayFile);

		// Data processing
		void ValidateMods();
		void ApplyMods();

		int FindFrameAt(long _time);

	private:
		void ClearReplay();
};

#endif