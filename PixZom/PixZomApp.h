//---------------------------------------------------------------------------
//
//	Main window app handling class
//
//---------------------------------------------------------------------------
#pragma once

#include <string>
#include <SDL.h>

#include "CRender.h"
#include "CTextureMGR.h"
#include "CInputHDLR.h"
#include "EventHDLR.h"
#include "PixZom.h"
#include "ResourceMGR.h"

class CPixZomApp
{
public:
	CPixZomApp();
	~CPixZomApp();

	bool Initialise(const std::string& windowName, Uint32 windowWidth, Uint32 windowHeight);

	void Run();

private:
	void CleanUp();

	void RegisterMessages();
	void RegisterListeners();

	void HandleInput(int elapsedTime);
	void Draw(int elapsedTime);

	EMoveDirection GetDirection(bool moveUp, bool moveDown, bool moveLeft, bool moveRight) const;
	
private:
	std::string m_windowName;
	Uint32 m_windowWidth;
	Uint32 m_windowHeight;

	bool m_bRun;

	SDL_Window* m_pWindow;

	unsigned int m_tick;
	unsigned int m_lastUpdate;

	bool m_bDrawEntityBB;

	CEventHDLR* m_eventHDLR;
	CResourceMGR* m_resourceMGR;

	CInputHDLR m_inputHDLR;
	CRender m_Render;
	CTextureMGR m_textureMGR;

	CPixZom m_pixZom;
};

