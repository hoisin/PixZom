#include "EntityMGR.h"



CEntityMGR::CEntityMGR() : m_nextFreeIndex(0)
{
}


CEntityMGR::~CEntityMGR()
{
}

Entity CEntityMGR::CreateEntity()
{
	// If this overflows, we've managed to create over 2 billion entities....
	Entity returnEntity = { m_nextFreeIndex };
	m_activeEntities.push_back(returnEntity);
	m_nextFreeIndex++;
	return returnEntity;
}

void CEntityMGR::DestoryEntity(Entity entity)
{
	// Find the entity
	auto it = m_activeEntities.begin();
	while (it != m_activeEntities.end())
	{
		if ((*it) == entity)
		{
			m_activeEntities.erase(it);
			break;
		}
	}
}

const std::list<Entity>& CEntityMGR::GetEntityArray()
{
	return m_activeEntities;
}
