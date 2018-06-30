//---------------------------------------------------------------------------
//
//	Misc defines related to graphics
//
//---------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>
#include <map>
#include <SDL.h>


// Struct holding data for loaded texture in SDL_Surface
struct STexture 
{
	std::string textureID;			// ID of texture
	SDL_Texture* pTextureSurface;	// Pointer to loaded texture
	Uint32 textureWidth;				
	Uint32 textureHeight;
	
	// Add animation frame data later
	Uint32 frameWidth;
	Uint32 frameHeight;
	
	// Animations refered to as string id
	// Number represents number of frames in animation
	std::map<std::string, int> m_rowFrames;

	STexture()
	{
		textureID = "";
		pTextureSurface = nullptr;
		textureWidth = 0;
		textureHeight = 0;
		frameWidth = 0;
		frameHeight = 0;
	}
};


