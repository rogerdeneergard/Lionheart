#include "Model.h"

Model::Model() : GraphicsObject() {}

Model::Model(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr) :
	GraphicsObject(devicePtr, contextPtr)
{
	Update();
}

bool Model::Initialise(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr, ID3D11ShaderResourceView * texturePtr,
	DXBuffer<CB_VS_View> &vsViewBuffer, Mesh * meshPtr)
{
	this->devicePtr = devicePtr;
	this->contextPtr = contextPtr;
	this->texturePtr = texturePtr;
	this->vsViewBufferPtr = &vsViewBuffer;
	this->meshPtr = meshPtr;

	try
	{
		
	}
	catch (ComException & exception)
	{
		ErrorLog::Log(exception);
		return false;
	}
	return true;
}

void Model::SetTexture(ID3D11ShaderResourceView * texturePtr)
{
	this->texturePtr = texturePtr;
}

void Model::Draw(const XMMATRIX & viewProjectionMatrix)
{

	OutputDebugStringA("\n");
	OutputDebugStringA(std::to_string(meshPtr->GetIndexBufferPtr()->BufferSize()).c_str());

	vsViewBufferPtr->GetData()->view = XMMatrixTranspose(worldMatrix * viewProjectionMatrix);
	vsViewBufferPtr->ApplyChanges();

	contextPtr->VSSetConstantBuffers(0, 1, vsViewBufferPtr->GetAddressOf());

	contextPtr->PSSetShaderResources(0, 1, &texturePtr);

	contextPtr->IASetVertexBuffers(
		0,
		1,
		meshPtr->GetVertexBufferPtr()->GetAddressOf(),
		meshPtr->GetVertexBufferPtr()->GetStridePtr(),
		meshPtr->GetVertexBufferPtr()->GetOffsetPtr()
	);

	contextPtr->IASetIndexBuffer(
		meshPtr->GetIndexBufferPtr()->Get(), 
		DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 
		0
	);

	contextPtr->DrawIndexed(meshPtr->GetIndexBufferPtr()->BufferSize(), 0, 0);
}

Mesh * Model::GetMeshPtr()
{
	return meshPtr;
}

void Model::Update()
{
	UpdateWorldMatrix();
}
