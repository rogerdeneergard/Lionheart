#include "Joint.h"
#include "Bone.h"
#include "Rig.h"

Joint::Joint() : RigObject() {}

Joint::Joint(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr, Bone * attachPtr, 
	XMVECTOR attachVector, float minPitch, float maxPitch, float minYaw, float maxYaw, float minRoll, 
	float maxRoll, Rig * rigPtr, bool isAttachedAtEnd)
	: RigObject(devicePtr, contextPtr, rigPtr), attachPtr(attachPtr), attachVector(attachVector), 
	minPitch(minPitch), maxPitch(maxPitch), minYaw(minYaw), maxYaw(maxYaw), minRoll(minRoll),
	isAttachedAtEnd(isAttachedAtEnd)
{
	jointRotFloat3 = XMFLOAT3(0.0f, 0.0f, 0.0f);
	jointRotVector = XMLoadFloat3(&jointRotFloat3);
	Update();
}

Joint::~Joint()
{
	free(attachPtr);
}

void Joint::SetJointRotation(XMVECTOR jointRotVector, const bool update)
{
	XMFLOAT3 jointRotFloat3;
	XMStoreFloat3(&jointRotFloat3, jointRotVector);
	SetJointRotation(jointRotFloat3, update);
}

void Joint::SetJointRotation(XMFLOAT3 jointRotFloat3, const bool update)
{
	FixRotation(jointRotFloat3);
	ConstrainJointRotation(jointRotFloat3);
	this->jointRotFloat3.x = jointRotFloat3.x;
	this->jointRotFloat3.y = jointRotFloat3.y;
	this->jointRotFloat3.z = jointRotFloat3.z;
	this->jointRotVector = XMLoadFloat3(&this->jointRotFloat3);
	if (update) rigPtr->Update();
}

void Joint::SetJointRotation(float x, float y, float z, const bool update)
{
	XMFLOAT3 jointRotFloat3 = XMFLOAT3(x, y, z);
	SetJointRotation(jointRotFloat3, update);
}

void Joint::AdjustJointRotation(XMVECTOR jointRotVector, const bool update)
{
	XMFLOAT3 jointRotFloat3;
	XMStoreFloat3(&jointRotFloat3, jointRotVector);
	AdjustJointRotation(jointRotFloat3, update);
}

void Joint::AdjustJointRotation(XMFLOAT3 jointRotFloat3, const bool update)
{
	jointRotFloat3.x += this->jointRotFloat3.x;
	jointRotFloat3.y += this->jointRotFloat3.y;
	jointRotFloat3.z += this->jointRotFloat3.z;
	SetJointRotation(jointRotFloat3, update);
}

void Joint::AdjustJointRotation(float x, float y, float z, const bool update)
{
	XMFLOAT3 jointRotFloat3 = XMFLOAT3(x, y, z);
	AdjustJointRotation(jointRotFloat3);
}

void Joint::ConstrainJointRotation(XMVECTOR &jointRotVector)
{
	XMFLOAT3 jointRotFloat3;
	XMStoreFloat3(&jointRotFloat3, jointRotVector);
	ConstrainJointRotation(jointRotFloat3);
	jointRotVector = XMLoadFloat3(&jointRotFloat3);
}

void Joint::ConstrainJointRotation(XMFLOAT3 &jointRotFloat3)
{
	if (jointRotFloat3.x > maxPitch) jointRotFloat3.x = maxPitch;
	if (jointRotFloat3.x < minPitch) jointRotFloat3.x = minPitch;
	if (jointRotFloat3.y > maxYaw) jointRotFloat3.y = maxYaw;
	if (jointRotFloat3.y < minYaw) jointRotFloat3.y = minYaw;
	if (jointRotFloat3.z > maxRoll) jointRotFloat3.z = maxRoll;
	if (jointRotFloat3.z < minRoll) jointRotFloat3.z = minRoll;
}

void Joint::Update()
{
	if (!rigPtr->IsUpdating()) 
	{
		rigPtr->Update();
		return;
	}
	if (!attachPtr->GetUpdated()) attachPtr->Update();
	if (isAttachedAtEnd) SetPosition(attachPtr->GetEndPos(), false);
	else SetPosition(attachPtr->GetPositionFloat3(), false);
	SetRotation(attachPtr->GetRotationVector() + attachVector, false);
	UpdateWorldMatrix();
}