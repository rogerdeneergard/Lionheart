#include "Shaders.h"

bool VertexShader::Initialise(Microsoft::WRL::ComPtr<ID3D11Device> &device, const BYTE * shader, size_t shaderSize,
	D3D11_INPUT_ELEMENT_DESC * layout, UINT layoutSize)
{
	this->shader = shader;
	this->shaderSize = shaderSize;
	HRESULT hr = device->CreateVertexShader(shader,
		shaderSize,
		NULL,
		this->shaderPtr.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLog::Log(hr, L"Failed to create vertex shader on initialisation.");
		return false;
	}

	hr = device->CreateInputLayout(layout, layoutSize, shader, shaderSize, this->inputLayoutPtr.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to create input layout on vertex shader initialisation.");
		return false;
	}

	return true;
}

ID3D11VertexShader * VertexShader::GetShaderPtr()
{
	return this->shaderPtr.Get();
}

const BYTE * VertexShader::GetBufferPtr()
{
	return this->shader;
}

size_t VertexShader::GetBufferSize()
{
	return this->shaderSize;
}

ID3D11InputLayout * VertexShader::GetInputLayoutPtr()
{
	return this->inputLayoutPtr.Get();
}

// Pixel shader logic

bool PixelShader::Initialise(Microsoft::WRL::ComPtr<ID3D11Device> &device, const BYTE * shader, size_t shaderSize)
{
	this->shader = shader;
	this->shaderSize = shaderSize;
	HRESULT hr = device->CreatePixelShader(shader,
		shaderSize,
		NULL,
		this->shaderPtr.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLog::Log(hr, L"Failed to create pixel shader on initialisation.");
		return false;
	}

	return true;
}

ID3D11PixelShader * PixelShader::GetShaderPtr()
{
	return this->shaderPtr.Get();
}

const BYTE * PixelShader::GetBufferPtr()
{
	return this->shader;
}

size_t PixelShader::GetBufferSize()
{
	return this->shaderSize;
}

ID3D11InputLayout * PixelShader::GetInputLayoutPtr()
{
	return this->inputLayoutPtr.Get();
}
