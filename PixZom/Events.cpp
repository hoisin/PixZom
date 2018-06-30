#include "Events.h"

char const * const kpWildcardEventType = "*";

/*
------------------------------------------------------------------------------------------------
	CEventType
------------------------------------------------------------------------------------------------
*/

CEventType::CEventType(const char * const pIdentStr) : m_ident(HashName(pIdentStr)),
	m_identStr(pIdentStr)
{
}

unsigned long CEventType::GetIdent() const
{
	// return hased indentifier as an unsigned long integer
	// Need to cast to 64 integer in 64bit build
	return reinterpret_cast<unsigned long>(m_ident);
}

const std::string& CEventType::GetString() const
{
	return m_identStr;
}

bool CEventType::operator<(CEventType const & evt) const
{
	return (GetIdent() < evt.GetIdent());
}

bool CEventType::operator==(CEventType const & evt) const
{
	return (GetIdent() == evt.GetIdent());
}

void * CEventType::HashName(char const * pIdentStr)
{
	// Relatively simple hash of arbitrary text string into a
	// 32-bit identifier Output value is
	// input-valid-deterministic, but no guarantees are made
	// about the uniqueness of the output per-input
	//
	// Input value is treated as lower-case to cut down on false
	// separations cause by human mistypes. Sure, it could be
	// construed as a programming error to mix up your cases, and
	// it cuts down on permutations, but in Real World Usage
	// making this text case-sensitive will likely just lead to
	// Pain and Suffering.
	//
	// This code lossely based upon the adler32 checksum by Mark
	// Adler and published as part of the zlib compression
	// library sources.

	// largest prime smaller than 65536
	unsigned long BASE = 65521L;

	// NMAX is the largest n such that 255n(n+1)/2 +
	// (n+1)(BASE-1) <= 2^32-1
	unsigned long NMAX = 5552;

#define DO1(buf,i)  {s1 += tolower(buf[i]); s2 += s1;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

	if (pIdentStr == NULL)
		return NULL;

	if (strcmp(pIdentStr, kpWildcardEventType) == 0)
		return 0;

	unsigned long s1 = 0;
	unsigned long s2 = 0;

	for (size_t len = strlen(pIdentStr); len > 0; )
	{
		unsigned long k = len < NMAX ? len : NMAX;

		len -= k;

		while (k >= 16)
		{
			DO16(pIdentStr);
			pIdentStr += 16;
			k -= 16;
		}

		if (k != 0) do
		{
			s1 += *pIdentStr++;
			s2 += s1;
		} while (--k);

		s1 %= BASE;
		s2 %= BASE;
	}

#pragma warning(push)
#pragma warning(disable : 4312)

	return reinterpret_cast<void *>((s2 << 16) | s1);

#pragma warning(pop)
#undef DO1
#undef DO2
#undef DO4
#undef DO8
#undef DO16
}


/*
------------------------------------------------------------------------------------------------
CEventMessage
------------------------------------------------------------------------------------------------
*/

CEventMessage::CEventMessage()
{
}

CEventMessage::~CEventMessage()
{
}


/*
------------------------------------------------------------------------------------------------
CMoveEvent
------------------------------------------------------------------------------------------------
*/

const CEventType CMoveEntityEvent::m_type(event_move_entity.c_str());

CMoveEntityEvent::CMoveEntityEvent(EMoveDirection direction, Entity entity) :
	m_direction(direction), m_entity(entity)
{
}

CMoveEntityEvent::~CMoveEntityEvent()
{
}

const CEventType & CMoveEntityEvent::VGetEventType() const
{
	return m_type;
}

EMoveDirection CMoveEntityEvent::GetDirection() const
{
	return m_direction;
}

Entity CMoveEntityEvent::GetEntity() const
{
	return m_entity;
}

/*
------------------------------------------------------------------------------------------------
CSetEntitySpeed
------------------------------------------------------------------------------------------------
*/

const CEventType CSetEntitySpeed::m_type(event_set_entity_speed.c_str());

CSetEntitySpeed::CSetEntitySpeed(int velocity, int acceleration, Entity entity)
{
}

CSetEntitySpeed::~CSetEntitySpeed()
{
}

const CEventType & CSetEntitySpeed::VGetEventType() const
{
	return m_type;
}

int CSetEntitySpeed::GetVelocity() const
{
	return m_velocity;
}

int CSetEntitySpeed::GetAcceleration() const
{
	return m_acceleration;
}

Entity CSetEntitySpeed::GetEntity() const
{
	return m_entity;
}

/*
------------------------------------------------------------------------------------------------
CSetEntityPosEvent
------------------------------------------------------------------------------------------------
*/

const CEventType CSetEntityPosEvent::m_type(event_set_entity_pos.c_str());

CSetEntityPosEvent::CSetEntityPosEvent(int x, int y, Entity entity) :
	m_x(x), m_y(y), m_entity(entity)
{
}

CSetEntityPosEvent::~CSetEntityPosEvent()
{
}

const CEventType & CSetEntityPosEvent::VGetEventType() const
{
	return m_type;
}

int CSetEntityPosEvent::GetX() const
{
	return m_x;
}

int CSetEntityPosEvent::GetY() const
{
	return m_y;
}

Entity CSetEntityPosEvent::GetEntity() const
{
	return m_entity;
}

/*
------------------------------------------------------------------------------------------------
CCollisionEntityEvent
------------------------------------------------------------------------------------------------
*/

const CEventType CCollisionEntityEvent::m_type(event_entity_collision_entity.c_str());

CCollisionEntityEvent::CCollisionEntityEvent(Entity entityA, Entity entityB)
{
	m_entityA = entityA;
	m_entityB = entityB;
}

CCollisionEntityEvent::~CCollisionEntityEvent()
{
}

const CEventType & CCollisionEntityEvent::VGetEventType() const
{
	return m_type;
}

Entity CCollisionEntityEvent::GetEntityA() const
{
	return m_entityA;
}

Entity CCollisionEntityEvent::GetEntityB() const
{
	return m_entityB;
}

/*
------------------------------------------------------------------------------------------------
CSpawnEntityEvent
------------------------------------------------------------------------------------------------
*/

const CEventType CSpawnEntityEvent::m_type(event_spawn_entity.c_str());

CSpawnEntityEvent::CSpawnEntityEvent(const std::string & entityId, const glm::ivec2 & spawnPos)
{
	m_entityId = "";
	m_spawnPos = glm::ivec2(0, 0);
}

CSpawnEntityEvent::~CSpawnEntityEvent()
{
}

const CEventType & CSpawnEntityEvent::VGetEventType() const
{
	return m_type;
}

const std::string & CSpawnEntityEvent::GetEntityId() const
{
	return m_entityId;
}

const glm::ivec2 & CSpawnEntityEvent::GetSpawnPos() const
{
	return m_spawnPos;
}

/*
------------------------------------------------------------------------------------------------
CLoadMapEvent
------------------------------------------------------------------------------------------------
*/

const CEventType CLoadMapEvent::m_type(event_load_map.c_str());

CLoadMapEvent::CLoadMapEvent()
{
}

CLoadMapEvent::~CLoadMapEvent()
{
}

const CEventType & CLoadMapEvent::VGetEventType() const
{
	return m_type;
}

/*
------------------------------------------------------------------------------------------------
CGameStartEvent
------------------------------------------------------------------------------------------------
*/

const CEventType CGameStartEvent::m_type(event_load_map.c_str());

CGameStartEvent::CGameStartEvent()
{
}

CGameStartEvent::~CGameStartEvent()
{
}

const CEventType & CGameStartEvent::VGetEventType() const
{
	return m_type;
}

/*
------------------------------------------------------------------------------------------------
CInitialiseEvent
------------------------------------------------------------------------------------------------
*/

const CEventType CInitialiseEvent::m_type(event_initialise.c_str());

CInitialiseEvent::CInitialiseEvent()
{
}

CInitialiseEvent::~CInitialiseEvent()
{
}

const CEventType & CInitialiseEvent::VGetEventType() const
{
	return m_type;
}

/*
------------------------------------------------------------------------------------------------
CQuitEvent
------------------------------------------------------------------------------------------------
*/

const CEventType CQuitEvent::m_type(event_quit.c_str());

CQuitEvent::CQuitEvent()
{
}

CQuitEvent::~CQuitEvent()
{
}

const CEventType & CQuitEvent::VGetEventType() const
{
	return m_type;
}
