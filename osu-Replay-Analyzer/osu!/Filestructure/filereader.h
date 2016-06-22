#ifndef _filereader_h_
#define _filereader_h_

#include "dirent.h"

#include <cstring>
#include <string>
#include <vector>
#include <fstream>

static std::string getStrAfter(std::string str, std::string txt)
{
	std::size_t pos = str.rfind(txt);
	return (pos != std::string::npos)? str.substr(pos + txt.size()) : "";
}

namespace FileReader
{
	template < class ContainerT >
	void tokenize(const std::string& str, ContainerT& tokens,
		          const std::string& delimiters = " ", bool trimEmpty = false)
	{
		std::string::size_type pos, lastPos = 0;

		using value_type = typename ContainerT::value_type;
		using size_type = typename ContainerT::size_type;

		while (true)
		{
			pos = str.find_first_of(delimiters, lastPos);
			if (pos == std::string::npos)
			{
				pos = str.length();

				if (pos != lastPos || !trimEmpty)
					tokens.push_back(value_type(str.data() + lastPos,
									 (size_type)pos - lastPos));

				break;
			}
			else
			{
				if (pos != lastPos || !trimEmpty)
					tokens.push_back(value_type(str.data() + lastPos,
									 (size_type)pos - lastPos));
			}

			lastPos = pos + 1;
		}
	}

	inline bool GetFolderFileList(std::string folder, std::vector<std::string> &fileList)
	{
		DIR *pDir;
		struct dirent *pDirent;

		pDir = opendir(folder.c_str());
		if (pDir == NULL)
		{
			//cout << "Cannot open directory " << folder << endl;
			return 0;
		}

		while ((pDirent = readdir(pDir)) != NULL)
		{
			if (!strcmp(pDirent->d_name, ".") || !strcmp(pDirent->d_name, ".."))
				continue;

			std::string dir(folder + '\\' + pDirent->d_name);
			DIR *pDir2 = opendir(dir.c_str());
			if (pDir2 == NULL)
			{
				//cout << "Not a dir... Found file: " << dir << endl;

				// Adding "'s to properly process paths with spaces later on
				dir = "\"" + dir + "\"";
				fileList.push_back(dir);
			}
			else
			{
				struct dirent *pDirent2;
				while ((pDirent2 = readdir(pDir2)) != NULL)
				{
					if (strstr(pDirent2->d_name, ".osu") == nullptr)
						continue;
					if (!strcmp(pDirent2->d_name, ".") || !strcmp(pDirent2->d_name, ".."))
						continue;

					std::string file = dir + "\\" + pDirent2->d_name;
					DIR *pDir3 = opendir(file.c_str());

					if (pDir3 == NULL)
					{
						//cout << "Found file: " << file << endl;

						// Adding "'s to properly process paths with spaces later on
						file = "\"" + file + "\"";
						fileList.push_back(file);
					}
					closedir(pDir3);
				}
				closedir(pDir2);
			}
		}
		closedir(pDir);

		return 1;
	}
}
#endif
