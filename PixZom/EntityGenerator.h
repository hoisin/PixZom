#pragma once

#include <string>
#include "EntityMGR.h"
#include "tinyxml2.h"
#include "Components.h"

class CEntityGenerator
{
public:
	CEntityGenerator();
	~CEntityGenerator();

	void SetEntityMGR(CEntityMGR* pMGR);

	Entity GenerateEntity(const std::string& entityId);

private:
	bool CreateEntity(Entity entity, tinyxml2::XMLElement* node);
	EComponentType GetCompType(const std::string& component);

	bool CreateTransformComp(Entity entity, tinyxml2::XMLElement* node);
	bool CreateMotionComp(Entity entity, tinyxml2::XMLElement* node);
	bool CreateSpriteComp(Entity entity, tinyxml2::XMLElement* node);
	bool CreateCollisionComp(Entity entity, tinyxml2::XMLElement* node);

private:
	CEntityMGR* m_entityMGR;
};

