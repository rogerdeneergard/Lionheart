#include "KeyLog.h"
#include<Windows.h>
#include<iostream>
KeyLog::KeyLog()
{
	for (int i = 0; i < sizeof(keyStates) / sizeof(bool); ++i)
	{
		this->keyStates[i] = false;
	}
}

bool KeyLog::EventQueueIsEmpty()
{
	return this->eventQueue.empty();
}

bool KeyLog::CharQueueIsEmpty()
{
	return this->charQueue.empty();
}

KeyEvent KeyLog::NextKey(const bool remove)
{
	if (this->eventQueue.empty())
	{
		return KeyEvent();
	}
	
	KeyEvent ke = this->eventQueue.front();
	if (remove) this->eventQueue.pop();
	return ke;
}

unsigned char KeyLog::NextChar(const bool remove)
{
	if (this->charQueue.empty())
	{
		return 0u;
	}

	unsigned char key = this->charQueue.front();
	if (remove) this->charQueue.pop();
	return key;
}

bool KeyLog::KeyIsDown(const unsigned char key)
{
	return this->keyStates[key];
}

void KeyLog::OnKeyDown(const unsigned char key)
{
	this->keyStates[key] = true;
	this->eventQueue.push(KeyEvent(KeyEvent::KeyEventType::DOWN, key));
}

void KeyLog::OnKeyUp(const unsigned char key)
{
	this->keyStates[key] = false;
	this->eventQueue.push(KeyEvent(KeyEvent::KeyEventType::UP, key));
}

void KeyLog::OnChar(const unsigned char key)
{
	this->charQueue.push(key);
}

void KeyLog::SetAutoRepeatKeys(const bool val)
{
	this->autoRepeatKeys = val;
}

void KeyLog::SetAutoRepeatChars(const bool val)
{
	this->autoRepeatChars = val;
}

bool KeyLog::GetAutoRepeatKeys()
{
	return this->autoRepeatKeys;
}

bool KeyLog::GetAutoRepeatChars()
{
	return this->autoRepeatChars;
}
