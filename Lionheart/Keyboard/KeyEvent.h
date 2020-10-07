#pragma once

class KeyEvent
{
public:
	enum KeyEventType
	{
		DOWN,
		UP,
		INVALID
	};

	KeyEvent();
	KeyEvent(const KeyEventType type, const unsigned char key);
	bool IsDown() const;
	bool IsUp() const;
	bool IsValid() const;
	unsigned char GetKey() const;

private:
	KeyEventType type;
	unsigned char key;

};