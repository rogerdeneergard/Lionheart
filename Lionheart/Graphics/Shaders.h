#pragma once
#include "../Util/ErrorLog.h"
#pragma comment(lib, "D3DCompiler.lib")
#include<d3d11.h>
#include<wrl/client.h>
#include<d3dcompiler.h>

class VertexShader
{
public:
	bool Initialise(Microsoft::WRL::ComPtr<ID3D11Device> &device, const BYTE * shader, size_t shaderSize, 
		D3D11_INPUT_ELEMENT_DESC * layout, UINT layoutSize);
	ID3D11VertexShader * GetShaderPtr();
	const BYTE * GetBufferPtr();
	size_t GetBufferSize();
	ID3D11InputLayout * GetInputLayoutPtr();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shaderPtr = nullptr;
	const BYTE * shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayoutPtr;
	size_t shaderSize = 0;
};

class PixelShader
{
public:
	bool Initialise(Microsoft::WRL::ComPtr<ID3D11Device> &device, const BYTE * shader, size_t shaderSize);
	ID3D11PixelShader * GetShaderPtr();
	const BYTE * GetBufferPtr();
	size_t GetBufferSize();
	ID3D11InputLayout * GetInputLayoutPtr();
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shaderPtr = nullptr;
	const BYTE * shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayoutPtr;
	size_t shaderSize = 0;
};

class ComputeShader
{
public:
	bool Initialise(ID3D11Device * devicePtr, const BYTE * shader, size_t shaderSize);
	ID3D11ComputeShader * GetShaderPtr();
	const BYTE * GetBufferPtr();
	size_t GetBufferSize();
	ID3D11InputLayout * GetInputLayoutPtr();
private:
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> shaderPtr = nullptr;
	const BYTE * shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayoutPtr;
	size_t shaderSize = 0;
};