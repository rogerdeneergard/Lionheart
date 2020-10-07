#include "GraphicsAdapter.h"
#include "../Util/ErrorLog.h"

std::vector<IDXGIAdapter *> AdapterHandler::GetAdapters()
{
	std::vector<IDXGIAdapter *> adapters;
	if (adapters.size() > 0) return adapters;

	Microsoft::WRL::ComPtr<IDXGIFactory> factoryPtr;

	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)factoryPtr.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to create DXGIFactory for enumerating adapters.");
		exit(-1);
	}

	IDXGIAdapter * adapterPtr;
	UINT index = 0;
	
	while (SUCCEEDED(factoryPtr->EnumAdapters(index, &adapterPtr)))
	{
		adapters.push_back(adapterPtr);
		index++;
	}

	free(adapterPtr);

	return adapters;
}