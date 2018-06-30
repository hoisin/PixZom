//---------------------------------------------------------------------------
//
//	Component management
//
//---------------------------------------------------------------------------
#pragma once

#include <array>
#include <map>
#include "EntityMGR.h"
#include "Components.h"

struct ComponentId
{
	int id;

	bool operator <(const ComponentId& other)
	{
		return (id < other.id) ? true : false;
	}

	bool operator ==(const ComponentId& other)
	{
		return (id == other.id) ? true : false;
	}
};


const int MaxSizeCmpArray = 2048;

// Template component manager
template <class CompType>
class CComponentMGR
{
public:
	CComponentMGR();
	virtual ~CComponentMGR();

	ComponentId AddComponent(Entity entity, const CompType& compData);
	void DestoryComponent(Entity entity);

	ComponentId FindComponentId(Entity entity) const;
	CompType* AccessComponent(Entity entity);

private:
	int m_size;
	std::array<CompType, MaxSizeCmpArray> m_components;
	std::map<Entity, ComponentId> m_entityMap;
};


// Component database
class CComponentDB
{
public:
	CComponentDB();
	~CComponentDB();

	ComponentId AddComponent(Entity entity, EComponentType type, const IComponent* compData);

	void DestoryEntityComponents(Entity entity);
	void DestoryComponent(Entity entity, EComponentType type);

	ComponentId FindComponentId(Entity entity, EComponentType type) const;
	IComponent* AccessComponent(Entity entity, EComponentType type);

	static CComponentDB* GetInstance();
	void Release();

private:
	CComponentMGR<TransformComponent> m_transformComponents;
	CComponentMGR<MotionComponent> m_motionComponents;
	CComponentMGR<SpriteComponent> m_spriteComponents;
	CComponentMGR<CollisionComponent> m_collisionComponents;

private:
	static CComponentDB* __instance;
};

#define COMPDB CComponentDB::GetInstance()