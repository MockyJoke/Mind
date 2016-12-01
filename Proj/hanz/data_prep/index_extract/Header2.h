#pragma once
// intarray2bmp.hpp
//
// Convert an array of integer values to a bitmap.
// Copyright 2009 Michael Thomas Greer
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#ifndef INTARRAY2BMP_HPP
#define INTARRAY2BMP_HPP

#include <fstream>
#include <iostream>
#include <string>

namespace intarray2bmpns
{

	//-------------------------------------------------------------------------- 
	// This little helper is to write little-endian values to file.
	//
	struct lwrite
	{
		unsigned long value;
		unsigned      size;
		lwrite(unsigned long value, unsigned size) :
			value(value), size(size)
		{ }
	};

	//--------------------------------------------------------------------------
	inline std::ostream& operator << (std::ostream& outs, const lwrite& v)
	{
		unsigned long value = v.value;
		for (unsigned cntr = 0; cntr < v.size; cntr++, value >>= 8)
			outs.put(static_cast <char> (value & 0xFF));
		return outs;
	}

	//--------------------------------------------------------------------------
	// Take an integer array and convert it into a color image.
	//
	// This first version takes an array of array style of array:
	//   int* a[ 10 ]
	//
	// The second, overloaded version takes a flat C-style array:
	//   int a[ 10 ][ 10 ]
	//
	template <typename IntType>
	bool intarray2bmp(
		const std::string& filename,
		IntType**          intarray,
		unsigned           rows,
		unsigned           columns,
		IntType            min_value,
		IntType            max_value
	) {
		// This is the difference between each color based upon
		// the number of distinct values in the input array.
		double granularity = 360.0 / ((double)(max_value - min_value) + 1);

		// Open the output BMP file
		std::ofstream f(filename.c_str(),
			std::ios::out | std::ios::trunc | std::ios::binary);
		if (!f) return false;

		// Some basic
		unsigned long headers_size = 14  // sizeof( BITMAPFILEHEADER )
			+ 40; // sizeof( BITMAPINFOHEADER )
		unsigned long padding_size = (4 - ((columns * 3) % 4)) % 4;
		unsigned long pixel_data_size = rows * ((columns * 3) + padding_size);

		// Write the BITMAPFILEHEADER
		f.put('B').put('M');                           // bfType
		f << lwrite(headers_size + pixel_data_size, 4);  // bfSize
		f << lwrite(0, 2);  // bfReserved1
		f << lwrite(0, 2);  // bfReserved2
		f << lwrite(headers_size, 4);  // bfOffBits

									   // Write the BITMAPINFOHEADER
		f << lwrite(40, 4);  // biSize
		f << lwrite(columns, 4);  // biWidth
		f << lwrite(rows, 4);  // biHeight
		f << lwrite(1, 2);  // biPlanes
		f << lwrite(24, 2);  // biBitCount
		f << lwrite(0, 4);  // biCompression=BI_RGB
		f << lwrite(pixel_data_size, 4);  // biSizeImage
		f << lwrite(0, 4);  // biXPelsPerMeter
		f << lwrite(0, 4);  // biYPelsPerMeter
		f << lwrite(0, 4);  // biClrUsed
		f << lwrite(0, 4);  // biClrImportant

							// Write the pixel data
		for (unsigned row = rows; row; row--)           // bottom-to-top
		{
			for (unsigned col = 0; col < columns; col++)  // left-to-right
			{
				unsigned char red, green, blue;
				//
				// This is how we convert an integer value to a color:
				// by mapping it evenly along the CIECAM02 hue color domain.
				//
				// http://en.wikipedia.org/wiki/Hue
				// http://en.wikipedia.org/wiki/hsl_and_hsv#conversion_from_hsv_to_rgb
				//
				// The following algorithm takes a few shortcuts since
				// both 'value' and 'saturation' are always 1.0.
				//
				double hue = (intarray[row - 1][col] - min_value) * granularity;
				int    H = (int)(hue / 60) % 6;
				double F = (hue / 60) - H;
				double Q = 1.0 - F;

#define c( x ) (255 * x)
				switch (H)
				{
				case 0:  red = c(1);  green = c(F);  blue = c(0);  break;
				case 1:  red = c(Q);  green = c(1);  blue = c(0);  break;
				case 2:  red = c(0);  green = c(1);  blue = c(F);  break;
				case 3:  red = c(0);  green = c(Q);  blue = c(1);  break;
				case 4:  red = c(F);  green = c(0);  blue = c(1);  break;
				default: red = c(1);  green = c(0);  blue = c(Q);
				}
#undef c

				f.put(static_cast <char> (blue))
					.put(static_cast <char> (green))
					.put(static_cast <char> (red));
			}

			if (padding_size) f << lwrite(0, padding_size);
		}

		// All done!
		return f.good();
	}

	//--------------------------------------------------------------------------
	template <typename IntType>
	bool intarray2bmp(
		const std::string& filename,
		IntType*           intarray,
		unsigned           rows,
		unsigned           columns,
		IntType            min_value,
		IntType            max_value
	) {
		IntType** ia = new(std::nothrow) IntType*[rows];
		for (unsigned row = 0; row < rows; row++)
		{
			ia[row] = intarray + (row * columns);
		}
		bool result = intarray2bmp(
			filename, ia, rows, columns, min_value, max_value
		);
		delete[] ia;
		return result;
	}

} // namespace intarray2bmp

#endif