#include "Mesh.h"

Mesh::Mesh() : GraphicsObject() {}

Mesh::Mesh(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr) :
	GraphicsObject(devicePtr, contextPtr)
{
	// Define vertices
	Vertex v[] =
	{
	 Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f), //Bottom left
	 Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f), //Top left
	 Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f), //Top right
	 Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f), //Bottom right

	 Vertex(-1.0f, -3.0f, -1.0f, 0.0f, 1.0f), //Bottom left
	 Vertex(-1.0f, -3.0f, 1.0f, 0.0f, 0.0f), //Top left
	 Vertex(1.0f, -3.0f, 1.0f, 1.0f, 0.0f), //Top right
	 Vertex(1.0f, -3.0f, -1.0f, 1.0f, 1.0f), //Bottom right
	};

	// Initialise vertex buffer
	{
		hr = vertexBuffer.Initialise(devicePtr, contextPtr, v, ARRAYSIZE(v));
		COM_CHECK_FAIL(hr, "Failed to initialise vertex buffer on scene initialisation.");
	}

	// Define indices
	DWORD indices[] =
	{
		// TOP
		0, 1, 2,
		0, 2, 3,
		// BOTTOM
		5, 4, 7,
		5, 7, 6,
		// FRONT
		4, 0, 3,
		4, 3, 7,
		// BACK
		6, 2, 1,
		6, 1, 5,
		// LEFT
		5, 1, 0,
		5, 0, 4,
		// RIGHT
		7, 3, 2,
		7, 2, 6
	};

	// Initialise index buffer
	{
		hr = indexBuffer.Initialise(
			devicePtr, 
			contextPtr, 
			indices, 
			ARRAYSIZE(indices), 
			0, 
			D3D11_USAGE_DEFAULT, 
			D3D11_BIND_INDEX_BUFFER
		);
		COM_CHECK_FAIL(hr, "Failed to initialise index buffer on scene initialisation.");
	}

	lookMode = CM_YAW;

	Update();
}

DXBuffer<Vertex> * Mesh::GetVertexBufferPtr()
{
	return &vertexBuffer;
}

DXBuffer<DWORD> * Mesh::GetIndexBufferPtr()
{
	return &indexBuffer;
}

void Mesh::Update()
{
}
