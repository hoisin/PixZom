/*
	Version: 1.0
	Description: Class that hashes a string and return unique identifier, for defining a event type
*/
#pragma once

#include <string>
#include <glm.hpp>
#include "EntityMGR.h"

// String defines for event types
const std::string event_move_entity = "event_move_entity";
const std::string event_set_entity_pos = "event_set_entity_pos";
const std::string event_set_entity_speed = "event_set_entity_speed";
const std::string event_entity_collision_entity = "event_entity_collision_entity";
const std::string event_spawn_entity = "event_spawn_entity";
const std::string event_load_map = "event_load_map";
const std::string event_game_start = "event_game_start";
const std::string event_initialise = "event_initialise";
const std::string event_quit = "event_quit";

class CEventType
{
public:
	explicit CEventType(const char * const pIndentStr);

	unsigned long GetIdent(void) const;
	const std::string& GetString(void) const;
	bool operator < (CEventType const &evt) const;
	bool operator == (CEventType const &evt) const;
	static void * HashName(char const * pIndetStr);

private:
	void *m_ident;				//The hash value, using void* so that debugger will show up as hex values
	std::string m_identStr;		//The string identifing this event
};


class CEventMessage
{
public:
	CEventMessage();
	virtual ~CEventMessage();

	virtual const CEventType& VGetEventType() const = 0;
};


/*
------------------------------------------------------------------------------------------------
CMoveEvent
------------------------------------------------------------------------------------------------
*/

enum EMoveDirection {
	MOVEDIR_UP,
	MOVEDIR_DOWN,
	MOVEDIR_LEFT,
	MOVEDIR_RIGHT,
	MOVEDIR_UP_LEFT,
	MOVEDIR_UP_RIGHT,
	MOVEDIR_DOWN_LEFT,
	MOVEDIR_DOWN_RIGHT,
	MOVEDIR_UNKNOWN,
	MOVEDIR_TOTAL
};

class CMoveEntityEvent : public CEventMessage
{
public:
	explicit CMoveEntityEvent(EMoveDirection direction, Entity entity);
	~CMoveEntityEvent();

	const CEventType& VGetEventType() const override;

	EMoveDirection GetDirection() const;
	Entity GetEntity() const;

public:
	static const CEventType m_type;

private:
	EMoveDirection m_direction;
	Entity m_entity;
};

/*
------------------------------------------------------------------------------------------------
CSetEntitySpeed
------------------------------------------------------------------------------------------------
*/
class CSetEntitySpeed : public CEventMessage
{
public:
	explicit CSetEntitySpeed(int velocity, int acceleration, Entity entity);
	~CSetEntitySpeed();

	const CEventType& VGetEventType() const override;

	int GetVelocity() const;
	int GetAcceleration() const;
	Entity GetEntity() const;

public:
	static const CEventType m_type;

private:
	int m_velocity;
	int m_acceleration;
	Entity m_entity;
};


/*
------------------------------------------------------------------------------------------------
CSetEntityPosEvent
------------------------------------------------------------------------------------------------
*/

class CSetEntityPosEvent : public CEventMessage
{
public:
	explicit CSetEntityPosEvent(int x, int y, Entity entity);
	~CSetEntityPosEvent();

	const CEventType& VGetEventType() const override;

	int GetX() const;
	int GetY() const;
	Entity GetEntity() const;

public:
	static const CEventType m_type;

private:
	int m_x;
	int m_y;
	Entity m_entity;
};

/*
------------------------------------------------------------------------------------------------
CEntityCollisionEvent
------------------------------------------------------------------------------------------------
*/

class CCollisionEntityEvent : public CEventMessage
{
public:
	explicit CCollisionEntityEvent(Entity entityA, Entity entityB);
	~CCollisionEntityEvent();

	const CEventType& VGetEventType() const override;

	Entity GetEntityA() const;
	Entity GetEntityB() const;

public:
	static const CEventType m_type;

private:
	Entity m_entityA;
	Entity m_entityB;
};

/*
------------------------------------------------------------------------------------------------
CSpawnEntityEvent
------------------------------------------------------------------------------------------------
*/

class CSpawnEntityEvent : public CEventMessage
{
public: 
	explicit CSpawnEntityEvent(const std::string& entityId, const glm::ivec2& spawnPos);
	~CSpawnEntityEvent();

	const CEventType& VGetEventType() const override;

	const std::string& GetEntityId() const;
	const glm::ivec2& GetSpawnPos() const;

public:
	static const CEventType m_type;

private:
	std::string m_entityId;	// Entity to spawn
	glm::ivec2 m_spawnPos;
};


/*
------------------------------------------------------------------------------------------------
CLoadMapEvent
------------------------------------------------------------------------------------------------
*/

class CLoadMapEvent : public CEventMessage
{
public:
	explicit CLoadMapEvent();
	~CLoadMapEvent();

	const CEventType& VGetEventType() const override;

public:
	static const CEventType m_type;
};

/*
------------------------------------------------------------------------------------------------
CGameStartEvent
------------------------------------------------------------------------------------------------
*/

class CGameStartEvent : public CEventMessage
{
public:
	explicit CGameStartEvent();
	~CGameStartEvent();

	const CEventType& VGetEventType() const override;

public:
	static const CEventType m_type;
};


/*
------------------------------------------------------------------------------------------------
CInitialiseEvent
------------------------------------------------------------------------------------------------
*/

class CInitialiseEvent : public CEventMessage
{
public:
	explicit CInitialiseEvent();
	~CInitialiseEvent();

	const CEventType& VGetEventType() const override;

public:
	static const CEventType m_type;
};

/*
------------------------------------------------------------------------------------------------
CQuitEvent
------------------------------------------------------------------------------------------------
*/

class CQuitEvent : public CEventMessage
{
public:
	explicit CQuitEvent();
	~CQuitEvent();

	const CEventType& VGetEventType() const override;

public:
	static const CEventType m_type;
};