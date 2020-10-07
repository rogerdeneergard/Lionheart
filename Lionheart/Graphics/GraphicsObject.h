#pragma once
#include <DirectXMath.h>
#include <Windows.h>

#include<d3d11.h>

using namespace DirectX;

class GraphicsObject
{
public:
	GraphicsObject();
	GraphicsObject(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr);

	const XMVECTOR GetPositionVector() const;
	const XMFLOAT3 GetPositionFloat3() const;
	const XMVECTOR GetRotationVector() const;
	const XMFLOAT3 GetRotationFloat3() const;

	void SetPosition(const XMVECTOR posVector, const bool update = true);
	void SetPosition(const XMFLOAT3 posFloat3, const bool update = true);
	void SetPosition(float x, float y, float z, const bool update = true);

	void AdjustPosition(const XMVECTOR posVector, const bool update = true);
	void AdjustPosition(const XMFLOAT3 posFloat3, const bool update = true);
	void AdjustPosition(float x, float y, float z, const bool update = true);

	void SetRotation(const XMVECTOR rotVector, const bool update = true);
	void SetRotation(const XMFLOAT3 rotFloat3, const bool update = true);
	void SetRotation(float x, float y, float z, const bool update = true);

	void AdjustRotation(const XMVECTOR rotVector, const bool update = true);
	void AdjustRotation(const XMFLOAT3 rotFloat3, const bool update = true);
	void AdjustRotation(float x, float y, float z, const bool update = true);

	void SetLookAtPos(const XMVECTOR posVector, const bool update = true);
	void SetLookAtPos(const XMFLOAT3 posFloat3, const bool update = true);
	void SetLookAtPos(float x, float y, float z, const bool update = true);

	const XMVECTOR GetForwardVector(UINT cm = NULL);
	const XMVECTOR GetBackVector(UINT cm = NULL);
	const XMVECTOR GetLeftVector(UINT cm = NULL);
	const XMVECTOR GetRightVector(UINT cm = NULL);
	const XMVECTOR GetUpVector(UINT cm = NULL);
	const XMVECTOR GetDownVector(UINT cm = NULL);

	const XMMATRIX GetWorldMatrix();

	void UpdateDirectionVectors(XMMATRIX controlMatrix);

	XMMATRIX DetermineControlMatrix(UINT cm = NULL);

	UINT GetControlMode();
	void SetControlMode(UINT cm);

	UINT GetLookMode();
	void SetLookMode(UINT cm);

	const static UINT CM_PITCH = 1;
	const static UINT CM_YAW = 2;
	const static UINT CM_ROLL = 4;

	virtual void Update();

protected:
	virtual void UpdateWorldMatrix();

	void FixRotation(XMVECTOR &_rotVector);
	void FixRotation(XMFLOAT3 &_rotFloat3);

	HRESULT hr;

	XMVECTOR posVector;
	XMFLOAT3 pos;

	XMVECTOR rotVector;
	XMFLOAT3 rot;

	XMMATRIX worldMatrix = XMMatrixIdentity();

	const XMVECTOR pX_UNIT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR pY_UNIT_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR pZ_UNIT_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	const XMVECTOR nX_UNIT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR nY_UNIT_VECTOR = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
	const XMVECTOR nZ_UNIT_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);

	XMVECTOR forwardVector;
	XMVECTOR backVector;
	XMVECTOR leftVector;
	XMVECTOR rightVector;
	XMVECTOR upVector;
	XMVECTOR downVector;

	UINT controlMode = CM_YAW;
	UINT lookMode = CM_YAW + CM_PITCH;

	ID3D11Device * devicePtr = nullptr;
	ID3D11DeviceContext * contextPtr = nullptr;
};