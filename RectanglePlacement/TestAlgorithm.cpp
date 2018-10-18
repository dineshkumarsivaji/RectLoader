/**

TestAlgorithm.cpp
Purpose: Invoke the RectLoader Algorithem and fill the square 
		 layout with Reatangles provided in inpur text file .

		 Input file have to be in the same folder as the source code

@author Dinesh Kumar Sivaji
@version 0.1 17/10/2018 Initial Version
*/


#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iterator>
#include "RectLoader.h";
using namespace std;

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

struct SubRect : public RectLoader::Square
{
	int n;      // Original index of this subrect, before sorting

	SubRect() { }
	SubRect(int _w, int _h, int _n) : Square(0, 0, _w, _h), n(_n) { }
};


// typedef for simplicity
typedef vector<SubRect> SubRectArray;
typedef vector<RectLoader> LayoutArray;


// --------------------------------------------------------------------------------
// Name        : splitString
// Description : Here we split the string with given delimiter
// @param	   : Rectangle dimentions in string and delimiter in char
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
// Description : Here we can get reactangels diminsitions from file,
//               and load the reactangles inside dynamic array (Vector)
// @param	   : a sub Rectangle array list
// --------------------------------------------------------------------------------
void loadRectangle(SubRectArray &subRects)
{
	subRects.clear();

	string item_name;
	ifstream infile;
	char data[100];

	infile.open("Rect.txt");
	int i = 0;
	while (infile >> item_name)
	{
		vector<string> subString = splitString(item_name, ',');
		int w = stoi (subString[0]);
		int h = stoi(subString[1]);
		subRects.push_back(SubRect(w, h, i));
		i++;
	}
	infile.close();
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
			printf("  Suqare %d, size %dx%d, Coverage %d / %d (%d%%)\n",
				it - layout.begin(), it->GetW(), it->GetH(),
				it->GetArea(), it->GetTotalArea(), it->GetArea() * 100 / it->GetTotalArea());
		}
	}
	return 0; 
}