#include "CKeyboard.h"


CKeyboard::CKeyboard() : m_heldTimeThreshold(0)
{
	// Initialise keys to up state
	for (int i = 0; i < 256; i++) 
	{
		m_keys[i] = 'u';
		m_keyHeldTimer[i] = 0;
	}
}


CKeyboard::~CKeyboard()
{
}

bool CKeyboard::Initialise(int heldThreshold)
{
	m_heldTimeThreshold = heldThreshold;

	return true;
}

//---------------------------------------------------------------------------
//
//	Needs to be called per frame
//
//---------------------------------------------------------------------------
void CKeyboard::Update(unsigned int elapsedTime)
{
	// Loop through all possible key combinations
	for (int i = 0; i < 256; i++)
	{
		// If key is registered as 'down'
		if (m_keys[i] == 'd')
		{
			// Add to key's timer
			m_keyHeldTimer[i] += elapsedTime;
			// If exceeds timer threshold, flag as held
			if (m_keyHeldTimer[i] > m_heldTimeThreshold) 
			{
				m_keys[i] = 'h';
			}
		}
	}
}

bool CKeyboard::IsKeyDown(int key) const
{
	if (m_keys[SDL_GetScancodeFromKey(key)] == 'd' || m_keys[SDL_GetScancodeFromKey(key)] == 'h')
		return true;
	
	return false;
}

bool CKeyboard::IsKeyUp(int key) const
{
	if (m_keys[SDL_GetScancodeFromKey(key)] == 'u')
		return true;

	return false;
}

bool CKeyboard::IsKeyHeld(int key) const
{
	if (m_keys[SDL_GetScancodeFromKey(key)] == 'h')
		return true;

	return false;
}

void CKeyboard::EventKeyDown(int key)
{
	if (m_keys[key] == 'u')
	{
		m_keys[key] = 'd';
	}
}

void CKeyboard::EventKeyUp(int key)
{
	if (m_keys[key] != 'u')
	{
		m_keys[key] = 'u';
	}
}

