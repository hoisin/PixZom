#pragma once

#include "EntityMGR.h"
#include "Components.h"
#include "EventHDLR.h"
#include "Sprite.h"
#include "SpriteDispenser.h"
#include "CTextureMGR.h"

#include <memory>
#include <list>

class CSpriteSystem : public CListener
{
public:
	CSpriteSystem();
	~CSpriteSystem();

	bool RegisterEntity(Entity entity);

	void HandleMessage(const CEventMessage& msg) override;

	std::list<CSprite*> GetDrawList(int deltaT);

private:
	int m_camWorldX;
	int m_camWorldY;

	std::map<Entity, TransformComponent*> m_transforms;
	std::map<Entity, SpriteComponent*> m_sprites;

	CSpriteDispenser m_spriteDispenser;
};

