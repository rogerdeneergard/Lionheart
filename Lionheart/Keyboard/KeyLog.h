#pragma once
#include "KeyEvent.h"
#include <queue>

class KeyLog
{
public:
	KeyLog();
	bool EventQueueIsEmpty();
	bool CharQueueIsEmpty();
	KeyEvent NextKey(const bool remove = true);
	unsigned char NextChar(const bool remove = true);
	bool KeyIsDown(const unsigned char);
	void OnKeyDown(const unsigned char);
	void OnKeyUp(const unsigned char);
	void OnChar(const unsigned char);
	void SetAutoRepeatKeys(const bool);
	void SetAutoRepeatChars(const bool);
	bool GetAutoRepeatKeys();
	bool GetAutoRepeatChars();
private:
	bool autoRepeatKeys = false;
	bool autoRepeatChars = false;
	bool keyStates[256];
	std::queue<KeyEvent> eventQueue;
	std::queue<unsigned char> charQueue;
};