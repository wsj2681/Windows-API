#include "cameraclass.h"

void Camera::SetPosition(float x, float y, float z)
{
	positionX = x;
	positionY = y;
	positionZ = z;
}


void Camera::SetRotation(float x, float y, float z)
{
	rotationX = x;
	rotationY = y;
	rotationZ = z;
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
	XMFLOAT3 up = { 0.f, 1.f, 0.f };
	XMFLOAT3 position = { positionX, positionY, positionZ };
	XMFLOAT3 lookAt = { 0.f, 0.f, 1.f };
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	upVector = XMLoadFloat3(&up);
	positionVector = XMLoadFloat3(&position);
	lookAtVector = XMLoadFloat3(&lookAt);

	pitch = rotationX * 0.0174532925f;
	yaw = rotationY * 0.0174532925f;
	roll = rotationZ * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}


void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = this->viewMatrix;
}