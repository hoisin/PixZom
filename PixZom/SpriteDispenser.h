//---------------------------------------------------------------------------
//
//	Sprite object MGR.
//  Avoids dynamic allocation
//
//---------------------------------------------------------------------------
#pragma once

#include <array>
#include "Sprite.h"

const int MaxSpriteArray = 4096;

class CSpriteDispenser
{
public:
	CSpriteDispenser();
	~CSpriteDispenser();

	int GetSprite();
	CSprite* AccessSprite(int index);

	void ClearSprites();

private:
	int m_size;
	std::array<CSprite, MaxSpriteArray> m_sprites;
};

