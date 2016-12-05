#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include "bmp_helper.h"
#ifdef WIN32
#include "windows.h"
#else
#include <sys/stat.h>
#endif

using namespace std;

char* WCs2MBs(const wchar_t * wcharStr) {
	char* str = NULL;
	int size = 0;
#ifdef __linux__
	setlocale(LC_ALL, "zh_CN.UTF8");
	size = wcstombs(NULL, wcharStr, 0);
	str = new char[size + 1];
	wcstombs(str, wcharStr, size);
	str[size] = '\0';
#else
	size = WideCharToMultiByte(CP_UTF8, 0, wcharStr, -1, NULL, NULL, NULL, NULL);
	str = new char[size];
	WideCharToMultiByte(CP_UTF8, 0, wcharStr, -1, str, size, NULL, NULL);
#endif 
	return str;
}

wchar_t * MBs2WCs(const char* pszSrc) {
	wchar_t* pwcs = NULL;
	int size = 0;
#ifdef __linux__
	setlocale(LC_ALL, "zh_CN.GB2312");
	size = mbstowcs(NULL, pszSrc, 0);
	pwcs = new wchar_t[size + 1];
	size = mbstowcs(pwcs, pszSrc, size + 1);
	pwcs[size] = 0;
#else
	size = MultiByteToWideChar(20936, 0, pszSrc, -1, 0, 0);
	if (size <= 0)
		return NULL;
	pwcs = new wchar_t[size];
	MultiByteToWideChar(20936, 0, pszSrc, -1, pwcs, size);
#endif 
	return pwcs;
}
class Color {
private:

public:
	static const unsigned int WHITE = 0xffffffff;
	static const unsigned int RED = 0xffff0000;
	static const unsigned int GREEN = 0xff00ff00;
	static const unsigned int BLUE = 0xff0000ff;
	static const unsigned int BLACK = 0xff000000;
	static unsigned int FromARGB(unsigned int a, unsigned int r, unsigned int g, unsigned int b) {
		uint32_t va = a << 24;
		uint32_t vr = r << 16;
		uint32_t vg = g << 8;
		uint32_t vb = b;
		uint32_t result = va + vr + vg + vb;
		return result;
	}

	unsigned color;
	Color() {
		color = 0;
	}
	Color(unsigned int color) :color(color) {
	}
	std::vector<int> GetARGBs() {
		int a = color >> 24;
		int r = (color << 8) >> 24;
		int g = (color << 16) >> 24;
		int b = (color << 24) >> 24;
		return{ a,r,g,b };
	}
};
struct GNT {
public:
	GNT(FILE* gnt_file) {
		fread(this, 10, 1, gnt_file);
		int img_size = sample_size - 10;
		data = new unsigned char[img_size];
		fread(data, img_size, 1, gnt_file);
	}

	GNT(char* gnt_file_name, int64_t offset) {
		FILE* gnt_file = fopen(gnt_file_name, "rb");
#ifdef __linux__
		fseeko(gnt_file, offset, 0);
#else
		_fseeki64(gnt_file, offset, 0);
#endif

		fread(this, 10, 1, gnt_file);
		int img_size = sample_size - 10;
		data = new unsigned char[img_size];
		fread(data, img_size, 1, gnt_file);

		fclose(gnt_file);
	}
	GNT(GNT & gnt) {
		memcpy(this, &gnt, 10);
		int img_size = sample_size - 10;
		data = new unsigned char[img_size];
		memcpy(data, gnt.data, img_size);
	}
	~GNT() {
		delete[] data;
	}
	unsigned int sample_size;
	char tag[2];
	unsigned short width;
	unsigned short height;
	unsigned char* data;

	string getTag() {
		if (tag[1] == '\0') {
			return string(tag);
		}
		else {
			char buffer[3];
			memcpy(buffer, tag, 2);
			buffer[2] = '\0';
			string result = string(buffer);
			return result;
		}
	}

	wstring getWTag() {
		if (tag[1] == '\0') {
			return wstring(MBs2WCs(tag));
		}
		else {
			char buffer[3];
			memcpy(buffer, tag, 2);
			buffer[2] = '\0';
			wchar_t* wchar = MBs2WCs(buffer);
			wstring result = wstring(MBs2WCs(buffer));
			delete[] wchar;
			return result;
		}
	}

	void save_BMP_file(string file_name) {
		intarray_to_bmp(file_name.c_str(), data, height, width);
	}
};

struct GNTSampleInfo {
public:

#define MAX_FILENAME_LENGTH 64
	GNTSampleInfo() {

	}
	GNTSampleInfo(FILE* bin_file) {

		char buffer[MAX_FILENAME_LENGTH + 16];
		fread(buffer, MAX_FILENAME_LENGTH + 16, 1, bin_file);
		memcpy(file_name, buffer, MAX_FILENAME_LENGTH);
		memcpy(&code, buffer + MAX_FILENAME_LENGTH, 4);
		memcpy(&index, buffer + MAX_FILENAME_LENGTH + 4, 4);
		memcpy(&image_offset, buffer + MAX_FILENAME_LENGTH + +8, 8);

		/*
		cout<<"Bla"<<endl;
		fread(file_name, MAX_FILENAME_LENGTH, 1, bin_file);
		fread(&code, 4, 1, bin_file);
		fread(&index, 4, 1, bin_file);
		fread(&code, 4, 1, bin_file);
		fread(&image_offset, 8, 1, bin_file);
		*/
	}
	char file_name[MAX_FILENAME_LENGTH];
	uint32_t code;
	uint32_t index;
#ifdef __linux
	int64_t image_offset;
#else
	int64_t image_offset;
#endif
	void save_to_file(FILE* bin_file) {
		fwrite(file_name, MAX_FILENAME_LENGTH, 1, bin_file);
		fwrite(&code, 4, 1, bin_file);
		fwrite(&index, 4, 1, bin_file);
		fwrite(&image_offset, 8, 1, bin_file);
	}
};
