#pragma once
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
#ifdef __linux__
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;
typedef long LONG;
typedef struct tagBITMAPINFOHEADER {
	DWORD      biSize;
	LONG       biWidth;
	LONG       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;
	LONG       biXPelsPerMeter;
	LONG       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagBITMAPFILEHEADER {
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BITMAPFILEHEADER;

/* constants for the biCompression field */
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

#else
#include "windows.h"
#endif

//struct lwrite
//{
//	unsigned long value;
//	unsigned      size;
//	lwrite(unsigned long value, unsigned size) :
//		value(value), size(size)
//	{ }
//};
//
////--------------------------------------------------------------------------
//inline std::ostream& operator << (std::ostream& outs, const lwrite& v)
//{
//	unsigned long value = v.value;
//	for (unsigned cntr = 0; cntr < v.size; cntr++, value >>= 8)
//		outs.put(static_cast <char> (value & 0xFF));
//	return outs;
//}

void intarray_to_bmp(const string& filename,
	unsigned char*    pixelarray,
	unsigned           rows,
	unsigned           columns
) {
	FILE* bmp_file = fopen(filename.c_str(), "wb");
	unsigned long headers_size = 14 + 40;//sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	unsigned long padding_size = (4 - ((columns * 3) % 4)) % 4;
	unsigned long pixel_data_size = rows * ((columns * 3) + padding_size);

	BITMAPFILEHEADER fileHeader;
	fileHeader.bfType = 0x4d42;
	fileHeader.bfSize = headers_size + pixel_data_size;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = headers_size;
	//fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmp_file);

	BITMAPINFOHEADER infoHeader;
	infoHeader.biSize = 40;// sizeof(BITMAPINFOHEADER);
	infoHeader.biWidth = columns;
	infoHeader.biHeight = rows;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 24;
	infoHeader.biCompression = BI_RGB;
	infoHeader.biSizeImage = pixel_data_size;
	infoHeader.biXPelsPerMeter = 0;
	infoHeader.biYPelsPerMeter = 0;
	infoHeader.biClrUsed = 0;
	infoHeader.biClrImportant = 0;
	//fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmp_file);

	// Write the BITMAPFILEHEADER
	fwrite(&fileHeader.bfType, 2, 1, bmp_file);                         // bfType
	fwrite(&fileHeader.bfSize, 4, 1, bmp_file);  // bfSize

	fwrite(&fileHeader.bfReserved1, 2, 1, bmp_file);   // bfReserved1
	fwrite(&fileHeader.bfReserved2, 2, 1, bmp_file);   // bfReserved2
	fwrite(&fileHeader.bfOffBits, 4, 1, bmp_file);    // bfOffBits

													  // Write the BITMAPINFOHEADER
	fwrite(&infoHeader.biSize, 4, 1, bmp_file);  // biSize
	fwrite(&infoHeader.biWidth, 4, 1, bmp_file);  // biWidth
	fwrite(&infoHeader.biHeight, 4, 1, bmp_file);  // biHeight
	fwrite(&infoHeader.biPlanes, 2, 1, bmp_file);  // biPlanes
	fwrite(&infoHeader.biBitCount, 2, 1, bmp_file);  // biBitCount
	fwrite(&infoHeader.biCompression, 4, 1, bmp_file);  // biCompression=BI_RGB
	fwrite(&infoHeader.biSizeImage, 4, 1, bmp_file);  // biSizeImage
	fwrite(&infoHeader.biXPelsPerMeter, 4, 1, bmp_file);  // biXPelsPerMeter
	fwrite(&infoHeader.biYPelsPerMeter, 4, 1, bmp_file);  // biYPelsPerMeter
	fwrite(&infoHeader.biClrUsed, 4, 1, bmp_file);  // biClrUsed
	fwrite(&infoHeader.biClrImportant, 4, 1, bmp_file);  // biClrImportant
	for (unsigned row = rows; row; row--)           // bottom-to-top
	{
		for (unsigned col = 0; col < columns; col++)  // left-to-right
		{
			unsigned char color = pixelarray[columns*(row - 1) + col];
			fwrite(&color, sizeof(unsigned char), 1, bmp_file);
			fwrite(&color, sizeof(unsigned char), 1, bmp_file);
			fwrite(&color, sizeof(unsigned char), 1, bmp_file);

		}
		if (padding_size != 0) {
			unsigned char* paddings = new unsigned char[padding_size];
			memset(paddings, 0, padding_size);
			fwrite(paddings, padding_size, 1, bmp_file);
			delete[] paddings;
		}
	}
	fclose(bmp_file);
}
