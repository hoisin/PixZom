#include "PixZom.h"
#include "EventHDLR.h"
#include "ResourceMGR.h"
#include "Components.h"

CPixZom::CPixZom() : m_Initialised(false)
{
	m_player = { -1 };
}


CPixZom::~CPixZom()
{
	m_componentDB->Release();
}

void CPixZom::HandleMessage(const CEventMessage & msg)
{
}

bool CPixZom::Initialise()
{
	// This is owner or componentDB
	m_componentDB = COMPDB;
	m_entityGenerator.SetEntityMGR(&m_entityMGR);

	m_movementSystem.SetLevelMap(&m_levelMap);
	m_transformSystem.SetLevelMap(&m_levelMap);

	// Register listener systems
	EVENTSYS->AddListener(&m_transformSystem, CSetEntityPosEvent::m_type);
	EVENTSYS->AddListener(&m_transformSystem, CCollisionEntityEvent::m_type);

	EVENTSYS->AddListener(&m_movementSystem, CMoveEntityEvent::m_type);
	EVENTSYS->AddListener(&m_movementSystem, CSetEntitySpeed::m_type);
	EVENTSYS->AddListener(&m_movementSystem, CCollisionEntityEvent::m_type);

	// No messages for collision or sprite systems yet.

	m_Initialised = true;

	return true;
}

bool CPixZom::LoadMap()
{
	if (m_Initialised)
	{
		std::string mapId = "test";
		std::string mapXMLDataDir = "..\\Assets\\Data\\PixZomMapData.xml";

		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError error = doc.LoadFile(mapXMLDataDir.c_str());

		if (error == tinyxml2::XML_SUCCESS)
		{
			tinyxml2::XMLElement* node = doc.RootElement();
			if (node)
			{
				tinyxml2::XMLElement* mapNode = node->FirstChildElement("Map");
				if (mapNode)
				{
					while (mapNode)
					{
						std::string readMapId = mapNode->Attribute("id");
						if (readMapId == mapId)
						{

							tinyxml2::XMLElement* widthNode = mapNode->FirstChildElement("Width");
							tinyxml2::XMLElement* heightNode = mapNode->FirstChildElement("Height");
							tinyxml2::XMLElement* tileWidthNode = mapNode->FirstChildElement("TileWidth");
							tinyxml2::XMLElement* tileHeightNode = mapNode->FirstChildElement("TileHeight");

							if (widthNode && heightNode && tileWidthNode && tileHeightNode)
							{
								int mapWidth = widthNode->IntText();
								int mapHeight = heightNode->IntText();
								int tileWidth = tileWidthNode->IntText();
								int tileHeight = tileHeightNode->IntText();

								m_levelMap.ClearMap();
								m_levelMap.CreateMap(tileWidth, tileHeight, (mapWidth / tileWidth), (mapHeight / tileHeight));
							}

							// Spawn entities
							tinyxml2::XMLElement* entitiesNode = mapNode->FirstChildElement("Entities");
							if (entitiesNode)
							{
								tinyxml2::XMLElement* entityNode = entitiesNode->FirstChildElement("Entity");
								while (entityNode)
								{
									std::string entityId = entityNode->Attribute("id");
									Entity newEntity = m_entityGenerator.GenerateEntity(entityId);

									if (entityId == "player_entity")
										m_player = newEntity;

									tinyxml2::XMLElement* transformNode = entityNode->FirstChildElement("Transform");
									if (transformNode)
									{
										tinyxml2::XMLElement* posXNode = transformNode->FirstChildElement("PosX");
										tinyxml2::XMLElement* posYNode = transformNode->FirstChildElement("PosY");
										TransformComponent* transform = static_cast<TransformComponent*>(COMPDB->AccessComponent(newEntity, COMPTYPE_TRANSFORM));
										if (transform)
										{
											transform->pos = glm::ivec2(posXNode->IntText(), posYNode->IntText());
											transform->prevPos = transform->pos;
										}
									}

									m_transformSystem.RegisterEntity(newEntity);
									m_movementSystem.RegisterEntity(newEntity);
									m_spriteSystem.RegisterEntity(newEntity);
									m_collisionSystem.RegisterEntity(newEntity);

									entityNode = entityNode->NextSiblingElement();
								}
							}

							return true;
						}
						mapNode = mapNode->NextSiblingElement();
					}
				}
			}
		}
	}

	return false;
}

void CPixZom::UpdateCollisions(Uint32 elapsedTime)
{
	m_CollisionDetected = m_collisionSystem.CheckCollisions();
}

void CPixZom::Update(Uint32 elapsedTime)
{
	if(!m_CollisionDetected) 
		m_transformSystem.Update();
}

std::list<CSprite*> CPixZom::GetDrawList(int deltaT)
{
	return m_spriteSystem.GetDrawList(deltaT);
}

Entity CPixZom::GetPlayerEntity() const
{
	return m_player;
}
