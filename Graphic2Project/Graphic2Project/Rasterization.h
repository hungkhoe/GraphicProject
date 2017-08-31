#include <iostream>
#include <ctime>
#include <d3d11.h>
#include<math.h>
#pragma comment(lib, "d3d11.lib")
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <DirectXPackedVector.h>
#include <DirectXPackedVector.h>
#include<atlbase.h>

using namespace std;
using namespace DirectX;
// variable

CComPtr <ID3D11Device> device;
CComPtr <IDXGISwapChain> chain;
CComPtr <ID3D11RenderTargetView> RTV;
CComPtr <ID3D11DeviceContext> context;
D3D11_VIEWPORT viewport;
CComPtr <ID3D11Texture2D> pBackBuffer;


void SetUpBuffer(HWND window)
{
	RECT rect;
	GetClientRect(window,&rect);
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = rect.right - rect.left;
	sd.BufferDesc.Height = rect.bottom - rect.top;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = window;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL FeatureLevels[4] = { D3D_FEATURE_LEVEL_10_0,D3D_FEATURE_LEVEL_10_1,D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_11_1 };
	HRESULT hr = S_OK;
	D3D_FEATURE_LEVEL FeatureLevel;
	UINT flags = 0;

#if _DEBUG
	flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flags,
		FeatureLevels,
		4,
		D3D11_SDK_VERSION,
		&sd,
		&chain,
		&device,
		&FeatureLevel,
		&context);

	chain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer.p);

	// Create a render-target view
	device->CreateRenderTargetView(pBackBuffer, NULL,
		&RTV.p);

	// Bind the view


	// TODO: PART 1 STEP 5

	viewport.Width = rect.right - rect.left;
	viewport.Height = rect.bottom - rect.top;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;	

	context->OMSetRenderTargets(1, &RTV.p, NULL);
	context->RSSetViewports(1, &viewport);
	
}

void Render()
{
	//// Just clear the backbuffer
	context->ClearRenderTargetView(RTV, Colors::DarkBlue);
	chain->Present(0, 0);
}

void Shutdown()
{
	
}