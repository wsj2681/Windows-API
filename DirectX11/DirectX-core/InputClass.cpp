#include "InputClass.h"

void InputClass::Initialize() noexcept
{
	for (int i = 0; i < 256; ++i)
	{
		keys[i] = false;
	}
}

void InputClass::KeyDown(unsigned int input) noexcept
{
	keys[input] = true;
}

void InputClass::KeyUp(unsigned int input) noexcept
{
	keys[input] = false;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return keys[key];
}
