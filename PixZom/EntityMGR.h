#pragma once

#include <list>

const int MaxEntitySize = 2048;

struct Entity
{
	int id;

	bool operator <(const Entity& other) const
	{
		return (id < other.id) ? true : false;
	}

	bool operator ==(const Entity& other) const
	{
		return (id == other.id) ? true : false;
	}
};

class CEntityMGR
{
public:
	CEntityMGR();
	~CEntityMGR();

	Entity CreateEntity();
	void DestoryEntity(Entity entity);

	const std::list<Entity>& GetEntityArray();

protected:
	int m_nextFreeIndex;
	std::list<Entity> m_activeEntities;
};

