#include "define.h"
#include "ObjLoader.h"
// variable

HWND hWnd;

CComPtr<ID3D11Device> device;
CComPtr<IDXGISwapChain> swapChain;
ID3D11RenderTargetView*	RTV;
CComPtr<ID3D11DeviceContext> deviceContext;
D3D11_VIEWPORT ViewPort;

ID3D11InputLayout* cubeInputLayout;

ID3D11Resource*	backBuffer;

//cube stuff
ID3D11Buffer* cubeBuffer;
ID3D11Buffer* cubeIndexBuffer;
ID3D11Buffer* cubeConstantBuffer;

ID3D11VertexShader* cubeVertexShader;
ID3D11PixelShader*	cubePixelShader;

//fighter stuff
ID3D11Buffer * fighterBuffer;
ID3D11Buffer * fighterIndexBuffer;
ID3D11Buffer * fighterConstantBuffer;

//
ID3D11Buffer * lightBuffer;

XMMATRIX CubeWorldMatrix;
XMMATRIX ViewMatrix;
XMMATRIX ProjectionMatrix;

XMMATRIX FighterWorldMatrix;

//matrix
XMVECTOR Eye = XMVectorSet(0.0f, 1.5f, -5.0f, 0.0f);
XMVECTOR Focus = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR ResetEye = Eye;

XMFLOAT4 m_MeshColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
XMFLOAT4 black = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

Light directionalLight;

//texture
ID3D11Texture2D * texture2D;
//zbuffer
ID3D11DepthStencilView*	depthStencil;

ID3D11SamplerState*	samplerState;

ID3D11ShaderResourceView * cubeTextureResources;

ID3D11ShaderResourceView * fighterTextureResources;



//Model
Object fighter;
void DrawFighter();
void InitFighterObject();
void BoxInit();
void DrawBox();
void LightInit();
void MapDirectionalLight();

void CameraControl()
{
if (GetAsyncKeyState(VK_F1)) {
	ViewMatrix = XMMatrixLookAtLH(ResetEye, Focus, Up); //reset
}

if (GetAsyncKeyState(VK_SHIFT)) {
	XMMATRIX translate = XMMatrixTranslation(0, 0, -0.001f); //zoom
	ViewMatrix = XMMatrixMultiply(ViewMatrix, translate);
}

if (GetAsyncKeyState('W')) {
	XMMATRIX translate = XMMatrixTranslation(0, -0.001f, 0); // move up
	ViewMatrix = XMMatrixMultiply(ViewMatrix, translate);
}
if (GetAsyncKeyState('S')) {
	XMMATRIX translate = XMMatrixTranslation(0, 0.001f, 0); // move down
	ViewMatrix = XMMatrixMultiply(ViewMatrix, translate);
}
if (GetAsyncKeyState('D')) {
	XMMATRIX translate = XMMatrixTranslation(-0.001f, 0, 0); // move right
	ViewMatrix = XMMatrixMultiply(ViewMatrix, translate);
}
if (GetAsyncKeyState('A')) {
	XMMATRIX translate = XMMatrixTranslation(0.001f, 0, 0); // move left
	ViewMatrix = XMMatrixMultiply(ViewMatrix, translate);
}

if (GetAsyncKeyState(VK_CONTROL)) {
	XMMATRIX translate = XMMatrixTranslation(0, 0, 0.001f); //zoom out
	ViewMatrix = XMMatrixMultiply(ViewMatrix, translate);
}

if (GetAsyncKeyState(VK_UP)) {
	XMMATRIX rotation = XMMatrixRotationX(0.0001);
	ViewMatrix = XMMatrixMultiply(ViewMatrix, rotation); // rotate up
}
if (GetAsyncKeyState(VK_DOWN)) {
	XMMATRIX rotation = XMMatrixRotationX(-0.0001);
	ViewMatrix = XMMatrixMultiply(ViewMatrix, rotation); // rotate down
}
if (GetAsyncKeyState(VK_LEFT)) {
	XMMATRIX rotation = XMMatrixRotationY(0.0001);
	ViewMatrix = XMMatrixMultiply(ViewMatrix, rotation); // rotate left
}
if (GetAsyncKeyState(VK_RIGHT)) {
	XMMATRIX rotation = XMMatrixRotationY(-0.0001);
	ViewMatrix = XMMatrixMultiply(ViewMatrix, rotation); // rotate right
}
}

HRESULT SetUpBuffer() {

	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right;
	UINT height = rc.bottom;

	DXGI_SWAP_CHAIN_DESC swapdesc;
	ZeroMemory(&swapdesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapdesc.BufferCount = 1;
	swapdesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapdesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapdesc.OutputWindow = hWnd;
	swapdesc.SampleDesc.Count = 1;
	swapdesc.SampleDesc.Quality = 0;
	swapdesc.Windowed = true;

	D3D_FEATURE_LEVEL FeatureLevels[4] = { D3D_FEATURE_LEVEL_10_0,D3D_FEATURE_LEVEL_10_1,D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_11_1 };
	D3D_FEATURE_LEVEL FeatureLevel;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, FeatureLevels,
		4, D3D11_SDK_VERSION, &swapdesc, &swapChain, &device, &FeatureLevel, &deviceContext);

	swapChain->GetBuffer(NULL, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer));
	device->CreateRenderTargetView(backBuffer, NULL, &RTV);	

	// zbuffer texture
	D3D11_TEXTURE2D_DESC texturedesc;
	ZeroMemory(&texturedesc, sizeof(texturedesc));
	texturedesc.Width = width;
	texturedesc.Height = height;
	texturedesc.MipLevels = 1;
	texturedesc.ArraySize = 1;
	texturedesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texturedesc.SampleDesc.Count = 1;
	texturedesc.SampleDesc.Quality = 0;
	texturedesc.Usage = D3D11_USAGE_DEFAULT;
	texturedesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texturedesc.CPUAccessFlags = 0;
	texturedesc.MiscFlags = 0;
	device->CreateTexture2D(&texturedesc, NULL, &texture2D);

	// zbuffer view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = texturedesc.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	device->CreateDepthStencilView(texture2D, &descDSV, &depthStencil);


	// viewport
	ViewPort.Width = static_cast<float>(width);
	ViewPort.Height = static_cast<float>(height);
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;	

	// texture loading
	CreateDDSTextureFromFile(device, L"greendragon.dds", NULL, &cubeTextureResources);
	CreateDDSTextureFromFile(device, L"SM_ch_E_Male_Body_Kyoshi.dds", NULL, &fighterTextureResources);

	//sampler state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampDesc, &samplerState);

	//create shader
	device->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &cubeVertexShader);
	device->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &cubePixelShader);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	
	UINT nums = ARRAYSIZE(layout);

	
	BoxInit();
	InitFighterObject();
	LightInit();

	// input layout
	device->CreateInputLayout(layout, nums, Trivial_VS, sizeof(Trivial_VS), &cubeInputLayout);

	//cube world matrix
	CubeWorldMatrix = XMMatrixIdentity();

	//fighter world matrix
	FighterWorldMatrix = XMMatrixIdentity();

	XMMATRIX translate = XMMatrixTranslation(-5.0f, 0, 0); // move right
	XMMATRIX scale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	//XMMATRIX rotate = XMMatrixRotationX(1.0f);

	FighterWorldMatrix = XMMatrixMultiply(FighterWorldMatrix, translate);
	FighterWorldMatrix = XMMatrixMultiply(FighterWorldMatrix, scale);
	//FighterWorldMatrix = XMMatrixMultiply(FighterWorldMatrix, rotate);

	

	// view matrix
	ViewMatrix = XMMatrixLookAtLH(Eye, Focus, Up);

	//projection matrix
	ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / static_cast<float>(height), 0.01f, 100.0f);

	return S_OK;
}


//----------------------------------------------------------------------------------------------------------
// Constant Frame Rendering
//----------------------------------------------------------------------------------------------------------
bool Render() {	

	// Time Per Frame
	static float t = 0.0f;
	static ULONGLONG timeStart = 0;
	ULONGLONG timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;
	t = (timeCur - timeStart) / 1000.0f;
		
	CameraControl();

	deviceContext->OMSetRenderTargets(1, &RTV, depthStencil);

	deviceContext->RSSetViewports(1, &ViewPort);

	//rotation for cube
	CubeWorldMatrix = XMMatrixRotationY(-t);

	//rotation for fighter
    FighterWorldMatrix = XMMatrixMultiply(XMMatrixRotationY(0.0001f),FighterWorldMatrix);

	deviceContext->ClearRenderTargetView(RTV, Colors::DarkCyan);

	deviceContext->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	//send cube matrix to vram
	Matrix send_matrix_to_vram;
	send_matrix_to_vram.World = XMMatrixTranspose(CubeWorldMatrix);
	send_matrix_to_vram.View = XMMatrixTranspose(ViewMatrix);
	send_matrix_to_vram.Projection = XMMatrixTranspose(ProjectionMatrix);
	
	//send fighter matrix to vram
	Matrix send_fighter_to_vram;
	send_fighter_to_vram.World = XMMatrixTranspose(FighterWorldMatrix);
	send_fighter_to_vram.View = XMMatrixTranspose(ViewMatrix);
	send_fighter_to_vram.Projection = XMMatrixTranspose(ProjectionMatrix);

	Light send_directional_light_to_vram;
	send_directional_light_to_vram.color = directionalLight.color;
	send_directional_light_to_vram.direction = directionalLight.direction;

	//MapDirectionalLight();
	deviceContext->PSSetSamplers(0, 1, &samplerState);	
	//Draw
	DrawBox();
	DrawFighter();

	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);

	deviceContext->UpdateSubresource(cubeConstantBuffer, 0, NULL, &send_matrix_to_vram, 0, 0);

	deviceContext->UpdateSubresource(fighterConstantBuffer, 0, NULL, &send_fighter_to_vram, 0, 0);

	deviceContext->UpdateSubresource(lightBuffer, 0, NULL, &send_directional_light_to_vram, 0, 0);

	swapChain->Present(0, 0);

	return true;
}

void Shutdown()
{
	RTV->Release();
	backBuffer->Release();
	

	cubeInputLayout->Release(); 
	
	cubeVertexShader->Release(); 
	cubePixelShader->Release();

	fighterBuffer->Release();
	fighterIndexBuffer->Release();
	fighterConstantBuffer->Release();

	cubeBuffer->Release();
	cubeIndexBuffer->Release();
	cubeConstantBuffer->Release();

	lightBuffer->Release();

	texture2D->Release();
	depthStencil->Release();
	samplerState->Release();

	cubeTextureResources->Release();
	fighterTextureResources->Release();	
}

void BoxInit()
{
	Vert cube[] = {

		// top face
		{ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),		XMFLOAT2(1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),		XMFLOAT2(0.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),			XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),		XMFLOAT2(0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),		XMFLOAT2(1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },

		//bottom
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(0.0f, 0.0f),XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(1.0f, 0.0f),XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(1.0f, 1.0f),XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(0.0f, 1.0f),XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
																												
		// right
		{ XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(0.0f, 1.0f), XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(1.0f, 1.0f), XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(1.0f, 0.0f), XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(0.0f, 0.0f), XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },

		//left
		{ XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(1.0f, 1.0f),XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(0.0f, 1.0f),XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(0.0f, 0.0f),XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),			XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(1.0f, 0.0f),XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },

		//front
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(0.0f, 1.0f),XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(1.0f, 1.0f),XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(1.0f, 0.0f),XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(0.0f, 0.0f),XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },

		//back

		{ XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(0.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),			XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),		XMFLOAT2(1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
	};

	// Creating Index
	UINT32 Indexes[] = {
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

	// cube buffer
	D3D11_BUFFER_DESC buffdesc;
	ZeroMemory(&buffdesc, sizeof(D3D11_BUFFER_DESC));
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.ByteWidth = sizeof(Vert) * 24;

	// Initializing SubSource
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = cube;

	// Creating Vertex Buffer
	device->CreateBuffer(&buffdesc, &data, &cubeBuffer);

	// index buffer
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.ByteWidth = sizeof(UINT32) * 36;
	data.pSysMem = Indexes;
	device->CreateBuffer(&buffdesc, &data, &cubeIndexBuffer);

	// constant
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.ByteWidth = sizeof(Matrix);
	device->CreateBuffer(&buffdesc, nullptr, &cubeConstantBuffer);	
}
void DrawBox()
{
	unsigned int	strides = sizeof(Vert);
	unsigned int	offsets = 0;

	deviceContext->IASetVertexBuffers(0, 1, &cubeBuffer, &strides, &offsets);		

	deviceContext->VSSetShader(cubeVertexShader, NULL, 0);

	deviceContext->PSSetShader(cubePixelShader, NULL, 0);

	deviceContext->PSSetShaderResources(0, 1, &cubeTextureResources);

	deviceContext->IASetInputLayout(cubeInputLayout);

	deviceContext->IASetIndexBuffer(cubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetConstantBuffers(0, 1, &cubeConstantBuffer);

	deviceContext->DrawIndexed(36, 0, 0);
}

void InitFighterObject()
{
	Object tempModel;
	tempModel.Load("FighterObject.obj");	
	fighter = tempModel;

	Vert * vertices = new Vert[fighter.GetModel().size()];
	unsigned int count = fighter.GetIndex().size();
	unsigned int * indicies = new unsigned int[count];

	for (unsigned int i = 0; i < fighter.GetModel().size(); i++)
		vertices[i] = fighter.GetModel()[i];

	for (unsigned int i = 0; i < fighter.GetIndex().size(); i++)
		indicies[i] = fighter.GetIndex()[i];	

	D3D11_BUFFER_DESC fighterBufferDesc;

	ZeroMemory(&fighterBufferDesc, sizeof(D3D11_BUFFER_DESC));
	fighterBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	fighterBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	fighterBufferDesc.CPUAccessFlags = NULL;
	fighterBufferDesc.ByteWidth = sizeof(Vert) * count;

	D3D11_SUBRESOURCE_DATA fighterSubResources;
	fighterSubResources.pSysMem = vertices;
	fighterSubResources.SysMemPitch = NULL;
	fighterSubResources.SysMemSlicePitch = NULL;
	
	device->CreateBuffer(&fighterBufferDesc, &fighterSubResources, &fighterBuffer);	

	D3D11_BUFFER_DESC fighterBufferDesc1;
	ZeroMemory(&fighterBufferDesc1, sizeof(D3D11_BUFFER_DESC));	

	fighterBufferDesc1.Usage = D3D11_USAGE_DEFAULT;
	fighterBufferDesc1.BindFlags = D3D11_BIND_INDEX_BUFFER;
	fighterBufferDesc1.CPUAccessFlags = NULL;
	fighterBufferDesc1.ByteWidth = sizeof(unsigned int) * count;

	fighterSubResources.pSysMem = indicies;

	device->CreateBuffer(&fighterBufferDesc1, &fighterSubResources, &fighterIndexBuffer);
	
	D3D11_BUFFER_DESC fighterBufferDesc2;
	ZeroMemory(&fighterBufferDesc2, sizeof(D3D11_BUFFER_DESC));
	fighterBufferDesc2.Usage = D3D11_USAGE_DEFAULT;
	fighterBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	fighterBufferDesc2.ByteWidth = sizeof(Matrix);
	

	device->CreateBuffer(&fighterBufferDesc2, nullptr, &fighterConstantBuffer);

	delete[] vertices;
	delete[] indicies;
}

void DrawFighter()	
{
	unsigned int	strides = sizeof(Vert);
	unsigned int	offsets = 0;

	deviceContext->VSSetShader(cubeVertexShader, NULL, 0);

	deviceContext->PSSetShader(cubePixelShader, NULL, 0);

	deviceContext->IASetVertexBuffers(0, 1, &fighterBuffer, &strides, &offsets);

	deviceContext->PSSetShaderResources(0, 1, &fighterTextureResources);	

	deviceContext->IASetIndexBuffer(fighterIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetInputLayout(cubeInputLayout);
	
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetConstantBuffers(0, 1, &fighterConstantBuffer);	

	deviceContext->DrawIndexed(fighter.GetIndex().size(), 0, 0);
}

void LightInit()
{
	//directional light init value

	directionalLight.color.x = 1.0f;
	directionalLight.color.y = 1.0f;
	directionalLight.color.z = 1.0f;
	directionalLight.color.w = 0.0f;

	directionalLight.direction.x = 1.0f;
	directionalLight.direction.y = 0.0f;
	directionalLight.direction.z = 0.0f;

	//directionalLight.padding = 1.0f;

	D3D11_BUFFER_DESC ligthConstbuffdesc;

	ZeroMemory(&ligthConstbuffdesc, sizeof(D3D11_BUFFER_DESC));
	ligthConstbuffdesc.Usage = D3D11_USAGE_DEFAULT;
	ligthConstbuffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	ligthConstbuffdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ligthConstbuffdesc.ByteWidth = sizeof(Light);

	device->CreateBuffer(&ligthConstbuffdesc, nullptr, &lightBuffer);	
}

void MapDirectionalLight()
{
	D3D11_MAPPED_SUBRESOURCE mapSubResource;
	deviceContext->Map(lightBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubResource);
	memcpy(mapSubResource.pData, &directionalLight, sizeof(Light));
	deviceContext->Unmap(lightBuffer, NULL);
}