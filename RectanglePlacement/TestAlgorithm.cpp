/**

TestAlgorithm.cpp
Purpose: Invoke the RectLoader Algorithm and fill the square 
		 layout with Rectangles provided in input text file.

		 Input file(Input.txt) have to be in the same folder as the source code.
		 Input file contains rectangle dimensions.

@author Dinesh Kumar Sivaji
@version 0.1 17/10/2018 Initial Version
		 0.2 18/10/2018 Formatting correction
		 0.3 18/10/2018 Added Clock to find the execution time 
		 0.4 19/10/2018 Added few File validation
		 0.5 19/10/2018 Included Stress Test
*/
#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>   // For sorting
#include <fstream>
#include <sstream>
#include <iterator>
#include <chrono>   // For Clock
#include "RectLoader.h";

using namespace std;
using namespace std::chrono;


const char DELIMITER = ',';				  // Delimiter
const string FILEPATH = "Input.txt";      // Input file path


// --------------------------------------------------------------------------------
// Name        : SubRect
// Description : Struct for Rectangle
// --------------------------------------------------------------------------------
struct SubRect : public RectLoader::Square
{
	int n;      // Original index of this subrect, before sorting

	SubRect() { }
	SubRect(int _w, int _h, int _n) : Square(0, 0, _w, _h), n(_n) { }
};

typedef vector<SubRect> SubRectArray;		// typedef for simplicity
typedef vector<RectLoader> LayoutArray;

// --------------------------------------------------------------------------------
// Name        : splitString
// Description : Here we split the string with given delimiter
//				 TODO: function can be extended to number and other validation
// @param	   : Rectangle dimensions in string and delimiter in char
// @return	   : a string array list of Rectangle width and height
// --------------------------------------------------------------------------------
vector<string> splitString(const string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

// --------------------------------------------------------------------------------
// Name        : LoadRectangle
// Description : Here we can get rectangle dimensions from file,
//               and load the rectangles inside dynamic array (Vector)
// @param	   : a sub Rectangle array list
// --------------------------------------------------------------------------------
void loadRectangle(SubRectArray &subRects)
{
	subRects.clear();

	string itemName;
	ifstream inFile;
	char data[100];

	inFile.open(FILEPATH);
	if (inFile.fail()) { //can't find file, it will fail
		cout << "Input file not found!!" << endl;
		exit(1);
	}
	int i = 0;
	while (inFile >> itemName)
	{
		vector<string> subString = splitString(itemName, DELIMITER);
		if (subString.size() == 2)
		{
			unsigned int w = abs((subString[0] == "") ? 0 : stoi(subString[0]));  // Sting, negative number conversion 
			unsigned int h = abs((subString[1] == "") ? 0 : stoi(subString[1])); // Sting, negative number conversion 
			if (w != 0 && h != 0)
			{
				subRects.push_back(SubRect(w, h, i));
				i++;
			}
		}
	}
	inFile.close();
}


// --------------------------------------------------------------------------------
// Name        : createLayout
// Description : Here we can create layout containing all subrects
// @param	   : a Layout array list and a sub Rectangle array list
// --------------------------------------------------------------------------------
void createLayout(LayoutArray &rectLayout, SubRectArray &subRects)
{
	// Sort the subRects based on dimensions, larger dimension goes first.
	std::sort(subRects.begin(), subRects.end(), RectLoader::Square::Greater);

	// Generate the first Layout
	rectLayout.clear();
	rectLayout.push_back(RectLoader());

	// Add all subrects
	for (SubRectArray::iterator it = subRects.begin();
		it != subRects.end();
		++it)
	{
		// We leave one pixel between subrects, just not to overlap.
		RectLoader::Square r(0, 0, it->w + 1, it->h + 1);
		int i = 0;
		// If an existing texture has actual space 
		bool bPlaced = false;
		bPlaced = rectLayout[i].AddAtEmptySpotAutoGrow(&r);

		// If correctly placed in a layout, the coordinates are returned in r.x and r.y
		if (bPlaced)
		{
			it->x = r.x;
			it->y = r.y;
		}
	}
}

// --------------------------------------------------------------------------------
// Name        : main
// Description : Here we have mail function
// --------------------------------------------------------------------------------
int main()
{
	auto start = high_resolution_clock::now();

	SubRectArray subRects;
	LayoutArray layout;

	loadRectangle(subRects);
	createLayout(layout, subRects);

	{// compound-statement 
		for (SubRectArray::const_iterator it = subRects.begin();
			it != subRects.end();
			++it)
		{
			printf("Subrectangle %d (originally %d), size %dx%d, goes at pos %d,%d\n",
				it - subRects.begin(), it->n, it->w, it->h, it->x, it->y);
		}
	}

	{// compound-statement 
		for (LayoutArray::const_iterator it = layout.begin();
			it != layout.end();
			++it)
		{
			printf("  Square size %dx%d, Coverage %d / %d (%d%%)\n",
				it->GetW(), it->GetH(),
				it->GetArea(), it->GetTotalArea(), it->GetArea() * 100 / it->GetTotalArea());
		}
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	printf(" Total time to execute: %d  milliseconds", duration.count());
	std::getchar();
	return 0; 
}