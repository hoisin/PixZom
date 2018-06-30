#include "CollisionSystem.h"
#include "ComponentDB.h"
#include "Events.h"
#include "Box.h"
#include "EventHDLR.h"

CCollisionSystem::CCollisionSystem()
{
}


CCollisionSystem::~CCollisionSystem()
{
}

bool CCollisionSystem::RegisterEntity(Entity entity)
{
	auto transformIt = m_transformComps.find(entity);
	auto collisionIt = m_collisionComps.find(entity);

	// Entity already exist!
	if (transformIt != m_transformComps.end()
		&& collisionIt != m_collisionComps.end())
		return false;

	TransformComponent* transform = static_cast<TransformComponent*>(COMPDB->AccessComponent(entity, COMPTYPE_TRANSFORM));
	CollisionComponent* collision = static_cast<CollisionComponent*>(COMPDB->AccessComponent(entity, COMPTYPE_COLLISION));

	if (transform && collision)
	{
		m_transformComps[entity] = transform;
		m_collisionComps[entity] = collision;
	}
	else
	{
		// Failed to find components for entity
		return false;
	}

	return true;
}

bool CCollisionSystem::CheckCollisions()
{
	bool collision = false;
	auto currentTransformIt = m_transformComps.begin();
	auto currentCollisionIt = m_collisionComps.begin();

	auto compareTransformIt = currentTransformIt;
	auto compareCollisionIt = currentCollisionIt;

	// We compare every entity with each other.
	// Every entity is only compared against each other once
	while (currentTransformIt != m_transformComps.end())
	{
		// Increment compare iterators to next items
		compareTransformIt++;
		compareCollisionIt++;

		// If entity to compare hasn't reached the end
		while (compareTransformIt != m_transformComps.end())
		{
			// Perform box collision check
			int entityWidthA = currentCollisionIt->second->width;
			int entityHeightA = currentCollisionIt->second->height;
			int entityWidthB = compareCollisionIt->second->width;
			int entityHeightB = compareCollisionIt->second->height;
			CBox entityA(currentTransformIt->second->pos.x - (entityWidthA / 2),
				currentTransformIt->second->pos.y - (entityHeightA / 2),
				(entityWidthA / 2) + currentTransformIt->second->pos.x,
				(entityHeightA / 2) + currentTransformIt->second->pos.y);
			CBox entityB(compareTransformIt->second->pos.x - (entityWidthB / 2),
				compareTransformIt->second->pos.y - (entityHeightB / 2),
				(entityWidthB / 2) + compareTransformIt->second->pos.x,
				(entityHeightB / 2) + compareTransformIt->second->pos.y);
			// If collided
 			if (entityA.Intersect(entityB))
			{
				// Send event out
				CCollisionEntityEvent* evt = new CCollisionEntityEvent(currentCollisionIt->first, compareCollisionIt->first);
				EVENTSYS->ActionEvent(evt);
				collision = true;
			}

			// Increment next item to check against
			compareTransformIt++;
			compareCollisionIt++;
		}

		// Increment 'current' entity
		currentTransformIt++;
		currentCollisionIt++;

		// Make compare entity equal to current
		compareTransformIt = currentTransformIt;
		compareCollisionIt = currentCollisionIt;
	}

	return collision;
}

void CCollisionSystem::HandleMessage(const CEventMessage & msg)
{
}
