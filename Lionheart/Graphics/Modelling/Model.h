#pragma once

#include "../Vertex.h"
#include "../DXBuffer.h"
#include "../DXCBuffer.h"
#include "../GraphicsObject.h"
#include "../../Util/DynamicArray.h"
#include "Mesh.h"
#include "Rig.h"

using namespace DirectX;


class Model : public GraphicsObject
{
public:
	Model();
	Model(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr);
	bool Initialise(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr, ID3D11ShaderResourceView * texturePtr,
		DXBuffer<CB_VS_View> &vsViewBuffer, Mesh * meshPtr);
	void SetTexture(ID3D11ShaderResourceView * texturePtr);

	void Draw(const XMMATRIX & viewMatrix);

	Mesh * GetMeshPtr();

	void Update() override;

private:
	ID3D11ShaderResourceView * texturePtr = nullptr;

	DXBuffer<CB_VS_View> * vsViewBufferPtr = nullptr;

	Mesh * meshPtr;


};