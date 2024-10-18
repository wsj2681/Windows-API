#pragma once
class InputClass
{
	bool keys[256];

public:
    InputClass() = default;
    InputClass(const InputClass&) = delete;
    InputClass& operator=(const InputClass&) = delete;
    InputClass(const InputClass&&) = delete;
    InputClass&& operator=(const InputClass&&) = delete;
    ~InputClass() = default;

    void Initialize() noexcept;
    void KeyDown(unsigned int) noexcept;
    void KeyUp(unsigned int) noexcept;
    bool IsKeyDown(unsigned int);
};

