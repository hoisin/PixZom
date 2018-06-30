//---------------------------------------------------------------------------
//
//	Keyboard state handler
//
//---------------------------------------------------------------------------
#pragma once

#include <SDL.h>

class CKeyboard
{
public:
	CKeyboard();
	~CKeyboard();

	bool Initialise(int heldThreshold);

	void Update(unsigned int elapsedTime);

	bool IsKeyDown(int key) const;
	bool IsKeyUp(int key) const;
	bool IsKeyHeld(int key) const;
	
	void EventKeyDown(int key);
	void EventKeyUp(int key);

protected:
	char m_keys[256];
	int m_keyHeldTimer[256];

	int m_heldTimeThreshold;
};

