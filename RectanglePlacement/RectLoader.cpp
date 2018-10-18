#include "RectLoader.h"

// --------------------------------------------------------------------------------
// Name        : Init
// Description : Constructor Function call
// @param	   : Rectangle width and height
// --------------------------------------------------------------------------------
void RectLoader::Init(int w, int h)
{
	End();
	m_size = Square(0, 0, w, h);
	m_vPositions.push_back(LayoutPos(0, 0));
	m_area = 0;
}

// --------------------------------------------------------------------------------
// Name        : End
// Description : Destructor Function call
// --------------------------------------------------------------------------------
void RectLoader::End()
{
	m_vPositions.clear();
	m_vRects.clear();
	m_size.w = 0;
}


// --------------------------------------------------------------------------------
// Name        : IsFree
// Description : Check if the given Square is partially or totally used
// @param	   : a Square Layout
// @return	   : a boolean confirmation
// --------------------------------------------------------------------------------
bool RectLoader::IsFree(const Square &r) const
{
	if (!m_size.Contains(r))
		return false;
	for (RectArrayList::const_iterator it = m_vRects.begin();
		it != m_vRects.end();	
		++it)
		if (it->Intersects(r))
			return false;
	return true;
}

// --------------------------------------------------------------------------------
// Name        : AddPosition
// Description : Add new anchor point
// @param	   : a new layout position of the rectangle
// @return	   : a boolean confirmation
// --------------------------------------------------------------------------------
void RectLoader::AddPosition(const LayoutPos &p)
{
	// Try to insert anchor as close as possible to the top left corner
	bool bFound = false;
	PosArrayList::iterator it;
	for (it = m_vPositions.begin();
		!bFound && it != m_vPositions.end();
		++it)
	{
		if (p.x + p.y < it->x + it->y)
			bFound = true;
	}
	if (bFound)
		m_vPositions.insert(it, p);
	else
		m_vPositions.push_back(p);
}


// --------------------------------------------------------------------------------
// Name        : AddRect
// Description : Add the given rect and updates anchor points
// @param	   : a Square Layout
// @return	   : a boolean confirmation
// --------------------------------------------------------------------------------
void RectLoader::AddRect(const Square &r)
{
	m_vRects.push_back(r);
	m_area += r.w*r.h;

	// Add two new anchor points
	AddPosition(LayoutPos(r.x, r.y + r.h));
	AddPosition(LayoutPos(r.x + r.w, r.y));
}

// --------------------------------------------------------------------------------
// Name        : AddAtEmptySpot
// Description : Add the given rectangle
// @param	   : a Square Layout
// @return	   : a boolean confirmation
// --------------------------------------------------------------------------------
bool RectLoader::AddAtEmptySpot(Square &r)
{
	// Find a valid spot among available anchors.
	bool bFound = false;
	PosArrayList::iterator it;
	for (it = m_vPositions.begin();
		!bFound && it != m_vPositions.end();
		++it)
	{
		Square Rect(it->x, it->y, r.w, r.h);

		if (IsFree(Rect))
		{
			r = Rect;
			bFound = true;
			break; // Don't let the loop increase the iterator.
		}
	}
	if (bFound)
	{
		// Remove the used anchor point
		m_vPositions.erase(it);

		int x = 0, y = 0;
		for (int x = 1; x <= r.x; x++)
			if (!IsFree(Square(r.x - x, r.y, r.w, r.h)))
				break;
		for (int y = 1; y <= r.y; y++)
			if (!IsFree(Square(r.x, r.y - y, r.w, r.h)))
				break;
		if (y > x)
			r.y -= y - 1;
		else
			r.x -= x - 1;
		AddRect(r);
	}
	return bFound;
}

// --------------------------------------------------------------------------------
// Name        : AddAtEmptySpotAutoGrow
// Description : Add a rectangle of the given size, growing our area if needed
// @param	   : a Square Layout
// @return	   : a boolean confirmation
// --------------------------------------------------------------------------------
bool RectLoader::AddAtEmptySpotAutoGrow(Square *pRect)
{
	if (pRect->w <= 0)
		return true;

	int orgW = m_size.w;
	int orgH = m_size.h;

	// Try to add it in the existing space
	while (!AddAtEmptySpot(*pRect))
	{
		int pw = m_size.w;
		int ph = m_size.h;
		// Grow both and reloop.
			m_size.w = pw * 2;
			m_size.h = ph * 2;

		if (pw != m_size.w || ph != m_size.h)
			if (AddAtEmptySpot(*pRect))
				break;

	}
	return true;
}
