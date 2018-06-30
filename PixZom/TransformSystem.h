#pragma once
#include <map>

#include "EventHDLR.h"
#include "EntityMGR.h"
#include "Components.h"
#include "EventHDLR.h"

class CPixLevelMap;

class CTransformSystem : public CListener
{
public:
	CTransformSystem();
	virtual ~CTransformSystem();

	bool RegisterEntity(Entity entity);
	void HandleMessage(const CEventMessage& msg) override;

	void SetLevelMap(CPixLevelMap* map);

	void Update();

protected:
	std::map<Entity, TransformComponent*> m_transformComps;

	CPixLevelMap* m_map;
};

