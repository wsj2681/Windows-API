#pragma once

#include <directxmath.h>
using namespace DirectX;

class Camera
{
	float positionX = 0.f;
	float positionY = 0.f;
	float positionZ = 0.f;

	float rotationX = 0.f;
	float rotationY = 0.f;
	float rotationZ = 0.f;
	XMMATRIX viewMatrix;

public:
	Camera() = default;
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
	Camera(Camera&&) = delete;
	Camera&& operator=(const Camera&&) = delete;
	~Camera() = default;

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);
};