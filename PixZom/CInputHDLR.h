//---------------------------------------------------------------------------
//
//	Input handler. Manages a number input devices
//
//---------------------------------------------------------------------------
#pragma once

#include <SDL.h>
#include "CKeyboard.h"

class CInputHDLR
{
public:
	CInputHDLR();
	~CInputHDLR();

	bool Initialise(Uint32 keyHeldTreshold);

	void Update(int elapsedTime);

	void HandleEvent(const SDL_Event& sdlEvent);

	const CKeyboard& GetKeyboard() const;

protected:
	void HandleEventKeyboard(const SDL_Event& sdlEvent);

protected:
	CKeyboard m_keyboard;
};

