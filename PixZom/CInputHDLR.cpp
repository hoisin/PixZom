#include "CInputHDLR.h"

CInputHDLR::CInputHDLR()
{
}


CInputHDLR::~CInputHDLR()
{
}

bool CInputHDLR::Initialise(Uint32 keyHeldThreshold)
{
	bool result = true;
	result = m_keyboard.Initialise(keyHeldThreshold);
	return result;
}

//---------------------------------------------------------------------------
//
//	Call on every frame update
//
//---------------------------------------------------------------------------
void CInputHDLR::Update(int elapsedTime)
{
	m_keyboard.Update(elapsedTime);
}

//---------------------------------------------------------------------------
//
//	Call on every SDL event poll
//
//---------------------------------------------------------------------------
void CInputHDLR::HandleEvent(const SDL_Event & sdlEvent)
{
	HandleEventKeyboard(sdlEvent);
}

const CKeyboard & CInputHDLR::GetKeyboard() const
{
	return m_keyboard;
}

void CInputHDLR::HandleEventKeyboard(const SDL_Event & sdlEvent)
{
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

	if (sdlEvent.type == SDL_KEYDOWN)
		m_keyboard.EventKeyDown(sdlEvent.key.keysym.scancode);

	if (sdlEvent.type == SDL_KEYUP)
		m_keyboard.EventKeyUp(sdlEvent.key.keysym.scancode);
}

