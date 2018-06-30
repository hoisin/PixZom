//---------------------------------------------------------------------------
//
//	Sprite draw object
//
//---------------------------------------------------------------------------
#pragma once

#include <glm.hpp>
#include "gfx_defines.h"

class CSprite
{
public:
	CSprite();
	~CSprite();

	void SetTexture(STexture* pTexture);
	STexture* GetTexture() const;

	void SetPos(const glm::ivec2& pos);
	const glm::ivec2 GetPos() const;

	void SetOrder(int order);
	int GetOrder() const;

	void SetCurrentFrame(int frame);
	void SetFrameRow(int row);
	int GetCurrentFrame() const;
	int GetFrameRow() const;
	
private:
	STexture * m_pTexture;

	// Screen position
	glm::ivec2 m_pos;
	int m_order;
	int m_currentFrame;
	int m_frameRow;
};

