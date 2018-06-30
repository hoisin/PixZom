#pragma once

#include <map>
#include "EventHDLR.h"
#include "EntityMGR.h"
#include "Components.h"

class CPixLevelMap;

class CMovementSystem : public CListener
{
public:
	CMovementSystem();
	~CMovementSystem();

	bool RegisterEntity(Entity entity);
	void HandleMessage(const CEventMessage& msg) override;

	void SetLevelMap(CPixLevelMap* map);

private:
	std::map<Entity, TransformComponent*> m_transformComps;
	std::map<Entity, MotionComponent*> m_motionComps;

	CPixLevelMap* m_map;
};

