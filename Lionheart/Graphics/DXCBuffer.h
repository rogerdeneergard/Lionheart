#pragma once

#include "DXBuffer.h"
#include "CBufferTypes.h"

template<class Type>
class DXCBuffer
{
public:
	DXCBuffer() {}
	Type data;

	ID3D11Buffer * Get() const
	{
		return this->bufferPtr.Get();
	}

	ID3D11Buffer * const * GetAddressOf() const
	{
		return this->bufferPtr.GetAddressOf();
	}

	HRESULT Initialise(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr, UINT align)
	{
		if (bufferPtr.Get() != nullptr) bufferPtr.Reset();

		this->contextPtr = contextPtr;

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Type) + (align - (sizeof(Type) % align)));
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		HRESULT hr = devicePtr->CreateBuffer(&bufferDesc, 0, bufferPtr.GetAddressOf());

		return hr;
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> bufferPtr;
	ID3D11DeviceContext * contextPtr = nullptr;
};