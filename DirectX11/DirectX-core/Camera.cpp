#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(const Camera&)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	this->positionX = x;
	this->positionY = y;
	this->positionZ = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	this->rotationX = x;
	this->rotationY = y;
	this->rotationZ = z;
}

XMFLOAT3 Camera::GetPosition()
{
	return XMFLOAT3(positionX, positionY, positionZ);
}

XMFLOAT3 Camera::GetRotation()
{
	return XMFLOAT3(rotationX, rotationY, rotationZ);
}

void Camera::Render()
{
	XMVECTOR up = { 0.f, 1.f, 0.f };
	XMVECTOR position = { positionX, positionY, positionZ };
	XMVECTOR lookAt = { 0.f, 0.f, 1.f };

	float pitch = rotationX * 0.0174532925f;
	float yaw = rotationY * 0.0174532925f;
	float roll = rotationZ * 0.0174532925f;

	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);
	
	lookAt += position;

	viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
}

void Camera::GetViewMatrix(XMMATRIX& view)
{
	view = this->viewMatrix;
}
