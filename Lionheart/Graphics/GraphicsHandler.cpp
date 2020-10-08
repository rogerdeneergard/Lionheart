#include "GraphicsHandler.h"
#include "../Util/ErrorLog.h"
#include "../vertexshader.shader"
#include "../pixelshader.shader"
#include "../Util/ComException.h"
#include "Modelling/Mesh.h"

bool GraphicsHandler::Initialise(HWND hWin, int width, int height)
{
	this->width = width;
	this->height = height;
	if (!InitialiseDirect3D(hWin)) return false;
	if (!InitialiseShaders()) return false;
	if (!InitialiseScene()) return false;

	// Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO & io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hWin);
	ImGui_ImplDX11_Init(devicePtr.Get(), contextPtr.Get());
	ImGui::StyleColorsClassic();

	return true;
}

void GraphicsHandler::RenderFrame()
{
	try
	{
		float bgColour[] = { 1.0f, 0.0f, 0.0f, 1.0f }; // Set background colour
		contextPtr->ClearRenderTargetView(renderTargetViewPtr.Get(), bgColour); // Clear render target
		contextPtr->ClearDepthStencilView(depthStencilViewPtr.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		/* Drawing preparation */
		contextPtr->IASetInputLayout(vertexShader.GetInputLayoutPtr()); // Set input layout
		contextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // Set topology
		contextPtr->RSSetState(rasterizerStatePtr.Get()); // Set rasterizer state
		contextPtr->OMSetDepthStencilState(depthStencilStatePtr.Get(), 0); // Set depth stencil state
		contextPtr->OMSetBlendState(blendStatePtr.Get(), NULL, 0xFFFFFFFF);
		contextPtr->OMSetRenderTargets(1, renderTargetViewPtr.GetAddressOf(), depthStencilViewPtr.Get()); // Set render target

		// Shader prep
		contextPtr->VSSetShader(vertexShader.GetShaderPtr(), NULL, 0); // Set vertex shader
		contextPtr->PSSetShader(pixelShader.GetShaderPtr(), NULL, 0); // Set pixel shader
		contextPtr->PSSetSamplers(0, 1, samplerStatePtr.GetAddressOf()); // Set pixel shader sampler

		/* Update constant buffers */
		/*
		XMMATRIX world = XMMatrixIdentity();
		(**vsViewBuffer.GetData()).view = XMMatrixTranspose(world * mainView.GetViewMatrix() * mainView.GetProjectionMatrix());

		hr = vsViewBuffer.ApplyChanges();
		COM_CHECK_FAIL(hr, "Failed to apply changes in vertex shader offset buffer.");

		contextPtr->VSSetConstantBuffers(0, 1, vsViewBuffer.GetAddressOf());

		(**psAlphaBuffer.GetData()).alpha = 1.0f;
		psAlphaBuffer.ApplyChanges();

		hr = psAlphaBuffer.ApplyChanges();
		COM_CHECK_FAIL(hr, "Failed to apply changes in pixel shader alpha buffer.");

		contextPtr->PSSetConstantBuffers(0, 1, psAlphaBuffer.GetAddressOf());
		*/
		/* Drawing */

		// Define vertices

		vsViewBuffer.GetData()->view = XMMatrixTranspose(XMMatrixIdentity() * mainView.GetViewMatrix() * mainView.GetProjectionMatrix());
		vsViewBuffer.ApplyChanges();

		contextPtr->VSSetConstantBuffers(0, 1, vsViewBuffer.GetAddressOf());

		contextPtr->PSSetShaderResources(0, 1, &texturePtr);

		contextPtr->IASetVertexBuffers(
			0,
			1,
			vertexBuffer.GetAddressOf(),
			vertexBuffer.GetStridePtr(),
			vertexBuffer.GetOffsetPtr()
		);

		contextPtr->IASetIndexBuffer(
			indexBuffer.Get(),
			DXGI_FORMAT::DXGI_FORMAT_R32_UINT,
			0
		);

		contextPtr->DrawIndexed(indexBuffer.BufferSize(), 0, 0);

		//model.Update();
		//model.Draw(mainView.GetViewMatrix() * mainView.GetProjectionMatrix());
		//vertexBuffer.ReadToSysMem();
		std::string debug = "View position - x: " + std::to_string(indexBuffer.GetData()[0]);
		debug += " y: " + std::to_string(vertexBuffer.GetData()[0].pos.y);
		debug += " z: " + std::to_string(vertexBuffer.GetData()[0].pos.z);
		debug += "\nView rotation - x: " + std::to_string(mainView.GetRotationFloat3().x);
		debug += " y: " + std::to_string(mainView.GetRotationFloat3().y);
		debug += " z: " + std::to_string(mainView.GetRotationFloat3().z);
		debug += "\nModel position - x: " + std::to_string(model.GetPositionFloat3().x);
		debug += " y: " + std::to_string(model.GetPositionFloat3().y);
		debug += " z: " + std::to_string(model.GetPositionFloat3().z);
		debug += "\nModel rotation - x: " + std::to_string(model.GetRotationFloat3().x);
		debug += " y: " + std::to_string(model.GetRotationFloat3().y);
		debug += " z: " + std::to_string(model.GetRotationFloat3().z);
		debug += "\n\n";
		//OutputDebugStringA(debug.c_str());
		std::string sdebug = "\nafter " + std::to_string(vertexBuffer.GetData()[0].pos.x);
		OutputDebugStringA(sdebug.c_str());
		/* Text */

		/* ImGUI and DXTK sprite testing
		spriteBatchPtr->Begin();

		spriteFontPtr->DrawString(spriteBatchPtr.get(), L"Hello!", DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f,
			DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));

		spriteBatchPtr->End();

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Test");
		ImGui::End();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		*/

		swapchainPtr->Present(0, NULL);
	}
	catch (ComException & exception)
	{
		ErrorLog::Log(exception);
	}
}

bool GraphicsHandler::InitialiseDirect3D(HWND hWin)
{
	try
	{
		// Get list of potential video adapters and choose best based on dedicated vram
		{
			std::vector<IDXGIAdapter *> adapters = AdapterHandler::GetAdapters();

			SIZE_T best = 0;

			for (int i = 0; i < adapters.size(); ++i)
			{
				DXGI_ADAPTER_DESC adapterDesc;
				adapters[i]->GetDesc(&adapterDesc);
				if (adapterDesc.DedicatedVideoMemory > best)
				{
					adapterPtr = adapters[i];
					best = adapterDesc.DedicatedVideoMemory;
				}
			}

			if (adapterPtr == nullptr)
			{
				ErrorLog::Log("No IDXGI adapters found.");
				return false;
			}
		}

		// Define and create device and swapchain
		{
			DXGI_SWAP_CHAIN_DESC scd;
			ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

			scd.BufferDesc.Width = width;
			scd.BufferDesc.Height = height;
			scd.BufferDesc.RefreshRate.Numerator = 60;
			scd.BufferDesc.RefreshRate.Denominator = 1;
			scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			scd.SampleDesc.Count = 1;
			scd.SampleDesc.Quality = 0;

			scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			scd.BufferCount = 1;
			scd.OutputWindow = hWin;
			scd.Windowed = true;
			scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			hr = D3D11CreateDeviceAndSwapChain(adapterPtr.Get(), // DXGI adapter
				D3D_DRIVER_TYPE_UNKNOWN, // D3D driver type
				NULL, // software driver type
				NULL, // flags for runtime layers
				NULL, // feature levels array
				0, // feature levels size
				D3D11_SDK_VERSION, // sdk version
				&scd, // pointer to swapchain description
				swapchainPtr.GetAddressOf(), // swapchain address
				devicePtr.GetAddressOf(), // device address
				NULL, // supported feature level
				contextPtr.GetAddressOf() // device context address
			);
			COM_CHECK_FAIL(hr, "Failed to create device and swapchain on Direct3D initialisation.");
		}
		
		// Create back buffer and render target view
		{
			Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
			hr = swapchainPtr->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(backBuffer.GetAddressOf()));
			COM_CHECK_FAIL(hr, "Failed GetBuffer on swapchain on Direct3D initialisation.");

			hr = devicePtr->CreateRenderTargetView(backBuffer.Get(), NULL, renderTargetViewPtr.GetAddressOf());
			COM_CHECK_FAIL(hr, "Failed to create render target view on Direct3D initialisation."); 
		}

		// Define and create depth stencil buffer, view, and state
		{
			// Define depth stencil buffer
			D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
			ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

			depthStencilBufferDesc.Width = width;
			depthStencilBufferDesc.Height = height;
			depthStencilBufferDesc.MipLevels = 1;
			depthStencilBufferDesc.ArraySize = 1;
			depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilBufferDesc.SampleDesc.Count = 1;
			depthStencilBufferDesc.SampleDesc.Quality = 0;
			depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilBufferDesc.CPUAccessFlags = 0;
			depthStencilBufferDesc.MiscFlags = 0;

			// Create the depth stencil buffer
			hr = devicePtr->CreateTexture2D(&depthStencilBufferDesc, NULL, depthStencilBufferPtr.GetAddressOf());
			COM_CHECK_FAIL(hr, "Failed to create depth stencil buffer on Direct3D initialisation.");

			// Create the depth stencil view
			hr = devicePtr->CreateDepthStencilView(depthStencilBufferPtr.Get(), NULL, depthStencilViewPtr.GetAddressOf());
			COM_CHECK_FAIL(hr, "Failed to create depth stencil view on Direct3D initialisation.");

			// Define depth stencil state
			D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
			ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

			depthStencilDesc.DepthEnable = TRUE;
			depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
			depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

			// Create depth stencil state
			hr = devicePtr->CreateDepthStencilState(&depthStencilDesc, depthStencilStatePtr.GetAddressOf());
			COM_CHECK_FAIL(hr, "Failed to create depth stencil state on Direct3D initialisation.");
		}

		// Define and set viewport
		{
			// Defining viewport
			D3D11_VIEWPORT viewport;
			ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = (FLOAT)width;
			viewport.Height = (FLOAT)height;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;

			// Set the viewport
			contextPtr->RSSetViewports(1, &viewport); 
		}

		// Define and create rasterizer state
		{
			// Define rasterizer state
			D3D11_RASTERIZER_DESC rasterizerDesc;
			ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

			rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

			hr = devicePtr->CreateRasterizerState(&rasterizerDesc, rasterizerStatePtr.GetAddressOf());
			COM_CHECK_FAIL(hr, "Failed to create rasterizer state on Direct3D initialisation.");

			spriteBatchPtr = std::make_unique<DirectX::SpriteBatch>(contextPtr.Get());
			spriteFontPtr = std::make_unique<DirectX::SpriteFont>(devicePtr.Get(), L"Data\\Graphics\\Fonts\\comic_sans_ms_16.spritefont");

			D3D11_SAMPLER_DESC samplerDesc;
			ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			// Create rasterizer state
			hr = devicePtr->CreateSamplerState(&samplerDesc, samplerStatePtr.GetAddressOf());
			COM_CHECK_FAIL(hr, "Failed to create sampler state on Direct3D initialisation.");
		}

		// Define and create blend state and blend state render target
		{
			// Define blend state
			D3D11_BLEND_DESC blendDesc;
			ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

			// Define blend state render target
			D3D11_RENDER_TARGET_BLEND_DESC rtbd;
			ZeroMemory(&rtbd, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));

			rtbd.BlendEnable = true;
			rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
			rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
			rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
			rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

			blendDesc.RenderTarget[0] = rtbd;

			hr = devicePtr->CreateBlendState(&blendDesc, blendStatePtr.GetAddressOf());
			COM_CHECK_FAIL(hr, "Failed to create blend state on Direct3D initialisation.");
		}
	}
	catch (ComException & exception)
	{
		ErrorLog::Log(exception);
		return false;
	}

	return true;
}

bool GraphicsHandler::InitialiseShaders()
{
	// Define layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Get layout size
	UINT layoutSize = ARRAYSIZE(layout);

	// Initialise vertex shader
	if (!vertexShader.Initialise(devicePtr, VS_main, sizeof(VS_main), layout, layoutSize)) return false;

	// Initialise pixel shader
	if (!pixelShader.Initialise(devicePtr, PS_main, sizeof(PS_main))) return false;

	return true;
}

bool GraphicsHandler::InitialiseScene()
{
	try
	{
		// Initialise vertex shader view buffer
		{
			hr = vsViewBuffer.Initialise(
				devicePtr.Get(), 
				contextPtr.Get(), 
				nullptr, 
				1, 
				16, 
				D3D11_USAGE_DYNAMIC, 
				D3D11_BIND_CONSTANT_BUFFER,
				D3D11_CPU_ACCESS_WRITE
			);
			COM_CHECK_FAIL(hr, "Failed to initialise vertex shader offset buffer on scene initialisation.");
		}

		// Initialise pixel shader alpha buffer
		{
			hr = psAlphaBuffer.Initialise(
				devicePtr.Get(), 
				contextPtr.Get(), 
				nullptr, 
				1, 
				16,
				D3D11_USAGE_DYNAMIC,
				D3D11_BIND_CONSTANT_BUFFER,
				D3D11_CPU_ACCESS_WRITE
			);
			COM_CHECK_FAIL(hr, "Failed to initialise pixel shader alpha buffer on scene initialisation.");
		}

		// Create texture 
		{
			hr = DirectX::CreateWICTextureFromFile(devicePtr.Get(), L"Data\\Graphics\\Textures\\grasstexture.png", nullptr,
				texturePtr.GetAddressOf());
			COM_CHECK_FAIL(hr, "Failed to create WIC texture from file on scene initialisation.");
		}

		// Set initial projection and position values for the main view
		{
			mainView.SetPosition(0.0f, 0.0f, -1.0f);
			mainView.SetProjection(75.0f, (float)width / (float)height, 0.1f, 1000.0f);
		}

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
		std::string debug = "\nbefore " + std::to_string(v[0].pos.x);
		// Initialise vertex buffer
		{
			hr = vertexBuffer.Initialise(
				devicePtr.Get(),
				contextPtr.Get(),
				v,
				ARRAYSIZE(v),
				0,
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_VERTEX_BUFFER
			);
			COM_CHECK_FAIL(hr, "Failed to initialise vertex buffer on scene initialisation.");
		}
		debug += "\nafter " + std::to_string(vertexBuffer.GetData()[0].pos.x);
		OutputDebugStringA(debug.c_str());
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
				devicePtr.Get(),
				contextPtr.Get(),
				indices,
				ARRAYSIZE(indices),
				0,
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_INDEX_BUFFER
			);
			COM_CHECK_FAIL(hr, "Failed to initialise index buffer on scene initialisation.");
		}

		Mesh * meshPtr = new Mesh(devicePtr.Get(), contextPtr.Get());
		if(!model.Initialise(devicePtr.Get(), contextPtr.Get(), texturePtr.Get(), vsViewBuffer, meshPtr)) return false;
		model.SetPosition(XMFLOAT3(0.0f, 0.0f, 5.0f), true);
	}
	catch (ComException &exception)
	{
		ErrorLog::Log(exception);
		return false;
	}

	return true;
}
