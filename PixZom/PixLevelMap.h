//---------------------------------------------------------------------------
//
//	Level map class.
//	Tile-based 
//  Defines moveable areas
//
//---------------------------------------------------------------------------
#pragma once

#include <vector>
#include <SDL.h>
#include <glm.hpp>
#include "Box.h"

struct STile
{
	bool bEnter;

	STile() {
		bEnter = true;
	};
};

// Access is [y][x]
typedef std::vector<std::vector<STile>> TileMap;

class CPixLevelMap
{
public:
	CPixLevelMap();
	~CPixLevelMap();

	bool CreateMap(Uint32 tileWidth, Uint32 tileHeight, Uint32 tilesInX, Uint32 tilesInY);
	STile* AccessTile(Uint32 x, Uint32 y);

	Uint32 GetWorldWidth() const;
	Uint32 GetWorldHeight() const;
	Uint32 GetTileWidth() const;
	Uint32 GetTileHeigh() const;

	bool HasHitMapBoundary(glm::ivec2 entityPos, CBox& box) const;

	void ClearMap(); 

protected:
	// Pixel dimensions
	Uint32 m_worldWidth;
	Uint32 m_worldHeight; 

	// Dimensions of each tile in map
	Uint32 m_tileWidth;
	Uint32 m_tileHeight;

	// Storing every tile in the map
	TileMap m_tileMap;
};

