#pragma once
#include<Windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include "../Util/ErrorLog.h"

template<class Type>
class DXBuffer
{
public:
	DXBuffer() {}

	Type * GetData()
	{
		return dataPtr;
	}

	ID3D11Buffer * Get() const
	{
		return this->bufferPtr.Get();
	}

	ID3D11Buffer * const * GetAddressOf() const
	{
		return this->bufferPtr.GetAddressOf();
	}

	UINT BufferSize() const
	{
		return this->bufferSize;
	}

	const UINT * GetStridePtr() const
	{
		return &stride;
	}

	UINT GetStride() const
	{
		return stride;
	}

	const UINT * GetOffsetPtr() const
	{
		return &offset;
	}

	UINT GetOffset() const
	{
		return offset;
	}

	ULONG Release() const
	{
		bufferPtr->Release();
		free(dataPtr);
		return 0;
	}

	HRESULT Initialise(ID3D11Device * devicePtr, ID3D11DeviceContext * contextPtr, Type * dataPtr, UINT size, UINT align = 0,
		UINT usage = D3D11_USAGE_DEFAULT, UINT bindFlags = D3D11_BIND_VERTEX_BUFFER, UINT cpuAccessFlags = 0, UINT miscFlags = 0)
	{
		this->devicePtr = devicePtr;
		this->contextPtr = contextPtr;

		if (bufferPtr.Get() != nullptr) bufferPtr.Reset();

		bufferSize = size;

		if (align == 0 || sizeof(Type) % align == 0) stride = sizeof(Type);
		else stride = static_cast<UINT>(sizeof(Type) + (align - (sizeof(Type) % align)));

		this->dataPtr = (Type *) new BYTE[stride * size];

		if (dataPtr != nullptr)
		{
			CopyMemory(this->dataPtr, dataPtr, stride * size);
		}

		offset = 0;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

		bufferDesc.Usage = (D3D11_USAGE)usage;
		bufferDesc.ByteWidth = stride * size;
		bufferDesc.BindFlags = (D3D11_BIND_FLAG) bindFlags;
		bufferDesc.CPUAccessFlags = (D3D11_CPU_ACCESS_FLAG) cpuAccessFlags;
		bufferDesc.MiscFlags = (D3D11_RESOURCE_MISC_FLAG) miscFlags;
		if (miscFlags == D3D11_RESOURCE_MISC_BUFFER_STRUCTURED) bufferDesc.StructureByteStride = stride;
		else if (bindFlags == D3D11_BIND_CONSTANT_BUFFER) bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		D3D11_SUBRESOURCE_DATA * bufferDataPtr = &bufferData;
		if (dataPtr != nullptr)
		{
			ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));

			bufferData.pSysMem = this->dataPtr;
		}
		else bufferDataPtr = 0;

		HRESULT hr = devicePtr->CreateBuffer(&bufferDesc, bufferDataPtr, bufferPtr.GetAddressOf());

		return hr;
	}

	HRESULT ApplyChanges()
	{
		try
		{
			if (contextPtr == nullptr) COM_CHECK_FAIL(E_FAIL, "Attempted to write to DXBuffer with no device context.");

			D3D11_MAPPED_SUBRESOURCE mpsr;
			hr = contextPtr->Map(bufferPtr.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mpsr);
			COM_CHECK_FAIL(hr, "Failed to map a subresource to write to DXBuffer.");

			CopyMemory(mpsr.pData, dataPtr, stride * bufferSize);
			contextPtr->Unmap(bufferPtr.Get(), 0);

			return hr;
		}
		catch (ComException &exception)
		{
			ErrorLog::Log(exception);
			return hr;
		}
	}

	HRESULT ReadToSysMem()
	{
		try
		{
			if (contextPtr == nullptr) COM_CHECK_FAIL(E_FAIL, "Attempted to read from DXBuffer with no device context.");

			D3D11_MAPPED_SUBRESOURCE mpsr;
			hr = contextPtr->Map(bufferPtr.Get(), 0, D3D11_MAP_READ, 0, &mpsr);
			COM_CHECK_FAIL(hr, "Failed to map a subresource to read from DXBuffer.");

			CopyMemory(dataPtr, mpsr.pData, stride * bufferSize);
			contextPtr->Unmap(bufferPtr.Get(), 0);

			return hr;
		}
		catch (ComException &exception)
		{
			ErrorLog::Log(exception);
			return hr;
		}
	}

private:
	DXBuffer(const DXBuffer<Type> &source);
	Microsoft::WRL::ComPtr<ID3D11Buffer> bufferPtr;
	ID3D11Device * devicePtr = nullptr;
	ID3D11DeviceContext * contextPtr = nullptr;
	UINT stride;
	UINT offset;
	Type * dataPtr;
	UINT bufferSize = 0;

	HRESULT hr;
};