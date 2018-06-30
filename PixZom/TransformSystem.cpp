#include "TransformSystem.h"
#include "ComponentDB.h"
#include "PixLevelMap.h"

CTransformSystem::CTransformSystem() : m_map(nullptr)
{
}


CTransformSystem::~CTransformSystem()
{
}

bool CTransformSystem::RegisterEntity(Entity entity)
{
	TransformComponent* transform = static_cast<TransformComponent*>(COMPDB->AccessComponent(entity, COMPTYPE_TRANSFORM));
	m_transformComps[entity] = transform;
	return false;
}

void CTransformSystem::HandleMessage(const CEventMessage & msg)
{
	if (msg.VGetEventType().GetString() == event_set_entity_pos)
	{
		const CSetEntityPosEvent* setPosEvent = static_cast<const CSetEntityPosEvent*>(&msg);

		TransformComponent* transform = static_cast<TransformComponent*>(COMPDB->AccessComponent(setPosEvent->GetEntity(), COMPTYPE_TRANSFORM));

		transform->pos.x = setPosEvent->GetX();
		transform->pos.y = setPosEvent->GetY();
		transform->prevPos.x = setPosEvent->GetX();
		transform->prevPos.y = setPosEvent->GetY();
	}

	if (msg.VGetEventType().GetString() == event_entity_collision_entity)
	{
		const CCollisionEntityEvent* collisionEvent = static_cast<const CCollisionEntityEvent*>(&msg);

		TransformComponent* transformA = static_cast<TransformComponent*>(COMPDB->AccessComponent(collisionEvent->GetEntityA(), COMPTYPE_TRANSFORM));
		TransformComponent* transformB = static_cast<TransformComponent*>(COMPDB->AccessComponent(collisionEvent->GetEntityB(), COMPTYPE_TRANSFORM));

		CollisionComponent* collisionA = static_cast<CollisionComponent*>(COMPDB->AccessComponent(collisionEvent->GetEntityA(), COMPTYPE_COLLISION));
		CollisionComponent* collisionB = static_cast<CollisionComponent*>(COMPDB->AccessComponent(collisionEvent->GetEntityB(), COMPTYPE_COLLISION));

		// Apply collision correction if both entities are blocking
		if (collisionA->blocking && collisionB->blocking)
		{
			transformA->pos.x = transformA->prevPos.x;
			transformA->pos.y = transformA->prevPos.y;

			transformB->pos.x = transformA->prevPos.x;
			transformB->pos.y = transformA->prevPos.y;
		}
	}
}

void CTransformSystem::SetLevelMap(CPixLevelMap * map)
{
	m_map = map;
}

void CTransformSystem::Update()
{
	auto it = m_transformComps.begin();

	while (it != m_transformComps.end())
	{
		(*it).second->prevPos = (*it).second->pos;
		it++;
	}
}
