#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct CB_VS_View
{
	DirectX::XMMATRIX view;
};

struct CB_PS_Alpha
{
	float alpha;
};

struct CB_CS_Skin
{
	XMVECTOR position;
	XMMATRIX boneOffsets[4];
	XMFLOAT4 boneWeights;
};