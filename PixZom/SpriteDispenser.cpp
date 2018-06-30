#include "SpriteDispenser.h"

CSpriteDispenser::CSpriteDispenser() : m_size(0)
{
}

CSpriteDispenser::~CSpriteDispenser()
{
}

int CSpriteDispenser::GetSprite()
{
	int id = m_size;
	m_size++;
	return id;
}

CSprite * CSpriteDispenser::AccessSprite(int index)
{
	return &m_sprites.at(index);
}

void CSpriteDispenser::ClearSprites()
{
	m_size = 0;
}
