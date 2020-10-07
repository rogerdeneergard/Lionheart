#pragma once

#include "../GraphicsObject.h"
#include "../DXBuffer.h"
#include "../Vertex.h"
#include "../../Util/DynamicArray.h"

class Mesh : public GraphicsObject
{
public:
	Mesh();
	Mesh(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr);
	DXBuffer<Vertex> * GetVertexBufferPtr();
	DXBuffer<DWORD> * GetIndexBufferPtr();

	void Update() override;

protected:
	DXBuffer<Vertex> vertexBuffer;
	DXBuffer<DWORD> indexBuffer;
	DynamicArray<XMFLOAT3> normals;
};