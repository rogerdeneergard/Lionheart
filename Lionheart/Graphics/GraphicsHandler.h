#pragma once
#include "GraphicsAdapter.h"
#include "Shaders.h"
#include "Vertex.h"
#include "DXBuffer.h"
#include "DXCBuffer.h"
#include "CBufferTypes.h"
#include "View.h"
#include "Modelling/Model.h"

// DearImGui Includes
#include "../DearImGui/imgui.h"
#include "../DearImGui/imgui_impl_win32.h"
#include "../DearImGui/imgui_impl_dx11.h"

// DXTK includes
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>

class GraphicsHandler
{
public:
	bool Initialise(HWND hWin, int width, int height);
	void RenderFrame();
	float mouseX;
	float mouseY;

	View mainView;

	Model model;

private:
	HRESULT hr;

	bool InitialiseDirect3D(HWND hWin);
	bool InitialiseShaders();
	bool InitialiseScene();

	Microsoft::WRL::ComPtr<ID3D11Device> devicePtr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> contextPtr;

	Microsoft::WRL::ComPtr<IDXGIAdapter> adapterPtr;

	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchainPtr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetViewPtr;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStatePtr;

	VertexShader vertexShader;
	PixelShader pixelShader;

	DXBuffer<CB_VS_View> vsViewBuffer;
	DXBuffer<CB_PS_Alpha> psAlphaBuffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState> blendStatePtr;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilViewPtr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBufferPtr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStatePtr;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatchPtr;
	std::unique_ptr<DirectX::SpriteFont> spriteFontPtr;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStatePtr;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texturePtr;

	UINT width, height;
};