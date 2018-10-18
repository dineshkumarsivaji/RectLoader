/**

RectLoader.h
Purpose: Header File.

@author Dinesh Kumar Sivaji
@version 0.1 17/10/2018 Initial Version
*/

#pragma once

#include <vector>
using namespace std;


class RectLoader
{
public:

	struct LayoutPos
	{
		int x, y;

		LayoutPos() { } // Default Constructor 
		LayoutPos(int _x, int _y) : x(_x), y(_y) { }

		bool operator ==(const LayoutPos &pos) const { return x == pos.x && y == pos.y; }
	};

	struct Square : public LayoutPos
	{
		int w, h;  // Width and Height of the Square

		Square() { }  // Default Constructor 
		Square(int _x, int _y, int _w, int _h) : LayoutPos(_x, _y), w(_w > 0 ? _w : 0), h(_h > 0 ? _h : 0) { }

		bool Contains(const LayoutPos &p)  const { return (p.x >= x && p.y >= y && p.x < (x + w) && p.y < (y + h)); }
		bool Contains(const Square &r) const { return (r.x >= x && r.y >= y && (r.x + r.w) <= (x + w) && (r.y + r.h) <= (y + h)); }
		bool Intersects(const Square &r) const { return w > 0 && h > 0 && r.w > 0 && r.h > 0 && ((r.x + r.w) > x && r.x < (x + w) && (r.y + r.h) > y && r.y < (y + h)); }

		static bool Greater(const Square &a, const Square &b) { return (a.w > b.w && a.w > b.h) || (a.h > b.w && a.h > b.h); }  // Greater size in at least one dim.
	};


	RectLoader(const string& filePath);

	vector<Square> load();

	typedef vector<LayoutPos>  PosArrayList;
	typedef vector<Square> RectArrayList;

	RectLoader() { Init(); }
	~RectLoader() { End(); }

	void Init(int w = 1, int h = 1);
	void End();
	bool IsOk() const { return m_size.w > 0; }

	int GetW() const { return m_size.w; }
	int GetH() const { return m_size.h; }
	long GetArea() const { return m_area; }
	long GetTotalArea() const { return m_size.w*m_size.h; }

	bool AddAtEmptySpotAutoGrow(Square *pSquare);


private:
	Square m_size;
	RectArrayList m_vRects;
	PosArrayList m_vPositions;
	long m_area;


	bool IsFree(const Square &r) const;
	void AddPosition(const LayoutPos &p);
	void AddRect(const Square &r);
	bool AddAtEmptySpot(Square &r);

};
