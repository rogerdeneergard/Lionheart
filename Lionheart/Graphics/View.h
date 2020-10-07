#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include "GraphicsObject.h"
using namespace DirectX;

class View : public GraphicsObject
{
public:
	View();
	View(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr);
	void SetProjection(float fovDeg, float aspectRatio, float nearZ, float farZ);

	const XMMATRIX & GetViewMatrix() const;
	const XMMATRIX & GetProjectionMatrix() const;

	void Update() override;

private:
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
};