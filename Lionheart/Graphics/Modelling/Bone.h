#pragma once

#include "../GraphicsObject.h"
#include <wrl/client.h>
#include "RigObject.h"

using namespace DirectX;

class Bone : public RigObject
{
public:
	Bone();
	Bone(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr, Joint * socketPtr, 
		XMVECTOR socketVector, float len, Rig * rigPtr);
	XMVECTOR GetEndPos();
	float GetLength();
	~Bone();

	void Update() override;

	XMMATRIX GetOffsetMatrix();

private:
	void UpdateEndPos();

	Joint * socketPtr;
	XMVECTOR socketVector;
	float len;

	XMMATRIX inverseBindPoseMatrix;

	XMVECTOR endPos;
};