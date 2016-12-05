#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "string_helper.h"
#include "gnt_helper.h"
#ifdef WIN32
#else
#include <sys/stat.h>
#endif
using namespace std;

map<unsigned int, vector<GNTSampleInfo>> ReadGNTInfoFile(string info_name) {
	FILE* bin_file = fopen(info_name.c_str(), "rb");
	int count = 0;
	map<unsigned int, vector<GNTSampleInfo>> hanzImageMap;

	while (!feof(bin_file)) {

		int c = getc(bin_file);
		if (c == EOF) {
			break;
		}
		ungetc(c, bin_file);

		GNTSampleInfo info(bin_file);

		if (hanzImageMap.find(info.code) == hanzImageMap.end()) {
			// Key not found
			/*if (info_file.index == 1246990) {
			cout << "Builded" << endl;
			}*/
			hanzImageMap[info.code] = {};
		}
		hanzImageMap[info.code].push_back(info);
		count++;
		//cout<<"pro:"<<count<<endl;

	}
	fclose(bin_file);
	return hanzImageMap;
}

void MakeDir(const char* dir) {
#ifdef WIN32
	CreateDirectoryA(dir, nullptr);
#else
	mkdir(dir, 0700);
#endif
}
void ExtractFromCode(map<unsigned int, vector<GNTSampleInfo>> gnt_info_dict, uint32_t code, float trainPercent=1.0) {
	vector<GNTSampleInfo> sampleList = gnt_info_dict[code];
	int seperationPos = trainPercent*sampleList.size();

	for (int i = 0; i < seperationPos; i++) {
		GNT sample(sampleList[i].file_name, sampleList[i].image_offset);
		string bmp_name = to_string(sampleList[i].index) + "_" + to_string(sampleList[i].image_offset) + ".bmp";
		string dirName = "out/train/" + to_string(code);
		MakeDir(dirName.c_str());
		sample.save_BMP_file(dirName + "/" + bmp_name);
	}
	for (int i = seperationPos; i < sampleList.size(); i++) {
		GNT sample(sampleList[i].file_name, sampleList[i].image_offset);
		string bmp_name = to_string(sampleList[i].index) + "_" + to_string(sampleList[i].image_offset) + ".bmp";
		string dirName = "out/validation/" + to_string(code);
		MakeDir(dirName.c_str());
		sample.save_BMP_file(dirName + "/" + bmp_name);
	}
	/*for (auto& info : gnt_info_dict[code]) {
		GNT sample(info.file_name, info.image_offset);
		string bmp_name = to_string(info.index) + "_" + to_string(info.image_offset) + ".bmp";
		string dirName = "out/" + to_string(code);
		MakeDir(dirName.c_str());
		sample.save_BMP_file(dirName + "/" + bmp_name);
	}*/
}

int main(int argn, char* argv[]) {
	//Z:\Project\hanz\1.0train-gb1.gnt.txt
	//char* path= "Z:/Project/hanz/1.0train-gb1.gnt.bin";
	//char* path = "f2.gnt.bin";

	map<unsigned int, vector<GNTSampleInfo>> gnt_info_dict = ReadGNTInfoFile(argv[1]);
	cout << "Loaded " << gnt_info_dict.size() << " characters, enter character codes(seperate by comma)." << endl;
	string codesLine;
	cin >> codesLine;

	MakeDir("out");
	MakeDir("out/train");
	MakeDir("out/validation");

	vector<string> codes = split(codesLine, ',');
	for (string & codeStr : codes) {
		uint32_t code = stoi(codeStr);
		if (codeStr.find("-") != string::npos) {
			vector<string> codePair = split(codeStr,'-');
			int start = stoi(codePair[0]);
			int end = stoi(codePair[1]);
			
			for (int i = start; i != end; end > start ? i++ : i--) {
				ExtractFromCode(gnt_info_dict, i, 0.8);
			}
		}
		else {
			ExtractFromCode(gnt_info_dict, code, 0.8);
		}
	}

	//map<unsigned int, vector<GNTSampleInfo>> gnt_info_dict = ReadGNTInfoFile(path);

	return 0;
}
