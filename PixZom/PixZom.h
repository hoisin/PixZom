//---------------------------------------------------------------------------
//
//	Main sim class handler
//
//---------------------------------------------------------------------------
#pragma once

#include <SDL.h>

#include "ComponentDB.h"
#include "EntityMGR.h"
#include "PixLevelMap.h"
#include "EventHDLR.h"
#include "EntityGenerator.h"

#include "TransformSystem.h"
#include "MovementSystem.h"
#include "SpriteSystem.h"
#include "CollisionSystem.h"

class CPixZom : public CListener
{
public:
	CPixZom();
	~CPixZom();

	void HandleMessage(const CEventMessage& msg) override;

	bool Initialise();

	bool LoadMap();

	void UpdateCollisions(Uint32 elapsedTime);
	void Update(Uint32 elapsedTime);

	std::list<CSprite*> GetDrawList(int deltaT);

	Entity GetPlayerEntity() const;

private:
	CComponentDB* m_componentDB;
	CEntityMGR m_entityMGR;
	CEntityGenerator m_entityGenerator;
	bool m_Initialised;
	bool m_CollisionDetected;

	// Systems 
	CTransformSystem m_transformSystem;
	CMovementSystem m_movementSystem;
	CSpriteSystem m_spriteSystem;
	CCollisionSystem m_collisionSystem;

	// map area
	CPixLevelMap m_levelMap;

	Entity m_player;
};

