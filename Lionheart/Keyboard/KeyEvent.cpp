#include "KeyEvent.h"

KeyEvent::KeyEvent() : type(KeyEventType::INVALID) {}

KeyEvent::KeyEvent(const KeyEventType type, const unsigned char key) : type(type), key(key) {}

bool KeyEvent::IsDown() const
{
	return (this->type == KeyEventType::DOWN);
}

bool KeyEvent::IsUp() const
{
	return (this->type == KeyEventType::UP);
}

bool KeyEvent::IsValid() const
{
	return (this->type != KeyEventType::INVALID);
}

unsigned char KeyEvent::GetKey() const
{
	return this->key;
}
