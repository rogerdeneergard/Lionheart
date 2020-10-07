#include "MeshRigged.h"
#include "../CBufferTypes.h"

struct SkinWeight
{
	Bone * bones[4];
	float weight[4];
};

MeshRigged::MeshRigged() : Mesh() {}

MeshRigged::MeshRigged(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr, 
	ID3D11ComputeShader * skinShader) :
	Mesh(devicePtr, contextPtr), skinShader(skinShader)
{ 
	
}

void MeshRigged::Update()
{
	UpdateVertexBuffer();
}

void MeshRigged::UpdateVertexBuffer()
{
	size_t len = vertexBuffer.BufferSize();
	DynamicArray<CB_CS_Skin> skinData;
	for (UINT i = 0; i < len; i++)
	{
		SkinWeight skinWeightData = skinWeights.Get(i);
		XMMATRIX boneOffsets[4] = {
			skinWeightData.bones[0]->GetOffsetMatrix(),
			skinWeightData.bones[1]->GetOffsetMatrix(),
			skinWeightData.bones[2]->GetOffsetMatrix(),
			skinWeightData.bones[3]->GetOffsetMatrix()
		};
		CB_CS_Skin _skinData;
		_skinData.position = bindPositions.Get(i);
		memcpy(_skinData.boneOffsets, boneOffsets, sizeof(XMMATRIX) * 4);
		_skinData.boneWeights = XMFLOAT4(
			skinWeightData.weight[0],
			skinWeightData.weight[1],
			skinWeightData.weight[2],
			skinWeightData.weight[3]
		);
		skinData.Add(_skinData);
	}

	CB_CS_Skin * dataPtr = nullptr;
	size_t sizeT = 0;

	skinData.LoadData(&dataPtr, &sizeT);

	UINT size = (UINT)sizeT;

	DXBuffer<CB_CS_Skin> skinInput;
	skinInput.Initialise(
		devicePtr, 
		contextPtr, 
		dataPtr, 
		size, 
		16, 
		D3D11_USAGE_DYNAMIC, 
		D3D11_BIND_SHADER_RESOURCE,
		D3D11_CPU_ACCESS_WRITE, 
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);

	ID3D11ShaderResourceView * srvPtr;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = size;

	hr = devicePtr->CreateShaderResourceView(skinInput.Get(), &srvDesc, &srvPtr);
	COM_CHECK_FAIL(hr, "Failed to create shader resource view on skinned mesh update.");

	DXBuffer<XMFLOAT4> output;
	output.Initialise(
		devicePtr, 
		contextPtr, 
		nullptr, 
		size, 
		16, 
		D3D11_USAGE_DEFAULT, 
		D3D11_BIND_UNORDERED_ACCESS, 
		0, 
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED
	);

	DXBuffer<XMFLOAT4> outputCPU;
	outputCPU.Initialise(
		devicePtr,
		contextPtr,
		nullptr,
		size,
		16,
		D3D11_USAGE_STAGING,
		0,
		D3D11_CPU_ACCESS_READ,
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED
	);

	ID3D11UnorderedAccessView * uavPtr;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Buffer.NumElements = size;

	hr = devicePtr->CreateUnorderedAccessView(output.Get(), &uavDesc, &uavPtr);
	COM_CHECK_FAIL(hr, "Failed to create unordered access view on skinned mesh update.");

	contextPtr->CSSetShader(skinShader, nullptr, 0);
	contextPtr->CSSetShaderResources(0, 1, &srvPtr);
	contextPtr->CSSetUnorderedAccessViews(0, 1, &uavPtr, 0);

	contextPtr->Dispatch(8, 8, 1);

	ID3D11ShaderResourceView * nullSRV[] = { NULL };
	contextPtr->CSSetShaderResources(0, 1, nullSRV);
	ID3D11UnorderedAccessView * nullUAV[] = { NULL };
	contextPtr->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
	contextPtr->CSSetShader(nullptr, nullptr, 0);

	contextPtr->CopyResource(outputCPU.Get(), output.Get());

	D3D11_MAPPED_SUBRESOURCE mpsr;
	contextPtr->Map(outputCPU.Get(), 0, D3D11_MAP_READ, 0, &mpsr);

	BYTE * vertexSysMem = new BYTE[outputCPU.BufferSize() * outputCPU.GetStride()];
	memcpy(vertexSysMem, mpsr.pData, sizeof(vertexSysMem));

	Vertex * vertices = new Vertex[size];
	for (UINT i = 0; i < size; i++)
	{
		XMFLOAT4 vertexData;
		memcpy(&vertexData, &vertexSysMem[i*outputCPU.GetStride()], sizeof(XMFLOAT4));
		vertices[i] = { vertexData.x, vertexData.y, vertexData.z, 0.0f, 1.0f };
	}

	CopyMemory(vertexBuffer.GetData(), &vertices, size);
	vertexBuffer.ApplyChanges();
}