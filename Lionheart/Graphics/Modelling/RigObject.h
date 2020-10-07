#pragma once

#include "../GraphicsObject.h"

class Rig;
class Joint;
class Bone;

class RigObject : public GraphicsObject
{
public:
	RigObject() : GraphicsObject() {}
	RigObject(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr) : 
		GraphicsObject(devicePtr, contextPtr), rigUpdated(false), rigPtr(nullptr) {}
	RigObject(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr, Rig * rigPtr) :
		GraphicsObject(devicePtr, contextPtr), rigUpdated(false), rigPtr(rigPtr) {};
	inline bool GetUpdated() { return rigUpdated; }
	inline void ToggleUpdated() { rigUpdated = !rigUpdated; }
protected:
	bool rigUpdated = false;
	Rig * rigPtr;
};