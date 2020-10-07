#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <dxgi.h>
#include <vector>

class AdapterHandler
{
public:
	static std::vector<IDXGIAdapter *> GetAdapters();
};