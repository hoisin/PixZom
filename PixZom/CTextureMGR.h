//---------------------------------------------------------------------------
//
//	Texture manager
//
//---------------------------------------------------------------------------
#pragma once

#include <memory>
#include <vector>
#include "gfx_defines.h"

class CTextureMGR
{
public:
	CTextureMGR();
	~CTextureMGR();

	bool Initialise(SDL_Renderer* pRenderer);
	int LoadTexture(const std::string fileDir, const std::string textureID);

	STexture* GetTexture(Uint32 index);
	STexture* GetTexture(const std::string& textureID);

	Uint32 GetTotalTextures() const;

	void CleanUp();

protected:
	std::vector<STexture> m_textures;
	SDL_Renderer* m_sdlRenderer;
};

