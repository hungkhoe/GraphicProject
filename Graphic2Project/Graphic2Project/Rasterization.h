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

#include "Trivial_VS.csh"
#include "Trivial_PS.csh"


using namespace std;
using namespace DirectX;
// variable

CComPtr <ID3D11Device> device;
CComPtr <IDXGISwapChain> chain;
CComPtr <ID3D11RenderTargetView> RTV;
CComPtr <ID3D11DeviceContext> context;
D3D11_VIEWPORT viewport;
CComPtr <ID3D11Texture2D> pBackBuffer;

ID3D11VertexShader* vertexShader;
ID3D11PixelShader* pixelShader;

unsigned int num_verts;
ID3D11Buffer* buffer;
ID3D11InputLayout* layout;

ID3D11Buffer * constant_buffers;

ID3D11Buffer * cubeBuffer;
ID3D11Buffer * indexBuffer;

ID3D11Buffer * worldMatrixBuffer;
ID3D11Buffer * viewMatrixBuffer;
ID3D11Buffer * projectMatrixBuffer;

XMMATRIX worldMatrix;
XMMATRIX viewMatrix;
XMMATRIX projectMatrix;

XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

//struct
struct Vert
{
	XMFLOAT4 coordination;	
	XMFLOAT2 uv;
	XMFLOAT4 color;
//	XMFLOAT4 normals;
};

struct WorldMatrix
{
	XMMATRIX worldMatrix;
	XMFLOAT4 meshColor;
};	

struct ViewMatrix
{
	XMMATRIX viewMatrix;
	
};

struct ProjectionMatrix
{
	XMMATRIX projectMatrix;
	
};


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


	device->CreateRenderTargetView(pBackBuffer, NULL,
		&RTV.p);


	viewport.Width = rect.right - rect.left;
	viewport.Height = rect.bottom - rect.top;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;	

	context->OMSetRenderTargets(1, &RTV.p, NULL);
	context->RSSetViewports(1, &viewport);
	
	Vert cube[] =
	{
		{ XMFLOAT4(-1.0f, 1.0f, -1.0f,1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, -1.0f,1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, 1.0f,1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, 1.0f,1.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT4(-1.0f, -1.0f, -1.0f,1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, -1.0f, -1.0f,1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT4(1.0f, -1.0f, 1.0f,1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT4(-1.0f, -1.0f, 1.0f,1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT4(-1.0f, -1.0f, 1.0f,1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f,1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, -1.0f,1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, 1.0f,1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT4(1.0f, -1.0f, 1.0f,1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT4(1.0f, -1.0f, -1.0f,1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT4(1.0f, 1.0f, -1.0f,1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, 1.0f,1.0f), XMFLOAT2(1.0f, 0.0f) },

		{ XMFLOAT4(-1.0f, -1.0f, -1.0f,1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT4(1.0f, -1.0f, -1.0f,1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT4(1.0f, 1.0f, -1.0f,1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, -1.0f,1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT4(-1.0f, -1.0f, 1.0f,1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT4(1.0f, -1.0f, 1.0f,1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT4(1.0f, 1.0f, 1.0f,1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, 1.0f,1.0f), XMFLOAT2(1.0f, 0.0f) }
	};


	D3D11_BUFFER_DESC bufferDesc1;
	ZeroMemory(&bufferDesc1, sizeof(bufferDesc1));
	bufferDesc1.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc1.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc1.CPUAccessFlags = NULL;
	bufferDesc1.ByteWidth = sizeof(Vert) * 24;


	D3D11_SUBRESOURCE_DATA InitData1;
	ZeroMemory(&InitData1, sizeof(InitData1));
	InitData1.pSysMem = cube;	

	device->CreateBuffer(&bufferDesc1, &InitData1, &cubeBuffer);


	UINT32 index[]=
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};

	bufferDesc1.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc1.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc1.CPUAccessFlags = NULL;
	bufferDesc1.ByteWidth = sizeof(UINT32) * 36;
	InitData1.pSysMem = index;

	device->CreateBuffer(&bufferDesc1, &InitData1, &indexBuffer)
	
	device->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &vertexShader);
	device->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &pixelShader);
	
	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	device->CreateInputLayout(vLayout, 3, Trivial_VS, sizeof(Trivial_VS), &layout);
	context->IASetInputLayout(layout);
	
	bufferDesc1.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc1.ByteWidth = sizeof(ViewMatrix);
	bufferDesc1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc1.CPUAccessFlags = 0;

	device->CreateBuffer(&bufferDesc1, NULL, &viewMatrixBuffer);

	bufferDesc1.ByteWidth = sizeof(ProjectionMatrix);
	device->CreateBuffer(&bufferDesc1, NULL, &projectMatrixBuffer);

	bufferDesc1.ByteWidth = sizeof(WorldMatrix);
	device->CreateBuffer(&bufferDesc1, NULL, &worldMatrixBuffer);
	
	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixLookAtLH(Eye, At, Up);

	ViewMatrix viewMatrixHolder;

	viewMatrixHolder.viewMatrix = XMMatrixTranspose(viewMatrix);
	context->UpdateSubresource(viewMatrixBuffer, 0, nullptr, &viewMatrixHolder, 0, 0);

	projectMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

	ProjectionMatrix projectionHolder;
	projectionHolder.projectMatrix= XMMatrixTranspose(projectMatrix);
	context->UpdateSubresource(projectMatrixBuffer, 0, nullptr, &projectionHolder, 0, 0);

}

void Render()
{
	context->ClearRenderTargetView(RTV, Colors::DarkBlue);

	context->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	WorldMatrix send_to_vramWorldMatrix;
	send_to_vramWorldMatrix.worldMatrix = worldMatrix;
	context->UpdateSubresource(worldMatrixBuffer, 0, nullptr, &send_to_vramWorldMatrix, 0, 0);


	g_pImmediateContext->VSSetShader(vertexShader, nullptr, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pCBNeverChanges);
	g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize);
	g_pImmediateContext->VSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
	g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
	g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
	g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTextureRV);
	g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
	g_pImmediateContext->DrawIndexed(36, 0, 0);

	chain->Present(0, 0);
}

void Shutdown()
{
	
}