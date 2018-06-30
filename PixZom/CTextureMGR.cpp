#include "CTextureMGR.h"

CTextureMGR::CTextureMGR() : m_sdlRenderer(nullptr)
{
	// WARNING
	// The vector must not change in size when adding 
	// If it does, all pointers pointing to the textures will be invalid.
	// Limitation is that we can only have a certain number textures loaded.
	// 
	// Another solution is to have texture pointers in the vector instead.
	// But you'll lose the contiguous block storing.
	//
	// Or have texture manager return a handle which stores key to vector element
	// in a map.
	// External code need to always request the pointer to STexture when needing to use.
	// Which means STexture* should not be stored at all outside of this class
	m_textures.reserve(2048);
}

CTextureMGR::~CTextureMGR()
{
	CleanUp();
}

bool CTextureMGR::Initialise(SDL_Renderer * pRenderer)
{
	// Need the SDL renderer to load stuff
	m_sdlRenderer = pRenderer;
	return true;
}

int CTextureMGR::LoadTexture( const std::string fileDir, const std::string textureID)
{
	int returnHandle = -1;
	if (m_sdlRenderer)
	{
		SDL_Surface* tempSurface = SDL_LoadBMP(fileDir.c_str());
		if (tempSurface) 
		{
			if (SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0, 255, 0)))
			{
				// Something went wrong...
			}

			STexture newTexture;
			newTexture.pTextureSurface = SDL_CreateTextureFromSurface(m_sdlRenderer, tempSurface);
			newTexture.textureWidth = tempSurface->w;
			newTexture.textureHeight = tempSurface->h;
			newTexture.textureID = textureID;
			m_textures.push_back(newTexture);
			// Don't need the temporary surface anymore
			SDL_FreeSurface(tempSurface);
			// Last texture in the array should be the one just created
			returnHandle = (int)m_textures.size() - 1;
		}
	}

	return returnHandle;
}

STexture* CTextureMGR::GetTexture(Uint32 index)
{
	if (index < m_textures.size())
		return &m_textures[index];

	return nullptr;
}

STexture* CTextureMGR::GetTexture(const std::string & textureID)
{
	// Search through textures for matching texture ID
	for (int i = 0; i < (int)m_textures.size(); i++) 
	{
		if (m_textures[i].textureID == textureID)
			return &m_textures[i];
	}

	return nullptr;
}

Uint32 CTextureMGR::GetTotalTextures() const
{
	return (Uint32)m_textures.size();
}

void CTextureMGR::CleanUp()
{
	// Free all loaded textures
	for (int i = 0; i < (int)m_textures.size(); i++)
	{
		if (m_textures[i].pTextureSurface)
		{
			SDL_DestroyTexture(m_textures[i].pTextureSurface);
			m_textures[i].pTextureSurface = nullptr;
		}
	}
}
