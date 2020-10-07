#include "GraphicsObject.h"
#include <Windows.h>
#include <string>

GraphicsObject::GraphicsObject() 
	: pos(XMFLOAT3(0.0f, 0.0f, 0.0f)), posVector(XMLoadFloat3(&pos)), rot(XMFLOAT3(0.0f, 0.0f, 0.0f)),
	rotVector(XMLoadFloat3(&rot)), devicePtr(nullptr), contextPtr(nullptr) {}

GraphicsObject::GraphicsObject(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr) 
	: pos(XMFLOAT3(0.0f, 0.0f, 0.0f)), posVector(XMLoadFloat3(&pos)), rot(XMFLOAT3(0.0f, 0.0f, 0.0f)), 
	rotVector(XMLoadFloat3(&rot)), devicePtr(devicePtr), contextPtr(contextPtr)
{
	Update();
}

const XMVECTOR GraphicsObject::GetPositionVector() const
{
	return posVector;
}

const XMFLOAT3 GraphicsObject::GetPositionFloat3() const
{
	return pos;
}

const XMVECTOR GraphicsObject::GetRotationVector() const
{
	return rotVector;
}

const XMFLOAT3 GraphicsObject::GetRotationFloat3() const
{
	return rot;
}

void GraphicsObject::SetPosition(const XMVECTOR posVector, const bool update)
{
	XMStoreFloat3(&pos, posVector);
	this->posVector = posVector;
	if(update) Update();
}

void GraphicsObject::SetPosition(const XMFLOAT3 posFloat3, const bool update)
{
	pos.x = posFloat3.x;
	pos.y = posFloat3.y;
	pos.z = posFloat3.z;
	posVector = XMLoadFloat3(&pos);
	if (update) Update();
}

void GraphicsObject::SetPosition(float x, float y, float z, const bool update)
{
	pos = XMFLOAT3(x, y, z);
	posVector = XMLoadFloat3(&pos);
	if (update) Update();
}

void GraphicsObject::AdjustPosition(const XMVECTOR posVector, const bool update)
{
	this->posVector += posVector;
	XMStoreFloat3(&pos, this->posVector);
	if (update) Update();
}

void GraphicsObject::AdjustPosition(const XMFLOAT3 posFloat3, const bool update)
{
	pos.x += posFloat3.x;
	pos.y += posFloat3.y;
	pos.z += posFloat3.z;
	posVector = XMLoadFloat3(&pos);
	if (update) Update();
}

void GraphicsObject::AdjustPosition(float x, float y, float z, const bool update)
{
	pos.x += x;
	pos.y += y;
	pos.z += z;
	posVector = XMLoadFloat3(&pos);
	if (update) Update();
}

void GraphicsObject::SetRotation(const XMVECTOR rotVector, const bool update)
{
	XMStoreFloat3(&rot, rotVector);
	FixRotation(rot);
	if (update) Update();
}

void GraphicsObject::SetRotation(const XMFLOAT3 rotFloat3, const bool update)
{
	rot.x = rotFloat3.x;
	rot.y = rotFloat3.y;
	rot.z = rotFloat3.z;
	FixRotation(rot);
	if (update) Update();
}

void GraphicsObject::SetRotation(float x, float y, float z, const bool update)
{
	rot = XMFLOAT3(x, y, z);
	FixRotation(rot);
	if (update) Update();
}

void GraphicsObject::AdjustRotation(const XMVECTOR rotVector, const bool update)
{
	XMStoreFloat3(&rot, this->rotVector + rotVector);
	FixRotation(rot);
	if (update) Update();
}

void GraphicsObject::AdjustRotation(const XMFLOAT3 rotFloat3, const bool update)
{
	rot.x += rotFloat3.x;
	rot.y += rotFloat3.y;
	rot.z += rotFloat3.z;
	FixRotation(rot);
	if (update) Update();
}

void GraphicsObject::AdjustRotation(float x, float y, float z, const bool update)
{
	rot.x += x;
	rot.y += y;
	rot.z += z;
	FixRotation(rot);
	if (update) Update();
}

void GraphicsObject::FixRotation(XMVECTOR &rotVector)
{
	XMFLOAT3 rotFloat3;
	XMStoreFloat3(&rotFloat3, rotVector);
	FixRotation(rotFloat3);
	rotVector = XMLoadFloat3(&rotFloat3);
}

void GraphicsObject::FixRotation(XMFLOAT3 &rotFloat3)
{
	while (rotFloat3.x > XM_PI) rotFloat3.x -= XM_2PI;
	while (rotFloat3.x < -XM_PI) rotFloat3.x += XM_2PI;
	while (rotFloat3.y > XM_PI) rotFloat3.y -= XM_2PI;
	while (rotFloat3.y < -XM_PI) rotFloat3.y += XM_2PI;
	while (rotFloat3.z > XM_PI) rotFloat3.z -= XM_2PI;
	while (rotFloat3.z < -XM_PI) rotFloat3.z += XM_2PI;
}

void GraphicsObject::SetLookAtPos(const XMVECTOR posVector, const bool update)
{
	XMFLOAT3 lookAtPos;
	XMStoreFloat3(&lookAtPos, posVector);

	SetLookAtPos(lookAtPos.x, lookAtPos.y, lookAtPos.z, update);
}

void GraphicsObject::SetLookAtPos(const XMFLOAT3 posFloat3, const bool update)
{
	SetLookAtPos(posFloat3.x, posFloat3.y, posFloat3.z, update);
}

void GraphicsObject::SetLookAtPos(float x, float y, float z, const bool update)
{
	if (x == pos.x && y == pos.y && z == pos.z) return;
	XMFLOAT3 view;
	view.x = pos.x - x;
	view.y = pos.y - y;
	view.z = pos.z - z;


	float pitch = rot.x;
	if (view.y != 0.0f && ((lookMode & CM_PITCH) != 0))
	{
		const float distance = sqrt(view.x * view.x + view.y * view.y);
		pitch = atan(view.y / distance);
	}

	float yaw = rot.y;
	if (view.z != 0.0f && ((lookMode & CM_YAW) != 0))
	{
		yaw = atan(view.x / view.z);

		if (view.z > 0)
		{
			yaw += XM_PI;
		}
	}

	SetRotation(pitch, yaw, rot.z, update);
}

const XMVECTOR GraphicsObject::GetForwardVector(UINT cm)
{
	if (cm == NULL) return forwardVector;
	else
	{
		XMMATRIX controlMatrix = DetermineControlMatrix(cm);
		return XMVector3TransformCoord(pZ_UNIT_VECTOR, controlMatrix);
	}
}

const XMVECTOR GraphicsObject::GetBackVector(UINT cm)
{
	if (cm == NULL) return backVector;
	else
	{
		XMMATRIX controlMatrix = DetermineControlMatrix(cm);
		return XMVector3TransformCoord(nZ_UNIT_VECTOR, controlMatrix);
	}
}

const XMVECTOR GraphicsObject::GetLeftVector(UINT cm)
{
	if (cm == NULL) return leftVector;
	else
	{
		XMMATRIX controlMatrix = DetermineControlMatrix(cm);
		return XMVector3TransformCoord(nX_UNIT_VECTOR, controlMatrix);
	}
}

const XMVECTOR GraphicsObject::GetRightVector(UINT cm)
{
	if (cm == NULL) return rightVector;
	else
	{
		XMMATRIX controlMatrix = DetermineControlMatrix(cm);
		return XMVector3TransformCoord(pX_UNIT_VECTOR, controlMatrix);
	}
}

const XMVECTOR GraphicsObject::GetUpVector(UINT cm)
{
	if (cm == NULL) return upVector;
	else
	{
		XMMATRIX controlMatrix = DetermineControlMatrix(cm);
		return XMVector3TransformCoord(pY_UNIT_VECTOR, controlMatrix);
	}
}

const XMVECTOR GraphicsObject::GetDownVector(UINT cm)
{
	if (cm == NULL) return downVector;
	else
	{
		XMMATRIX controlMatrix = DetermineControlMatrix(cm);
		return XMVector3TransformCoord(nY_UNIT_VECTOR, controlMatrix);
	}
}

const XMMATRIX GraphicsObject::GetWorldMatrix()
{
	return worldMatrix;
}

void GraphicsObject::UpdateDirectionVectors(XMMATRIX controlMatrix)
{
	forwardVector = XMVector3TransformCoord(pZ_UNIT_VECTOR, controlMatrix);
	backVector = XMVector3TransformCoord(nZ_UNIT_VECTOR, controlMatrix);
	leftVector = XMVector3TransformCoord(nX_UNIT_VECTOR, controlMatrix);
	rightVector = XMVector3TransformCoord(pX_UNIT_VECTOR, controlMatrix);
	upVector = XMVector3TransformCoord(pY_UNIT_VECTOR, controlMatrix);
	downVector = XMVector3TransformCoord(nY_UNIT_VECTOR, controlMatrix);
}

XMMATRIX GraphicsObject::DetermineControlMatrix(UINT cm)
{
	XMMATRIX pitchMatrix = XMMatrixRotationRollPitchYaw(rot.x, 0.0f, 0.0f);
	XMMATRIX yawMatrix = XMMatrixRotationRollPitchYaw(0.0f, rot.y, 0.0f);
	XMMATRIX rollMatrix = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, rot.z);

	XMMATRIX controlMatrix = XMMatrixIdentity();
	if (cm == NULL) cm = controlMode;

	while (cm != 0)
	{
		if (cm % CM_ROLL == 0)
		{
			controlMatrix *= rollMatrix;
			cm -= CM_ROLL;
		}
		else if (cm % CM_YAW == 0)
		{
			controlMatrix *= yawMatrix;
			cm -= CM_YAW;
		}
		else if (cm % CM_PITCH == 0)
		{
			controlMatrix *= pitchMatrix;
			cm -= CM_PITCH;
		}
	}

	return controlMatrix;
}

UINT GraphicsObject::GetControlMode()
{
	return controlMode;
}

void GraphicsObject::SetControlMode(UINT cm)
{
	controlMode = cm;
	Update();
}

UINT GraphicsObject::GetLookMode()
{
	return lookMode;
}

void GraphicsObject::SetLookMode(UINT cm)
{
	lookMode = cm;
	Update();
}

void GraphicsObject::Update() 
{
	UpdateWorldMatrix();
}

void GraphicsObject::UpdateWorldMatrix()
{
	worldMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(pos.x, pos.y, pos.z);
	UpdateDirectionVectors(DetermineControlMatrix());
}