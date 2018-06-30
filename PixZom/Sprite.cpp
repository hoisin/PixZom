#include "Sprite.h"


CSprite::CSprite() : m_pTexture(nullptr), m_pos(glm::ivec2(0,0)), m_order(1),
m_currentFrame(1), m_frameRow(1)
{
}


CSprite::~CSprite()
{
}

void CSprite::SetTexture(STexture * pTexture)
{
	m_pTexture = pTexture;
}

STexture * CSprite::GetTexture() const
{
	return m_pTexture;
}

void CSprite::SetPos(const glm::ivec2 & pos)
{
	m_pos = pos;
}

const glm::ivec2 CSprite::GetPos() const
{
	return m_pos;
}


void CSprite::SetOrder(int order)
{
	m_order = order;
}

int CSprite::GetOrder() const
{
	return m_order;
}

void CSprite::SetCurrentFrame(int frame)
{
	m_currentFrame = frame;
}

void CSprite::SetFrameRow(int row)
{
	m_frameRow = row;
}

int CSprite::GetCurrentFrame() const
{
	return m_currentFrame;
}

int CSprite::GetFrameRow() const
{
	return m_frameRow;
}
