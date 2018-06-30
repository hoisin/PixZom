#include "CRender.h"

CRender::CRender() : m_pRenderer(nullptr), m_winWidth(0), m_winHeight(0), m_fillR(0), m_fillG(0), 
m_fillB(0), m_fillA(0), m_font(nullptr)
{
}

CRender::~CRender()
{
	// Manually call CleanUp()!
	if (TTF_WasInit())
	{
		if (m_font)
		{
			TTF_CloseFont(m_font);
			m_font = nullptr;
		}

		TTF_Quit();
	}
}

bool CRender::Initialise(Uint32 winWidth, Uint32 winHeight, SDL_Window* pWindow)
{
	// Don't pass in a nullptr....
	if (pWindow) 
	{
		m_pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
		if (m_pRenderer == nullptr)
		{
			// Report some error
			return false;
		}

		// Initialise TTF, fail if return is non-zero
		int ttfInit = TTF_Init();
		if (ttfInit != 0)
			return false;

		return true;
	}

	return false;
}

void CRender::SetDrawColour(Uint8 bgRed, Uint8 bgGreen, Uint8 bgBlue, Uint8 bgAlpha)
{
	m_fillR = bgRed;
	m_fillG = bgGreen;
	m_fillB = bgBlue;
	m_fillA = bgAlpha;

	SDL_SetRenderDrawColor(m_pRenderer, bgRed, bgGreen, bgBlue, bgAlpha);
}

void CRender::BeginDraw() const
{
	SDL_RenderClear(m_pRenderer);
}

void CRender::EndDraw() const
{
	SDL_RenderPresent(m_pRenderer);
}

void CRender::CleanUp()
{
	if (m_pRenderer)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}
}

/*
	Draws a rectangle 
*/
void CRender::DrawRectangle(int pX, int pY, int width, int height, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
	SDL_Rect drawRect;
	drawRect.x = pX;
	drawRect.y = pY;
	drawRect.w = width;
	drawRect.h = height;

	// Set fill colour
	SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);
	SDL_RenderFillRect(m_pRenderer, &drawRect);
	// Restore fill colour
	SDL_SetRenderDrawColor(m_pRenderer, m_fillR, m_fillG, m_fillB, m_fillA);
}

/*
	Draws rectangle outline
*/
void CRender::DrawRectangleLine(int pX, int pY, int width, int height, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
	// Set fill colour
	SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);

	SDL_Point points[5] = { 0, 0 };
	points[0].x = pX;
	points[0].y = pY;
	points[1].x = pX + width;
	points[1].y = pY;
	points[2].x = pX + width;
	points[2].y = pY + height;
	points[3].x = pX;
	points[3].y = pY + height;
	points[4].x = pX;
	points[4].y = pY;
	SDL_RenderDrawLines(m_pRenderer, points, 5);

	// Restore fill colour
	SDL_SetRenderDrawColor(m_pRenderer, m_fillR, m_fillG, m_fillB, m_fillA);
}

/*
	Draws texture of specified texture area at screen location pX,pY (top left).
*/
void CRender::DrawTexture(SDL_Texture * pTexture, Uint32 texWidth, Uint32 texHeight, int pX, int pY) const
{
	SDL_Rect src;
	SDL_Rect dst;

	// Texture src
	src.x = 0;
	src.y = 0;
	src.w = texWidth;
	src.h = texHeight;

	// Window dst
	dst.x = pX;
	dst.y = pY;
	dst.w = texWidth;
	dst.h = texHeight;

	SDL_RenderCopy(m_pRenderer, pTexture, &src, &dst);
}

/*
	Draws texture of specified texture area at screen location pX,pY (top left).
	Additional tX, tY allows specify start of area (top left) on texture
*/
void CRender::DrawTexture(SDL_Texture * pTexture, Uint32 texWidth, Uint32 texHeight, Uint32 tX, Uint32 tY, int pX, int pY) const
{
	SDL_Rect src;
	SDL_Rect dst;

	// Texture src
	src.x = tX;
	src.y = tY;
	src.w = texWidth;
	src.h = texHeight;

	// Window dst
	dst.x = pX;
	dst.y = pY;
	dst.w = texWidth;
	dst.h = texHeight;

	SDL_RenderCopy(m_pRenderer, pTexture, &src, &dst);
}

/*
	Draws texture of specified texture area at screen location pX,pY (top left).
	Additional tX, tY allows specify start of area (top left) on texture
	Support for rotation and flip
*/
void CRender::DrawTexture(SDL_Texture * pTexture, Uint32 texWidth, Uint32 texHeight, Uint32 tX, Uint32 tY, int pX, int pY, double rot,
	bool bFlipH, bool bFlipV) const
{
	SDL_Rect src;
	SDL_Rect dst;

	// Texture src
	src.x = tX;
	src.y = tY;
	src.w = texWidth;
	src.h = texHeight;

	// Window dst
	dst.x = pX;
	dst.y = pY;
	dst.w = texWidth;
	dst.h = texHeight;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	if (bFlipH)
		flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);

	if (bFlipV)
		flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);

	SDL_RenderCopyEx(m_pRenderer, pTexture, &src, &dst, rot, nullptr, flip);
}

SDL_Renderer * CRender::GetRenderer() const
{
	return m_pRenderer;
}

bool CRender::LoadFont(const char * fontFile, int ptSize)
{
	m_font = TTF_OpenFont(fontFile, ptSize);

	if (!m_font)
		return false;

	return true;
}

void CRender::SetFontColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	m_fontColour.r = r;
	m_fontColour.g = g;
	m_fontColour.b = b;
	m_fontColour.a = a;
}

void CRender::DrawText(const char * text, int posX, int posY) const
{
	if (m_font)
	{
		SDL_Surface* textSurface = TTF_RenderText_Solid(m_font,
			text, m_fontColour);

		SDL_Texture* textureText = SDL_CreateTextureFromSurface(m_pRenderer,
			textSurface);

		int width = 0;
		int height = 0;
		TTF_SizeText(m_font, text, &width, &height);
		DrawTexture(textureText, width, height, posX, posY);

		SDL_DestroyTexture(textureText);
		SDL_FreeSurface(textSurface);
	}
}

void CRender::GetTextSize(const char * text, int & outWidth, int & outHeight) const
{
	if(m_font)
		TTF_SizeText(m_font, text, &outWidth, &outHeight);
}