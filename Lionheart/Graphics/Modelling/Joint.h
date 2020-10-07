#pragma once

#include "RigObject.h"

using namespace DirectX;

class Joint : public RigObject
{
public:
	Joint();
	Joint(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr, Bone * attachPtr, 
		XMVECTOR attachVector, float minPitch, float maxPitch, float minYaw, float maxYaw, 
		float minRoll, float maxRoll, Rig * rigPtr, bool isAttachedAtEnd);
	~Joint();

	void SetJointRotation(XMVECTOR jointRotVector, const bool update = true);
	void SetJointRotation(XMFLOAT3 jointRotFloat3, const bool update = true);
	void SetJointRotation(float x, float y, float z, const bool update = true);

	void AdjustJointRotation(XMVECTOR jointRotVector, const bool update = true);
	void AdjustJointRotation(XMFLOAT3 jointRotFloat3, const bool update = true);
	void AdjustJointRotation(float x, float y, float z, const bool update = true);

	void Update() override;

private:
	void ConstrainJointRotation(XMVECTOR &jointRotVector);
	void ConstrainJointRotation(XMFLOAT3 &jointRotFloat3);

	Bone * attachPtr;
	XMVECTOR attachVector;

	float minPitch, maxPitch,
		minYaw, maxYaw,
		minRoll, maxRoll;

	XMVECTOR jointRotVector;
	XMFLOAT3 jointRotFloat3;

	bool isAttachedAtEnd;
};