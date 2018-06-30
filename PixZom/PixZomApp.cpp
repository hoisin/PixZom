#include "PixZomApp.h"

CPixZomApp::CPixZomApp() : m_windowName(""), m_windowWidth(0), m_windowHeight(0), m_bRun(false), 
	m_pWindow(nullptr), m_tick(0), m_lastUpdate(0), m_eventHDLR(nullptr), m_resourceMGR(nullptr),
	m_bDrawEntityBB(false)
{
}


CPixZomApp::~CPixZomApp()
{
}

//---------------------------------------------------------------------------
//
//	Initialise method for setting up sub-systems
//
//---------------------------------------------------------------------------
bool CPixZomApp::Initialise(const std::string& windowName, Uint32 windowWidth, Uint32 windowHeight)
{
	m_windowName = windowName;
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	// Initialise everything in SDL.
	// Initialise sub-systems after this line.
	SDL_Init(SDL_INIT_EVERYTHING);

	m_pWindow = SDL_CreateWindow(m_windowName.c_str() , SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		m_windowWidth, m_windowHeight, SDL_WINDOW_OPENGL);

	if (m_pWindow == nullptr) {
		// To Do:  Report some error
		CleanUp();
		return false;
	}

	// Create the renderer
	if (!m_Render.Initialise(m_windowWidth, m_windowHeight, m_pWindow))
	{
		CleanUp();
		return false;
	}

	// Set background clearing to black
	m_Render.SetDrawColour(0x00, 0x00, 0x00, 0xFF);

	// Renderer debug font text loading for writing text
	if (!m_Render.LoadFont("..\\Assets\\Font\\arial.ttf", 16))
		return false;

	m_Render.SetFontColour(0x00, 0xFF, 0x00);

	// Default tick rate 50ms
	m_tick = 50;
	m_inputHDLR.Initialise(100);

	// This class is the owener of event handler and resource manager
	m_eventHDLR = EVENTSYS;
	m_resourceMGR = RESOURCEMGR;
	m_resourceMGR->Initialise(m_Render.GetRenderer());

	// Setup listeners
	RegisterMessages();
	RegisterListeners();
	
	// Init game class
	m_pixZom.Initialise();
	m_pixZom.LoadMap();

	// Display the window
	SDL_ShowWindow(m_pWindow);

	// Debug
	m_bDrawEntityBB = true;

	m_bRun = true;
	return true;
}

//---------------------------------------------------------------------------
//
//	Main run function which initiates application loop
//
//---------------------------------------------------------------------------
void CPixZomApp::Run() 
{
	// Init the last update
	m_lastUpdate = SDL_GetTicks();

	while (m_bRun) 
	{
		// Poll events
		SDL_Event sdlEvent;
		while (SDL_PollEvent(&sdlEvent)) 
		{
			m_inputHDLR.HandleEvent(sdlEvent);
		}

		// Update stuff that are timed
		unsigned int currentTime = SDL_GetTicks();
		unsigned int elapsedTime = currentTime - m_lastUpdate;

		// Update hardware input detection
		m_inputHDLR.Update(elapsedTime);
		
		if (elapsedTime > m_tick) 
		{
			// Queue input messages from user
			HandleInput(elapsedTime);
			// Update anything needed before processing messages
			m_pixZom.Update(elapsedTime);
			// Process messages
			m_eventHDLR->ProcessEvents(50);
			// Run collision updates
			m_pixZom.UpdateCollisions(elapsedTime);

			// Update last time
			m_lastUpdate = currentTime;
			// Reset elapsed time as we have 'updated'
			elapsedTime = 0;
		}

		// Draw stuff
		Draw(elapsedTime);
	}

	// On exit loop, clean up
	CleanUp();
}

//---------------------------------------------------------------------------
//
//	Free up anything that needs cleaning up
//
//---------------------------------------------------------------------------
void CPixZomApp::CleanUp()
{
	// Clean up render
	m_Render.CleanUp();

	// Clean up the window
	if (m_pWindow) 
		SDL_DestroyWindow(m_pWindow);

	if(m_eventHDLR)
		m_eventHDLR->Release();

	if(m_resourceMGR)
		m_resourceMGR->Release();

	// Quit SDL
	SDL_Quit();
}

//---------------------------------------------------------------------------
//
//	Register all event message types
//
//---------------------------------------------------------------------------
void CPixZomApp::RegisterMessages()
{
	m_eventHDLR->RegisterEvent(CMoveEntityEvent::m_type);
	m_eventHDLR->RegisterEvent(CSetEntitySpeed::m_type);
	m_eventHDLR->RegisterEvent(CSetEntityPosEvent::m_type);
	m_eventHDLR->RegisterEvent(CCollisionEntityEvent::m_type);
	m_eventHDLR->RegisterEvent(CSpawnEntityEvent::m_type);

	m_eventHDLR->RegisterEvent(CLoadMapEvent::m_type);
	m_eventHDLR->RegisterEvent(CGameStartEvent::m_type);
	m_eventHDLR->RegisterEvent(CInitialiseEvent::m_type);
	m_eventHDLR->RegisterEvent(CQuitEvent::m_type);
}

//---------------------------------------------------------------------------
//
//	Register events to listeners we have ownership off in this class
//
//---------------------------------------------------------------------------
void CPixZomApp::RegisterListeners()
{
	m_eventHDLR->AddListener(&m_pixZom, CLoadMapEvent::m_type);
	m_eventHDLR->AddListener(&m_pixZom, CGameStartEvent::m_type);
	m_eventHDLR->AddListener(&m_pixZom, CInitialiseEvent::m_type);
	m_eventHDLR->AddListener(&m_pixZom, CQuitEvent::m_type);
}

//---------------------------------------------------------------------------
//
//	Interpret input commands
//
//---------------------------------------------------------------------------
void CPixZomApp::HandleInput(int elapsedTime)
{
	bool bMoveUp = false;
	bool bMoveDown = false;
	bool bMoveLeft = false;
	bool bMoveRight = false;

	if (m_inputHDLR.GetKeyboard().IsKeyDown(SDLK_ESCAPE))
		m_bRun = false;

	if (m_inputHDLR.GetKeyboard().IsKeyDown(SDLK_w))
	{
		bMoveUp = true;
	}
	if (m_inputHDLR.GetKeyboard().IsKeyDown(SDLK_s))
	{
		bMoveDown = true;
	}
	if (m_inputHDLR.GetKeyboard().IsKeyDown(SDLK_a))
	{
		bMoveLeft = true;
	}
	if (m_inputHDLR.GetKeyboard().IsKeyDown(SDLK_d))
	{
		bMoveRight = true;
		
	}

	EMoveDirection direction = GetDirection(bMoveUp, bMoveDown, bMoveLeft, bMoveRight);
	if (direction != MOVEDIR_UNKNOWN)
	{
		CMoveEntityEvent* moveEvt = new CMoveEntityEvent(direction, m_pixZom.GetPlayerEntity());
		m_eventHDLR->QueueMessage(moveEvt);
	}
}

//---------------------------------------------------------------------------
//
//	All draw code here
//
//---------------------------------------------------------------------------
void CPixZomApp::Draw(int elapsedTime)
{
	// Draw stuff
	std::list<CSprite*> drawList = m_pixZom.GetDrawList(elapsedTime);
	m_Render.BeginDraw();

	m_Render.DrawText("This is a long string to test the text rendering",
		0, 0);

	for (auto it = drawList.begin(); it != drawList.end(); it++)
	{
		CSprite* sprite = (*it);
		int frameWidth = sprite->GetTexture()->frameWidth;
		int frameHeight = sprite->GetTexture()->frameHeight;

		if (m_bDrawEntityBB) {
			m_Render.DrawRectangleLine(sprite->GetPos().x,
				sprite->GetPos().y,
				frameWidth,
				frameHeight,
				255, 0, 0, 0);
		}

		STexture* texture = sprite->GetTexture();
		m_Render.DrawTexture(texture->pTextureSurface,
			frameWidth,
			frameHeight,
			(sprite->GetCurrentFrame() - 1) * frameWidth,
			(sprite->GetFrameRow() - 1) * frameHeight,
			sprite->GetPos().x,
			sprite->GetPos().y,
			0
		);
	}

	m_Render.EndDraw();
}

//---------------------------------------------------------------------------
//
//	Helper method to return direction to move depending on input
//
//---------------------------------------------------------------------------
EMoveDirection CPixZomApp::GetDirection(bool moveUp, bool moveDown, bool moveLeft, bool moveRight) const
{
	if (!(moveUp && moveDown) && !(moveLeft && moveRight))
	{
		if (moveUp && moveLeft)
			return MOVEDIR_UP_LEFT;

		if (moveUp && moveRight)
			return MOVEDIR_UP_RIGHT;

		if (moveDown && moveLeft)
			return MOVEDIR_DOWN_LEFT;

		if (moveDown && moveRight)
			return MOVEDIR_DOWN_RIGHT;

		if (moveLeft)
			return MOVEDIR_LEFT;

		if (moveRight)
			return MOVEDIR_RIGHT;

		if (moveUp)
			return MOVEDIR_UP;

		if (moveDown)
			return MOVEDIR_DOWN;
	}

	return MOVEDIR_UNKNOWN;
}
