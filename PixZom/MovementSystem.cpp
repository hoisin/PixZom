#include "MovementSystem.h"
#include "ComponentDB.h"
#include "PixLevelMap.h"

CMovementSystem::CMovementSystem() : m_map(nullptr)
{
}


CMovementSystem::~CMovementSystem()
{
}

bool CMovementSystem::RegisterEntity(Entity entity)
{
	// Expecting motion and transform components
	TransformComponent* transform = static_cast<TransformComponent*>(COMPDB->AccessComponent(entity, COMPTYPE_TRANSFORM));
	MotionComponent* motion = static_cast<MotionComponent*>(COMPDB->AccessComponent(entity, COMPTYPE_MOTION));

	if (transform != nullptr && motion != nullptr)
	{
		m_transformComps[entity] = transform;
		m_motionComps[entity] = motion;
		return true;
	}

	return false;
}

void CMovementSystem::HandleMessage(const CEventMessage& msg)
{
	if (msg.VGetEventType().GetString() == event_move_entity)
	{
		const CMoveEntityEvent* moveEvent = static_cast<const CMoveEntityEvent*>(&msg);
		TransformComponent* pTransform = static_cast<TransformComponent*>(COMPDB->AccessComponent(moveEvent->GetEntity(), COMPTYPE_TRANSFORM));
		MotionComponent* pMotion = static_cast<MotionComponent*>(COMPDB->AccessComponent(moveEvent->GetEntity(), COMPTYPE_MOTION));

		double degreesInRad45 = 0.785398;

		switch (moveEvent->GetDirection())
		{
		case MOVEDIR_UP:
			pTransform->prevPos = pTransform->pos;
			pTransform->pos.y -= pMotion->velocity;
			break;
		case MOVEDIR_DOWN:
			pTransform->prevPos = pTransform->pos;
			pTransform->pos.y += pMotion->velocity;
			break;
		case MOVEDIR_LEFT:
			pTransform->prevPos = pTransform->pos;
			pTransform->pos.x -= pMotion->velocity;
			break;
		case MOVEDIR_RIGHT:
			pTransform->prevPos = pTransform->pos;
			pTransform->pos.x += pMotion->velocity;
			break;
		case MOVEDIR_UP_LEFT:
			pTransform->prevPos = pTransform->pos;
			pTransform->pos.x -= (int)(pMotion->velocity * cos(degreesInRad45));
			pTransform->pos.y -= (int)(pMotion->velocity * sin(degreesInRad45));
			break;
		case MOVEDIR_UP_RIGHT:
			pTransform->prevPos = pTransform->pos;
			pTransform->pos.x += (int)(pMotion->velocity * cos(degreesInRad45));
			pTransform->pos.y -= (int)(pMotion->velocity * sin(degreesInRad45));
			break;
		case MOVEDIR_DOWN_LEFT:
			pTransform->prevPos = pTransform->pos;
			pTransform->pos.x -= (int)(pMotion->velocity * cos(degreesInRad45));
			pTransform->pos.y += (int)(pMotion->velocity * sin(degreesInRad45));
			break;
		case MOVEDIR_DOWN_RIGHT:
			pTransform->prevPos = pTransform->pos;
			pTransform->pos.x += (int)(pMotion->velocity * cos(degreesInRad45));
			pTransform->pos.y += (int)(pMotion->velocity * sin(degreesInRad45));
			break;
		default:
			break;
		}

		// Initial check would be simply the map limits check.
		// Perform tile checking later point
		CollisionComponent* pCollision = static_cast<CollisionComponent*>(COMPDB->AccessComponent(moveEvent->GetEntity(), COMPTYPE_COLLISION));
		CBox entityBox(0, 0, pCollision->width, pCollision->height);
		if (m_map->HasHitMapBoundary(pTransform->pos, entityBox))
		{
			pTransform->pos = pTransform->prevPos;
		}
	}

	if (msg.VGetEventType().GetString() == event_set_entity_speed)
	{
		const CSetEntitySpeed* setSpeedEvent = static_cast<const CSetEntitySpeed*>(&msg);
		MotionComponent* pMotion = static_cast<MotionComponent*>(COMPDB->AccessComponent(setSpeedEvent->GetEntity(), COMPTYPE_MOTION));
		pMotion->velocity = setSpeedEvent->GetVelocity();
		pMotion->acceleration = setSpeedEvent->GetAcceleration();
	}

	if (msg.VGetEventType().GetString() == event_entity_collision_entity)
	{
		const CCollisionEntityEvent* collisionEvent = dynamic_cast<const CCollisionEntityEvent*>(&msg);

		CollisionComponent* collisionA = static_cast<CollisionComponent*>(COMPDB->AccessComponent(collisionEvent->GetEntityA(), COMPTYPE_COLLISION));
		CollisionComponent* collisionB = static_cast<CollisionComponent*>(COMPDB->AccessComponent(collisionEvent->GetEntityB(), COMPTYPE_COLLISION));

		if (collisionA->blocking && collisionB->blocking)
		{
			TransformComponent* transformA = static_cast<TransformComponent*>(COMPDB->AccessComponent(collisionEvent->GetEntityA(), COMPTYPE_TRANSFORM));
			TransformComponent* transformB = static_cast<TransformComponent*>(COMPDB->AccessComponent(collisionEvent->GetEntityB(), COMPTYPE_TRANSFORM));

			transformA->pos = transformA->prevPos;
			transformB->pos = transformB->prevPos;

			// If both entities are blocking we need to ignore/abort movement messages
			// that may be in the current processing queue
		}
	}
}

void CMovementSystem::SetLevelMap(CPixLevelMap * map)
{
	m_map = map;
}
