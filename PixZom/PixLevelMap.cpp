#include "PixLevelMap.h"

CPixLevelMap::CPixLevelMap() : m_worldWidth(0), m_worldHeight(0), m_tileWidth(0), m_tileHeight(0)
{
}

CPixLevelMap::~CPixLevelMap()
{
}

bool CPixLevelMap::CreateMap(Uint32 tileWidth, Uint32 tileHeight, Uint32 tilesInX, Uint32 tilesInY)
{
	m_worldWidth = tileWidth * tilesInX;
	m_worldHeight = tileHeight * tilesInY;

	m_tileMap.resize(tilesInY);
	for (unsigned int y = 0; y < tilesInY; y++) {
		m_tileMap[y].resize(tilesInX);
	}
	return true;
}

STile * CPixLevelMap::AccessTile(Uint32 x, Uint32 y)
{
	STile* returnTile = nullptr;

	if (y < (Uint32)m_tileMap.size()) {
		if (x < (Uint32)m_tileMap[y].size()) {
			returnTile = &m_tileMap[y][x];
		}
	}

	return returnTile;
}

Uint32 CPixLevelMap::GetWorldWidth() const
{
	return m_worldWidth;
}

Uint32 CPixLevelMap::GetWorldHeight() const
{
	return m_worldHeight;
}

Uint32 CPixLevelMap::GetTileWidth() const
{
	return m_tileWidth;
}

Uint32 CPixLevelMap::GetTileHeigh() const
{
	return m_tileHeight;
}

bool CPixLevelMap::HasHitMapBoundary(glm::ivec2 entityPos, CBox & box) const
{
	// Calculate entity box in world
	int entityLeft = entityPos.x - (box.GetWidth() / 2);
	int entityTop = entityPos.y - (box.GetHeight() / 2);
	int entityRight = entityPos.x + (box.GetWidth() / 2);
	int entityBottom = entityPos.y + (box.GetHeight() / 2);

	CBox entity(entityLeft, entityTop, entityRight, entityBottom);
	CBox map(0, 0, m_worldWidth, m_worldHeight);

	return !entity.IsContained(map);
}

void CPixLevelMap::ClearMap()
{
	for (int y = 0; y < (int)m_tileMap.size(); y++) {
		m_tileMap[y].clear();
	}

	m_worldWidth = 0;
	m_worldHeight = 0;
	m_tileWidth = 0;
	m_tileHeight = 0;
	m_tileMap.clear();
}
