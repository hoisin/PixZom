//---------------------------------------------------------------------------
//
//	SDL wrapper
//
//---------------------------------------------------------------------------
#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

class CRender
{
public:
	CRender();
	~CRender();

	bool Initialise(Uint32 winWidth, Uint32 winHeight, SDL_Window* pWindow);

	void SetDrawColour(Uint8 bgRed, Uint8 bgGreen, Uint8 bgBlue, Uint8 bgAlpha);

	void BeginDraw() const;
	void EndDraw() const;

	void CleanUp();

	void DrawRectangle(int pX, int pY, int width, int height, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const;
	void DrawRectangleLine(int pX, int pY, int width, int height, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const;

	void DrawTexture(SDL_Texture* pTexture, Uint32 texWidth, Uint32 texHeight, int pX, int pY) const;
	void DrawTexture(SDL_Texture* pTexture, Uint32 texWidth, Uint32 texHeight, Uint32 tX, Uint32 tY, int pX, int pY) const;
	void DrawTexture(SDL_Texture* pTexture, Uint32 texWidth, Uint32 texHeight, Uint32 tX, Uint32 tY, int pX, int pY, double rot,
		bool bFlipH = false, bool bFlipV = false) const;

	SDL_Renderer* GetRenderer() const;

	// Text stuff for debugging output
	bool LoadFont(const char* fontFile, int ptSize);
	void SetFontColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 1);
	void DrawText(const char* text, int posX, int posY) const;
	void GetTextSize(const char* text, int& outWidth, int& outHeight) const;

protected:
	SDL_Renderer* m_pRenderer;

	Uint32 m_winWidth;
	Uint32 m_winHeight;

	Uint8 m_fillR;
	Uint8 m_fillG;
	Uint8 m_fillB;
	Uint8 m_fillA;

	TTF_Font* m_font;
	SDL_Color m_fontColour;
};

