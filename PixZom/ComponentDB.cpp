#include "ComponentDB.h"

//-----------------------------------------------------------------------------------------------
//		ComponentMGR
//-----------------------------------------------------------------------------------------------

template<class CompType>
CComponentMGR<CompType>::CComponentMGR() : m_size(0)
{
}

template<class CompType>
CComponentMGR<CompType>::~CComponentMGR()
{
}

template<class CompType>
ComponentId CComponentMGR<CompType>::AddComponent(Entity entity, const CompType & compData)
{
	// Existing check
	ComponentId returnId = { -1 };
	auto existIt = m_entityMap.find(entity);
	if (existIt == m_entityMap.end())
	{
		// New entity, add a component for it
		returnId.id = m_size;
		m_components[returnId.id] = compData;
		m_entityMap[entity] = returnId;
		m_size++;
	}
	else
	{
		// Return existing entity's component id
		returnId = existIt->second;
	}

	return returnId;
}

template<class CompType>
void CComponentMGR<CompType>::DestoryComponent(Entity entity)
{
	auto findCompIt = m_entityMap.find(entity);
	if (findCompIt != m_entityMap.end())
	{
		int replaceId = findCompIt->second.id;
		if (replaceId == (m_size - 1))
		{
			m_components[replaceId] = m_components[m_size - 1];

			auto updateCompIt = m_entityMap.begin();
			while (updateCompIt != m_entityMap.end())
			{
				if (updateCompIt->second.id == (m_size - 1))
				{
					updateCompIt->second.id = replaceId;
					break;
				}
				updateCompIt++;
			}
		}
		m_entityMap.erase(entity);
		m_size--;
	}
}

template<class CompType>
ComponentId CComponentMGR<CompType>::FindComponentId(Entity entity) const
{
	ComponentId returnId = { -1 };
	auto findIt = m_entityMap.find(entity);
	if (findIt != m_entityMap.end())
		returnId = findIt->second;

	return returnId;
}

template<class CompType>
CompType * CComponentMGR<CompType>::AccessComponent(Entity entity)
{
	auto findCompIt = m_entityMap.find(entity);
	if (findCompIt != m_entityMap.end())
		return &m_components.at(findCompIt->second.id);

	return nullptr;
}


//-----------------------------------------------------------------------------------------------
//		ComponentDB
//-----------------------------------------------------------------------------------------------

CComponentDB* CComponentDB::__instance = nullptr;

CComponentDB::CComponentDB()
{
}


CComponentDB::~CComponentDB()
{
}


ComponentId CComponentDB::AddComponent(Entity entity, EComponentType type, const IComponent * compData)
{
	ComponentId returnId = { -1 };

	switch (type)
	{
	case COMPTYPE_TRANSFORM:
	{
		const TransformComponent* transform = static_cast<const TransformComponent*>(compData);
		returnId = m_transformComponents.AddComponent(entity, *transform);
		break;
	}
	case COMPTYPE_MOTION:
	{
		const MotionComponent* motion = static_cast<const MotionComponent*>(compData);
		returnId = m_motionComponents.AddComponent(entity, *motion);
		break;
	}
	case COMPTYPE_SPRITE:
	{
		const SpriteComponent* sprite = static_cast<const SpriteComponent*>(compData);
		returnId = m_spriteComponents.AddComponent(entity, *sprite);
		break;
	}
	case COMPTYPE_COLLISION:
	{
		const CollisionComponent* collision = static_cast<const CollisionComponent*>(compData);
		returnId = m_collisionComponents.AddComponent(entity, *collision);
		break;
	}
	default:
		break;
	}

	return returnId;
}


void CComponentDB::DestoryEntityComponents(Entity entity)
{
	m_transformComponents.DestoryComponent(entity);
	m_motionComponents.DestoryComponent(entity);
	m_spriteComponents.DestoryComponent(entity);
	m_collisionComponents.DestoryComponent(entity);
}

void CComponentDB::DestoryComponent(Entity entity, EComponentType type)
{
	switch (type)
	{
	case COMPTYPE_TRANSFORM:
		m_transformComponents.DestoryComponent(entity);
		break;
	case COMPTYPE_MOTION:
		m_motionComponents.DestoryComponent(entity);
		break;
	case COMPTYPE_SPRITE:
		m_spriteComponents.DestoryComponent(entity);
		break;
	case COMPTYPE_COLLISION:
		m_collisionComponents.DestoryComponent(entity);
		break;
	default:
		break;
	}
}


ComponentId CComponentDB::FindComponentId(Entity entity, EComponentType type) const
{
	ComponentId returnId = { -1 };
	switch (type)
	{
	case COMPTYPE_TRANSFORM:
		returnId = m_transformComponents.FindComponentId(entity);
		break;
	case COMPTYPE_MOTION:
		returnId = m_motionComponents.FindComponentId(entity);
		break;
	case COMPTYPE_SPRITE:
		returnId = m_spriteComponents.FindComponentId(entity);
		break;
	case COMPTYPE_COLLISION:
		returnId = m_collisionComponents.FindComponentId(entity);
		break;
	default:
		break;
	}

	return returnId;
}

IComponent * CComponentDB::AccessComponent(Entity entity, EComponentType type)
{
	IComponent* returnComp = nullptr;

	switch (type)
	{
	case COMPTYPE_TRANSFORM:
		returnComp = m_transformComponents.AccessComponent(entity);
		break;
	case COMPTYPE_MOTION:
		returnComp = m_motionComponents.AccessComponent(entity);
		break;
	case COMPTYPE_SPRITE:
		returnComp = m_spriteComponents.AccessComponent(entity);
		break;
	case COMPTYPE_COLLISION:
		returnComp = m_collisionComponents.AccessComponent(entity);
		break;
	default:
		break;
	}

	return returnComp;
}

CComponentDB * CComponentDB::GetInstance()
{
	if (__instance)
		return __instance;
	else
		__instance = new CComponentDB;


	return __instance;
}

void CComponentDB::Release()
{
	if (__instance)
	{
		delete __instance;
		__instance = nullptr;
	}
}

