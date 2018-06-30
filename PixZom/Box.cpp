#include "Box.h"



CBox::CBox(int left, int top, int right, int bottom) : m_left(left),
	m_top(top), m_right(right), m_bottom(bottom)
{
}
 
CBox::CBox() : m_left(0), m_top(0), m_right(0), m_bottom(0)
{
}


CBox::~CBox()
{
}

bool CBox::Intersect(const CBox & other) const
{
	if (m_left > other.m_right || m_right < other.m_left ||
		m_top > other.m_bottom || m_bottom < other.m_top)
		return false;

	return true;
}

bool CBox::IsContained(const CBox & other) const
{
	if (Intersect(other))
	{
		int myArea = GetWidth() * GetHeight();
		int otherArea = other.GetWidth() * other.GetHeight();

		if (myArea == otherArea)
		{
			if (m_left == other.m_left && m_right == other.m_right &&
				m_top == other.m_top && m_bottom == other.m_bottom)
				return true;
		}
		else
		{
			if (myArea > otherArea)
			{
				if (other.m_left >= m_left && other.m_right <= m_right &&
					other.m_top >= m_top && other.m_bottom <= m_bottom)
					return true;
			}
			else
			{
				if (m_left >= other.m_left && m_right <= other.m_right &&
					m_top >= other.m_top && m_bottom <= other.m_bottom)
					return true;
			}
		}
	}

	return false;
}

int CBox::GetWidth() const
{
	return m_right - m_left;
}

int CBox::GetHeight() const
{
	return m_bottom - m_top;
}
