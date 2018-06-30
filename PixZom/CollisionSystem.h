#pragma once

#include <map>
#include "EventHDLR.h"
#include "EntityMGR.h"
#include "Components.h"

class CCollisionSystem : public CListener
{
public:
	CCollisionSystem();
	~CCollisionSystem();

	bool RegisterEntity(Entity entity);

	bool CheckCollisions();

	void HandleMessage(const CEventMessage& msg) override;

protected:
	// Transform and collision components
	// Store pointers for direct access
	std::map<Entity, TransformComponent*> m_transformComps;
	std::map<Entity, CollisionComponent*> m_collisionComps;
};

