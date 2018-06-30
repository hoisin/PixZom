#include "EntityGenerator.h"
#include "ComponentDB.h"

CEntityGenerator::CEntityGenerator() : m_entityMGR(nullptr)
{
}


CEntityGenerator::~CEntityGenerator()
{
}

void CEntityGenerator::SetEntityMGR(CEntityMGR * pMGR)
{
	m_entityMGR = pMGR;
}

Entity CEntityGenerator::GenerateEntity(const std::string & entityId)
{
	Entity newEntity = { -1 };

	if (m_entityMGR)
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError xmlError = doc.LoadFile("..\\Assets\\Data\\PixZomEntityData.xml");
		if (xmlError == tinyxml2::XML_SUCCESS) {
			tinyxml2::XMLElement* entities = doc.RootElement()->FirstChildElement("Entities");

			if (entities)
			{
				tinyxml2::XMLElement* entityNode = entities->FirstChildElement("Entity");
				if (entityNode)
				{
					while (entityNode)
					{
						const char* getEntityId = entityNode->Attribute("id");
						std::string getEntityStr(getEntityId);

						if (getEntityStr == entityId)
						{
							newEntity = m_entityMGR->CreateEntity();
							if (!CreateEntity(newEntity, entityNode))
							{
								m_entityMGR->DestoryEntity(newEntity);
								COMPDB->DestoryEntityComponents(newEntity);
								newEntity.id = -1;
							}
							break;
						}
						entityNode = entityNode->NextSiblingElement();
					}
				}
			}
		}
	}
	return newEntity;
}

bool CEntityGenerator::CreateEntity(Entity entity, tinyxml2::XMLElement * node)
{
	// node parameter should be "entity"
	// Go through all components
	tinyxml2::XMLElement* component = node->FirstChildElement();

	bool result = true;

	while (component)
	{
		switch (GetCompType(component->Name()))
		{
		case COMPTYPE_TRANSFORM:
			result = CreateTransformComp(entity, component);
			break;
		case COMPTYPE_MOTION:
			result = CreateMotionComp(entity, component);
			break;
		case COMPTYPE_SPRITE:
			result = CreateSpriteComp(entity, component);
			break;
		case COMPTYPE_COLLISION:
			result = CreateCollisionComp(entity, component);
			break;
		default:
			result = false;
			break;
		}

		component = component->NextSiblingElement();
	}

	return result;
}

EComponentType CEntityGenerator::GetCompType(const std::string & component)
{
	if (component == "Transform")
		return COMPTYPE_TRANSFORM;

	if (component == "Motion")
		return COMPTYPE_MOTION;

	if (component == "Sprite")
		return COMPTYPE_SPRITE;

	if (component == "Collision")
		return COMPTYPE_COLLISION;

	return COMPTYPE_UNKNOWN;
}

bool CEntityGenerator::CreateTransformComp(Entity entity, tinyxml2::XMLElement * node)
{
	TransformComponent transform;
	tinyxml2::XMLElement* nodeData = nullptr;

	nodeData = node->FirstChildElement("PosX");
	if (nodeData)
	{
		int x = 0;
		nodeData->QueryIntText(&x);
		transform.pos.x = x;
	}
	else
		return false;

	node->FirstChildElement("PosY");
	if (nodeData)
	{
		int y = 0;
		nodeData->QueryIntText(&y);
		transform.pos.y = y;
	}
	else
		return false;

	transform.prevPos = transform.pos;
	COMPDB->AddComponent(entity, COMPTYPE_TRANSFORM, &transform);
	return true;
}

bool CEntityGenerator::CreateMotionComp(Entity entity, tinyxml2::XMLElement * node)
{
	MotionComponent motion;
	tinyxml2::XMLElement* nodeData = nullptr;
	nodeData = node->FirstChildElement("Velocity");
	if (nodeData)
		nodeData->QueryIntText(&motion.velocity);
	else
		return false;

	nodeData = node->FirstChildElement("Acceleration");
	if (nodeData)
		nodeData->QueryIntText(&motion.acceleration);
	else
		return false;

	COMPDB->AddComponent(entity, COMPTYPE_MOTION, &motion);
	return true;
}

bool CEntityGenerator::CreateSpriteComp(Entity entity, tinyxml2::XMLElement * node)
{
	SpriteComponent sprite;
	tinyxml2::XMLElement* nodeData = nullptr;
	nodeData = node->FirstChildElement("TextureID");

	if (nodeData)
		sprite.spriteId = nodeData->GetText();
	else
		return false;

	COMPDB->AddComponent(entity, COMPTYPE_SPRITE, &sprite);
	return true;
}

bool CEntityGenerator::CreateCollisionComp(Entity entity, tinyxml2::XMLElement * node)
{
	CollisionComponent collision;
	tinyxml2::XMLElement* nodeData = nullptr;
	nodeData = node->FirstChildElement("Width");
	if (nodeData)
		nodeData->QueryIntText(&collision.width);
	else
		return false;

	nodeData = node->FirstChildElement("Height");
	if (nodeData)
		nodeData->QueryIntText(&collision.height);
	else
		return false;

	nodeData = node->FirstChildElement("Blocking");
	if (nodeData)
		nodeData->QueryBoolText(&collision.blocking);
	else
		return false;

	COMPDB->AddComponent(entity, COMPTYPE_COLLISION, &collision);
	return true;
}
