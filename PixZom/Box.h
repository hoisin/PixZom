#pragma once

class CBox
{
public:
	CBox(int left, int top, int right, int bottom);
	CBox();
	~CBox();

	bool Intersect(const CBox& other) const;
	bool IsContained(const CBox& other) const;

	int GetWidth() const;
	int GetHeight() const;

public:
	int m_left;
	int m_top;
	int m_right;
	int m_bottom;
};

