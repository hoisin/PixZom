//---------------------------------------------------------------------------
//
//	Components
//
//---------------------------------------------------------------------------
#pragma once

#include <glm.hpp>

enum EComponentType
{
	COMPTYPE_UNKNOWN,
	COMPTYPE_TRANSFORM,
	COMPTYPE_MOTION,
	COMPTYPE_SPRITE,
	COMPTYPE_COLLISION,
	COMPTYPE_TOTAL
};


// Base component
struct IComponent
{
	virtual ~IComponent() {}
};


struct TransformComponent : public IComponent
{
	glm::ivec2 pos;
	glm::ivec2 prevPos;

	TransformComponent() : pos(glm::ivec2(0, 0)), prevPos(glm::ivec2(0,0))
	{
	}
};


struct MotionComponent : public IComponent
{
	int velocity;
	int acceleration;

	MotionComponent() : velocity(0), acceleration(0)
	{
	}
};


struct SpriteComponent : public IComponent
{
	std::string spriteId;	// This is actually a handle to texture ID in XML

	SpriteComponent() : spriteId("")
	{
	}
};


struct CollisionComponent : public IComponent
{
	int width;
	int height;

	bool blocking;

	CollisionComponent() : width(0), height(0), blocking(false)
	{
	}
};