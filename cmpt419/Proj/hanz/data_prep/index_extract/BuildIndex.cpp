#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "gnt_helper.h"
using namespace std;


void BuildGNTIndex(string file_name) {
	FILE* gnt_file = fopen(file_name.c_str(), "rb");
	map<string, int> codeMap;
	fstream txt_file;
	string txt_file_name = file_name + string(".txt");
	txt_file.open(txt_file_name, std::fstream::out);
	FILE* bin_file = fopen((file_name + string(".bin")).c_str(), "wb");

	int count = 0;

	//fseek(gnt_file, 0, SEEK_END);
	//int size = ftell(gnt_file);
	int seq = 0;
	//fseek(gnt_file, 0, 0);

	while (!feof(gnt_file)) {
		//fpos_t position;
		//fgetpos(gnt_file, &position);
#ifdef __linux
		int64_t position = ftello(gnt_file);
#else
		int64_t position = _ftelli64(gnt_file);
#endif // __linux

		int c = getc(gnt_file);
		if (c == EOF) {
			break;
		}
		ungetc(c, gnt_file);

		GNT sample(gnt_file);
		string tag = sample.getTag();
		if (codeMap.find(tag) == codeMap.end()) {
			//Key not found
			codeMap[tag] = seq;
			seq++;
		}
		/*if (position == 4310642373) {
		cout << "shsdh" << endl;
		}*/
		string sample_id = file_name + "_" + to_string(codeMap[tag]) + "_" + to_string(count) + "_" + to_string(position);
		txt_file << sample_id << "," << tag;
		string bmp_name = sample_id + ".bmp";
		//sample.save_BMP_file("out/"+bmp_name);
		txt_file << endl;
		GNTSampleInfo info;
		memcpy(info.file_name, file_name.c_str(), file_name.size() + 1);
		info.image_offset = position;
		info.index = count;
		info.code = codeMap[tag];
		info.save_to_file(bin_file);

		//if (hanzImageMap.find(tag) == hanzImageMap.end()) {
		//	// Key not found
		//	hanzImageMap[tag] = {};
		//}

		//GNTSampleInfo sample_info;
		//sample_info.file_name = file_name;
		//sample_info.index = count;
		//sample_info.image_offset = position;

		//hanzImageMap[tag].push_back(sample_info);
		count++;
	}
	fclose(bin_file);
	fclose(gnt_file);
	txt_file.close();
}

void BuildSampleMap() {
	map<string, vector<GNTSampleInfo>> hanzImageMap;

}
int main(int argn, char* argv[]) {


	BuildGNTIndex(argv[1]);
	//char* path = "Z:/Project/hanz/1.0train-gb1.gnt";
	//GNT s(path, 4310649763);
	//BuildGNTIndex(path);
	return 0;
}
