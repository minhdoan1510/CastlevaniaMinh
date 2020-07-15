#include <iostream>
#include <utility>
#include <vector>
#include <sys/types.h>
#include <filesystem>
#include <fstream>
#include "dirent.h"
#include <sstream>

using namespace std;

#define CELL_SIZE pair<int, int>(256,192)

typedef struct RECTR
{
	float left;
	float top;
	float right = left + 32;
	float bottom = top + 32;
};

vector<string> split(string line, string delimeter = "\t")
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}

vector<pair<int, int>> GetPosGrid(RECTR rectObj)
{
	vector<pair<int, int>> posCell;
	int minRow = int(rectObj.top) / CELL_SIZE.second;
	int maxRow = int(rectObj.bottom) / CELL_SIZE.second;
	int minColumn = int(rectObj.left) / CELL_SIZE.first;
	int maxColumn = int(rectObj.right) / CELL_SIZE.first;

	for (int i = minRow; i <= maxRow; i++)
	{
		for (int j = minColumn; j <= maxColumn; j++)
		{
			posCell.push_back(make_pair(i, j));
		}
	}
	return posCell;
}
#define MAX_GAME_LINE 1000
void HandleFileGrid(string path)
{
	ifstream f(path + '\\' + "obj.txt");
	ofstream fwrite(path+'\\'+"objnew.txt");
	int type;
	float _x, _y, _w, _h;
	int _type, _item, directStair, _direct;
	char str[MAX_GAME_LINE];
	string resuft;
	while (f.getline(str, MAX_GAME_LINE))
	{
		resuft.clear();
		string line(str);

		vector<string> tokens = split(line);
		if (tokens[0] == "-555")
		{
			fwrite << "-555";
			break;
		}
		if (tokens.size() <= 2) continue;
		if (tokens.at(0) == "//")
			continue;
		int xsizeframe = 32;
		int ysizeframe = 32;
		if (tokens[0] == "3")
		{
			xsizeframe = atoi(tokens[3].c_str());
			ysizeframe = atoi(tokens[4].c_str());
		}
		float x = atoi(tokens[1].c_str());
		float y = atoi(tokens[2].c_str());

		RECTR rect;
		rect.left = x;
		rect.top = y;
		rect.right = x + xsizeframe;
		rect.bottom = y + ysizeframe;

		vector<pair<int, int>> vtri = GetPosGrid(rect);

		std::stringstream ss;

		ss << vtri.size();

		std::string sService = ss.str();

		resuft = tokens[0] + '\t' + sService;

		for (int i = 0; i < vtri.size(); i++)
		{
			resuft += '\t';
			resuft += std::to_string(vtri.at(i).first);
			resuft += '\t';
			resuft += std::to_string(vtri.at(i).second);
		}
		for (int i = 1; i < tokens.size(); i++)
		{
			resuft += '\t';
			resuft += tokens[i];
		}
		fwrite << resuft << '\n';
	}
	f.close();
	fwrite.close();
}

int main() {
	string path;
	cout << "Nhap duong dan map game: ";
	cin >> path;

	DIR* dir;
	struct dirent* ent;
	if ((dir = opendir(path.c_str())) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_name[0] != '.' && ent->d_name[1] != '.')
			{
				HandleFileGrid(path + "\\" + ent->d_name);
			}
		}
		closedir(dir);
	}


	else {
		perror("");
		return EXIT_FAILURE;
	}
	return 0;
}