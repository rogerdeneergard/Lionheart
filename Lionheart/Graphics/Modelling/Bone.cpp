#include "Bone.h"
#include "Joint.h"
#include "Rig.h"

Bone::Bone() : RigObject() {}

Bone::Bone(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr, Joint * socketPtr, 
	XMVECTOR socketVector, float len, Rig * rigPtr) 
	: RigObject(devicePtr, contextPtr, rigPtr),
	socketPtr(socketPtr), socketVector(socketVector), len(len) 
{
	Update();
	inverseBindPoseMatrix = XMMatrixInverse(nullptr, worldMatrix);
}

XMVECTOR Bone::GetEndPos()
{
	return endPos;
}

float Bone::GetLength()
{
	return len;
}

Bone::~Bone()
{
	free(socketPtr);
}

void Bone::Update()
{
	if (!rigPtr->IsUpdating())
	{
		rigPtr->Update();
		return;
	}
	if (socketPtr != nullptr)
	{
		if (!socketPtr->GetUpdated()) socketPtr->Update();
		SetPosition(socketPtr->GetPositionVector(), false);
		SetRotation(socketPtr->GetRotationVector() + socketVector, false);
	}
	else
	{
		SetPosition(rigPtr->GetBasePositionFloat3(), false);
		SetRotation(rigPtr->GetBaseRotationFloat3(), false);
	}
	UpdateEndPos();
	UpdateWorldMatrix();
}

XMMATRIX Bone::GetOffsetMatrix()
{
	return inverseBindPoseMatrix * worldMatrix;
}

void Bone::UpdateEndPos()
{
	XMVECTOR lengthVector = (rotVector / XMVector3Length(rotVector)) * len;
	endPos = posVector + lengthVector;
}
