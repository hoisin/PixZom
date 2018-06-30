#include "SpriteSystem.h"
#include "ComponentDB.h"
#include "ResourceMGR.h"

CSpriteSystem::CSpriteSystem()
{
	m_camWorldX = 0;
	m_camWorldY = 0;
}


CSpriteSystem::~CSpriteSystem()
{
}

bool CSpriteSystem::RegisterEntity(Entity entity)
{
	auto checkIt = m_sprites.find(entity);
	if (checkIt != m_sprites.end())
		return false;

	SpriteComponent* spriteCmp = static_cast<SpriteComponent*>(COMPDB->AccessComponent(entity, COMPTYPE_SPRITE));
	TransformComponent* transformCmp = static_cast<TransformComponent*>(COMPDB->AccessComponent(entity, COMPTYPE_TRANSFORM));

	if (!spriteCmp || !transformCmp)
		return false;

	m_sprites[entity] = spriteCmp;
	m_transforms[entity] = transformCmp;
	return true;
}

void CSpriteSystem::HandleMessage(const CEventMessage & msg)
{
}

std::list<CSprite*> CSpriteSystem::GetDrawList(int deltaT)
{
	std::list<CSprite*> drawList;

	auto transformIt = m_transforms.begin();
	// Interp value = deltaT / frameUpdateTime;
	float interp = (float)((float)deltaT / 50.0);
	if (interp > 1)
		interp = 1;

	// Clear in use sprites in dispenser
	m_spriteDispenser.ClearSprites();

	// Build the sprite list
	while (transformIt != m_transforms.end())
	{
		auto spriteIt = m_sprites.find(transformIt->first);

		// Interpolate between previous and current position
		int x = (int)(transformIt->second->prevPos.x + ((transformIt->second->pos.x - transformIt->second->prevPos.x) * interp));
		int y = (int)(transformIt->second->prevPos.y + ((transformIt->second->pos.y - transformIt->second->prevPos.y) * interp));
		// Calculate screen position
		x -= m_camWorldX;
		y -= m_camWorldY;
		// Grab a sprite and fill in details
		int newSpriteIdx = m_spriteDispenser.GetSprite();
		CSprite* newSprite = m_spriteDispenser.AccessSprite(newSpriteIdx);
		STexture* pTexture = RESOURCEMGR->GetTextureResource(spriteIt->second->spriteId, true);
		newSprite->SetTexture(pTexture);

		newSprite->SetPos(glm::ivec2(x - (pTexture->frameWidth/2), y - (pTexture->frameHeight / 2)));
		newSprite->SetOrder(1);
		newSprite->SetCurrentFrame(1);
		newSprite->SetFrameRow(1);
		// Add to draw list
		drawList.push_back(newSprite);
		// Next entity pls
		transformIt++;
	}

	return drawList;
}
