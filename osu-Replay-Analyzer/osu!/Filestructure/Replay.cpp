#include "Replay.h"
#include "Beatmap.h"
#include "filereader.h"
#include "LZMA.h"
#include "../../utils/mathUtils.h"


#include <fstream>
#include <assert.h>

Replay::Replay(){}

void Replay::ProcessReplay(std::string _filepath, Beatmap* _beatmap)
{
	// Can't have a replay without a beatmap
	if (!_beatmap->isValid())
		return;

	std::ifstream replayFile(_filepath, std::ios::binary);
	if (replayFile.is_open())
	{
		bool success = this->ParseFile(replayFile);
		assert(success == true);
		replayFile.close();
	}

	mod = _beatmap->getMods();
	this->ValidateMods();

	/// \TODO: Make sure the stream data is ordered time-wise
}

std::tuple<irr::core::vector2df, int> Replay::getDataAt(long _time)
{
	int frame = FindFrameAt(_time);

	if(_time != -1 && frame != -1)
		return std::tuple<irr::core::vector2df, int>(std::get<DATA::MOUSE>(replayStream[frame]), std::get<DATA::KEYBRD>(replayStream[frame]));
	else 
		return std::tuple<irr::core::vector2df, int>(irr::core::vector2df(-1, -1), 0);
}

std::tuple<long, irr::core::vector2df, int> Replay::getFrame(int _frame) const
{
	if(_frame < replayStream.size())
		return replayStream[_frame];
	else
		return std::tuple<long, irr::core::vector2df, int>(-1, irr::core::vector2df(-1, -1), -1);
}

int Replay::getNumFrames()
{
	return replayStream.size();
}

Mods Replay::getMods()
{
	return mod;
}

bool Replay::isValid()
{
	return (replayStream.size() != 0);
}

bool Replay::isBeatmap(std::string* _MD5)
{
	return (beatmapMD5 == *_MD5);
}

bool Replay::isGamemode(GAMEMODE _gamemode)
{
	return (gameMode == _gamemode);
}

// ------- [PRIVATE] --------

int Replay::FindFrameAt(long _time)
{
	int start = 1;
	int end = this->replayStream.size() - 2;
	int mid;

	while (start <= end)
	{
		mid = (start + end) / 2;
		if (BTWN(std::get<DATA::TIME>(replayStream[mid]), _time, std::get<DATA::TIME>(replayStream[mid - 1])))
			return max(0, mid - 1);
		else if (_time < std::get<DATA::TIME>(replayStream[mid]))
			end = mid - 1;
		else start = mid + 1;
	}

	return -1;
}

std::string Replay::ReadOsuStr(std::ifstream &_replayFile)
{
	char chr;
	std::string str;

	// check if there is something to read first
	_replayFile.read(&chr, sizeof(chr));
	if (chr != '\0')
	{
		// If it begins with this, play by peppy string rules
		if (chr == 0x0b)
		{
			// get the first part of the peppy string; size of the string
			std::string size_str;
			do
			{
				_replayFile.read(&chr, sizeof(chr));
				size_str += chr;
			} while (chr < 0);

			// read the peppy string
			int size = decodeULEB128((const uint8_t*)size_str.data());
			char* tmp = new char[size+1];
				_replayFile.read(tmp, size);
				tmp[size] = '\0';
				str.assign(tmp);
			delete tmp;
		}
		else // otherwise, take it as a normal null terminated string
		{
			str += chr;
			_replayFile.read(&chr, sizeof(chr));
			
			while (chr != '\0')
			{
				str += chr;
				_replayFile.read(&chr, sizeof(chr));
			}
		}
	}

	return str;
}

bool Replay::ParseFile(std::ifstream &_replayFile)
{
	_replayFile.read(&gameMode,       sizeof(gameMode));
	_replayFile.read((char*)&version, sizeof(version));
	
	beatmapMD5 = ReadOsuStr(_replayFile);
	player     = ReadOsuStr(_replayFile);
	replayMD5  = ReadOsuStr(_replayFile);

	_replayFile.read((char*)&num300,  sizeof(num300));
	_replayFile.read((char*)&num200,  sizeof(num200));
	_replayFile.read((char*)&num50,   sizeof(num50));
	_replayFile.read((char*)&numMax,  sizeof(numMax));
	_replayFile.read((char*)&num100,  sizeof(num100));
	_replayFile.read((char*)&numMiss, sizeof(numMiss));
	_replayFile.read((char*)&score,   sizeof(score));
	_replayFile.read((char*)&combo,   sizeof(combo));
	_replayFile.read((char*)&perfect, sizeof(perfect));
	_replayFile.read((char*)&mods,    sizeof(mods));
	
	graph = ReadOsuStr(_replayFile);
	
	_replayFile.read((char*)&timestamp, sizeof(timestamp));


	// Stream data
	int replaySize;
		_replayFile.read((char*)&replaySize, sizeof(replaySize));
		
	char* data = new char[replaySize];
		_replayFile.read(data, replaySize);

	// Data generation from compressed stream data
	// \TODO: Put this in its own function
	LZMA::DataStream stream;
		stream.inData = (const unsigned char*)data;
		stream.inLen = replaySize;
		stream.outData = nullptr;
		stream.outLen = 0;

	LZMA::DecompressStreamData(&stream);
	delete data;

	ParseReplayData(stream.outData, stream.outLen, gameMode);

	return true;
}

/*
std::string mods2str(MODS mods)
{
std::string modStr;

if (mods & EZ) modStr += "EZ";
if (mods & HR) modStr += "HR";
if (mods & HT) modStr += "HT";
if (mods & DT) modStr += "DT";
if (mods & FL) modStr += "FL";
if (mods & HD) modStr += "HD";

return modStr;
}
*/

void Replay::ParseReplayData(unsigned char* _data, size_t _length, char _gamemode)
{
	std::string Replaydata = std::string((const char*)_data);
	std::tuple<long, irr::core::vector2df, int> dataPointStruct;
	int time = 0;

	std::vector<std::string> dataPoints;
	FileReader::tokenize(Replaydata, dataPoints, ",");

	for (std::string dataPoint : dataPoints)
	{
		std::vector<std::string> data;
		FileReader::tokenize(dataPoint, data, "|");

		if (data.size() >= 4) // there must be at least 4 data indentifiers for a complete datapoint
		{
			time += atoi(data[0].data());

			std::get<DATA::TIME>(dataPointStruct) = time;
			std::get<DATA::MOUSE>(dataPointStruct).X = atoi(data[1].data());
			std::get<DATA::MOUSE>(dataPointStruct).Y = atoi(data[2].data());
			std::get<DATA::KEYBRD>(dataPointStruct) = atoi(data[3].data());

			if (std::get<DATA::TIME>(dataPointStruct) != -12345) // Record if it's not this...
				replayStream.push_back(dataPointStruct);
		}
	}
}

void Replay::ValidateMods()
{
	// If one of these asserts triggers, then someone has been hacking the replay
	assert(((this->mods & EZ) != true) && ((this->mods & HR) != true));
	assert(((this->mods & DT) != true) && ((this->mods & HT) != true));
	assert(((this->mods & HD) != true) && ((this->mods & FL) != true));

	mod.setTM(1.0);

	// Set mod flags
	if (mods & EZ) mod.setModifier(Mods::MODS::EZ);
	if (mods & HR) mod.setModifier(Mods::MODS::HR);
	if (mods & HT) mod.setModifier(Mods::MODS::HT);
	if (mods & DT) mod.setModifier(Mods::MODS::DT);
	if (mods & FL) mod.setModifier(Mods::MODS::FL);
	if (mods & HD) mod.setModifier(Mods::MODS::HD);
	if (mods & FI) mod.setModifier(Mods::MODS::FI);

	// adjust frame timings due to DT or HT mod
	double divisor = mod.getTM();
	if (divisor == 1.0) return;

	// Mod replay stream timings
	for (auto &frame : replayStream)
		std::get<0>(frame) /= divisor;
}