#include "View.h"
#include <Windows.h>
#include <string>

View::View() : GraphicsObject() {}

View::View(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr) :
	GraphicsObject(devicePtr, contextPtr)
{
	Update();
}

void View::SetProjection(float fovDeg, float aspectRatio, float nearZ, float farZ)
{
	float fovRad = (fovDeg / 360.0f) * XM_2PI;
	projectionMatrix = XMMatrixPerspectiveFovLH(fovRad, aspectRatio, nearZ, farZ);
	Update();
}

const XMMATRIX & View::GetViewMatrix() const
{
	return viewMatrix;
}

const XMMATRIX & View::GetProjectionMatrix() const
{
	return projectionMatrix;
}

void View::Update()
{
	// Create rotation matrix
	XMMATRIX viewRotMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	// Create unit vector in direction of rotation
	XMVECTOR unitVector = XMVector3TransformCoord(pZ_UNIT_VECTOR, viewRotMatrix);
	// Translate rotation unit vector to view position
	XMVECTOR viewVector = unitVector + posVector;
	// Create zenith vector
	XMVECTOR zenithVector = XMVector3TransformCoord(pY_UNIT_VECTOR, viewRotMatrix);
	// Set the view matrix
	viewMatrix = XMMatrixLookAtLH(posVector, viewVector, zenithVector);

	UpdateDirectionVectors(DetermineControlMatrix());
}