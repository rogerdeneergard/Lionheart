#pragma once

#include "Mesh.h"
#include "Rig.h"

struct SkinWeight;

class MeshRigged : public Mesh
{
public:
	MeshRigged();
	MeshRigged(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr,
		ID3D11ComputeShader * skinShader);
	void Update() override;
private:
	void UpdateVertexBuffer();
	Rig * rigPtr;
	DynamicArray<const SkinWeight> skinWeights;
	DynamicArray<const XMVECTOR> bindPositions;

	ID3D11ComputeShader * skinShader;
};