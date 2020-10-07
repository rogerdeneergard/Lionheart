#include "Rig.h"

Rig::Rig() : GraphicsObject() {}

Rig::Rig(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr) : GraphicsObject(devicePtr, contextPtr){}

XMVECTOR Rig::GetBasePositionVector()
{
	return basePosVector;
}

XMFLOAT3 Rig::GetBasePositionFloat3()
{
	return basePosFloat3;
}

XMVECTOR Rig::GetBaseRotationVector()
{
	return baseRotVector;
}

XMFLOAT3 Rig::GetBaseRotationFloat3()
{
	return baseRotFloat3;
}

void Rig::SetBasePosition(XMVECTOR basePosVector, const bool update)
{
	XMFLOAT3 basePosFloat3;
	XMStoreFloat3(&basePosFloat3, basePosVector);
	SetBasePosition(basePosFloat3, update);
}

void Rig::SetBasePosition(XMFLOAT3 basePosFloat3, const bool update)
{
	this->basePosFloat3.x = basePosFloat3.x;
	this->basePosFloat3.y = basePosFloat3.y;
	this->basePosFloat3.z = basePosFloat3.z;
	basePosVector = XMLoadFloat3(&this->basePosFloat3);
	if (update) Update();
}

void Rig::SetBasePosition(float x, float y, float z, const bool update)
{
	XMFLOAT3 basePosFloat3 = XMFLOAT3(x, y, z);
	SetBasePosition(basePosFloat3, update);
}

void Rig::AdjustBasePosition(XMVECTOR basePosVector, const bool update)
{
	XMFLOAT3 basePosFloat3;
	XMStoreFloat3(&basePosFloat3, (basePosVector + this->basePosVector));
	AdjustBasePosition(basePosFloat3, update);
}

void Rig::AdjustBasePosition(XMFLOAT3 basePosFloat3, const bool update)
{
	basePosFloat3.x += this->basePosFloat3.x;
	basePosFloat3.y += this->basePosFloat3.y;
	basePosFloat3.z += this->basePosFloat3.z;
	SetBasePosition(basePosFloat3, update);
}

void Rig::AdjustBasePosition(float x, float y, float z, const bool update)
{
	XMFLOAT3 basePosFloat3 = XMFLOAT3(x + this->basePosFloat3.x,
		y + this->basePosFloat3.y,
		z + this->basePosFloat3.z);
	AdjustBasePosition(basePosFloat3);
}

void Rig::SetBaseRotation(XMVECTOR baseRotVector, const bool update)
{
	XMFLOAT3 baseRotFloat3;
	XMStoreFloat3(&baseRotFloat3, baseRotVector);
	SetBaseRotation(baseRotFloat3, update);
}

void Rig::SetBaseRotation(XMFLOAT3 baseRotFloat3, const bool update)
{
	this->baseRotFloat3.x = baseRotFloat3.x;
	this->baseRotFloat3.y = baseRotFloat3.y;
	this->baseRotFloat3.z = baseRotFloat3.z;
	baseRotVector = XMLoadFloat3(&this->baseRotFloat3);
	if (update) Update();
}

void Rig::SetBaseRotation(float x, float y, float z, const bool update)
{
	XMFLOAT3 baseRotFloat3 = XMFLOAT3(x, y, z);
	SetBaseRotation(baseRotFloat3, update);
}

void Rig::AdjustBaseRotation(XMVECTOR baseRotVector, const bool update)
{
	XMFLOAT3 baseRotFloat3;
	XMStoreFloat3(&baseRotFloat3, (baseRotVector + this->baseRotVector));
	AdjustBaseRotation(baseRotFloat3, update);
}

void Rig::AdjustBaseRotation(XMFLOAT3 baseRotFloat3, const bool update)
{
	baseRotFloat3.x += this->baseRotFloat3.x;
	baseRotFloat3.y += this->baseRotFloat3.y;
	baseRotFloat3.z += this->baseRotFloat3.z;
	SetBaseRotation(baseRotFloat3, update);
}

void Rig::AdjustBaseRotation(float x, float y, float z, const bool update)
{
	XMFLOAT3 baseRotFloat3 = XMFLOAT3(x + this->baseRotFloat3.x,
		y + this->baseRotFloat3.y,
		z + this->baseRotFloat3.z);
	AdjustBaseRotation(baseRotFloat3);
}

bool Rig::IsUpdating()
{
	return isUpdating;
}

void Rig::Update()
{
	UpdateRig();
	UpdateWorldMatrix();
}

void Rig::UpdateRig()
{
	isUpdating = true;
	for (int i = 0; i < rigObjects.GetLength(); ++i)
	{
		rigObjects.Get(i)->Update();
		rigObjects.Get(i)->ToggleUpdated();
	}
	for (int i = 0; i < rigObjects.GetLength(); ++i)
	{
		rigObjects.Get(i)->ToggleUpdated();
	}
	isUpdating = false;
}