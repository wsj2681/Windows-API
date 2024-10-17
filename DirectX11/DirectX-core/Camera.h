#pragma once

#include <DirectXMath.h>

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
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX& view);
};

