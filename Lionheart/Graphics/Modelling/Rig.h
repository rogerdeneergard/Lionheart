#pragma once

#include "RigObject.h"
#include "Bone.h"
#include "Joint.h"
#include "../../Util/DynamicArray.h"

class Rig : public GraphicsObject
{
public:
	Rig();
	Rig(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr);

	XMVECTOR GetBasePositionVector();
	XMFLOAT3 GetBasePositionFloat3();

	XMVECTOR GetBaseRotationVector();
	XMFLOAT3 GetBaseRotationFloat3();

	void SetBasePosition(XMVECTOR basePosVector, const bool update = true);
	void SetBasePosition(XMFLOAT3 basePosFloat3, const bool update = true);
	void SetBasePosition(float x, float y, float z, const bool update = true);

	void AdjustBasePosition(XMVECTOR basePosVector, const bool update = true);
	void AdjustBasePosition(XMFLOAT3 basePosFloat3, const bool update = true);
	void AdjustBasePosition(float x, float y, float z, const bool update = true);

	void SetBaseRotation(XMVECTOR baseRotVector, const bool update = true);
	void SetBaseRotation(XMFLOAT3 baseRotFloat3, const bool update = true);
	void SetBaseRotation(float x, float y, float z, const bool update = true);

	void AdjustBaseRotation(XMVECTOR baseRotVector, const bool update = true);
	void AdjustBaseRotation(XMFLOAT3 baseRotFloat3, const bool update = true);
	void AdjustBaseRotation(float x, float y, float z, const bool update = true);

	bool IsUpdating();

	void Update() override;
private:
	void UpdateRig();

	XMVECTOR basePosVector;
	XMFLOAT3 basePosFloat3;

	XMVECTOR baseRotVector;
	XMFLOAT3 baseRotFloat3;

	DynamicArray<RigObject *> rigObjects;

	bool isUpdating = false;
};