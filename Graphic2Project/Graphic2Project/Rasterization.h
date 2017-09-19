#include "define.h"
#include "ObjLoader.h"
// variable

HWND hWnd;

CComPtr<ID3D11Device> device;
CComPtr<IDXGISwapChain> swapChain;
ID3D11RenderTargetView*	RTV;
CComPtr<ID3D11DeviceContext> deviceContext;
D3D11_VIEWPORT ViewPort;

//input layouyt
ID3D11InputLayout* cubeInputLayout;
ID3D11InputLayout* SkyBoxInputLayout;

//back buffer
ID3D11Resource*	backBuffer;

//cube buffer
ID3D11Buffer* cubeBuffer;
ID3D11Buffer* cubeIndexBuffer;
ID3D11Buffer* cubeConstantBuffer;

//skybox buffer
ID3D11Buffer* skyboxBuffer;
ID3D11Buffer* skyboxIndexBuffer;
ID3D11Buffer* skyboxConstantBuffer;

//quad buffer
ID3D11Buffer* quadBuffer;
ID3D11Buffer* quadIndexBuffer;
ID3D11Buffer* quadConstantBuffer;

//sun planet
ID3D11Buffer* sunBuffer;
ID3D11Buffer* sunIndexBuffer;
ID3D11Buffer* sunConstantBuffer;

//moon planet
ID3D11Buffer* moonBuffer;
ID3D11Buffer* moonIndexBuffer;
ID3D11Buffer* moonConstantBuffer;

//shader
ID3D11VertexShader* cubeVertexShader;
ID3D11PixelShader*	cubePixelShader;

ID3D11VertexShader* skyboxVertexShader;
ID3D11PixelShader*	skyboxPixelShader;

ID3D11GeometryShader * geometryShader;
ID3D11PixelShader * geometryPixelShader;

//fighter stuff
ID3D11Buffer * fighterBuffer;
ID3D11Buffer * fighterIndexBuffer;
ID3D11Buffer * fighterConstantBuffer;

//spaceship stuff
ID3D11Buffer * spaceShipBuffer;
ID3D11Buffer * spaceShipIndexBuffer;
ID3D11Buffer * spaceShipConstantBuffer;

//geometry buffer
ID3D11Buffer * geometryBuffer;
ID3D11Buffer * geometryIndexBuffer;
ID3D11Buffer * geometryConstantBuffer;

//light 
DirectionalLight directionalLight;
PointLight pointLight;
SpotLight spotLight;

//lightbuffer
ID3D11Buffer * lightBuffer;
ID3D11Buffer * pointLightBuffer;
ID3D11Buffer * spotLightBuffer;

//world matrix
XMMATRIX CubeWorldMatrix;
XMMATRIX FighterWorldMatrix;
XMMATRIX QuadWorldMatrix;
XMMATRIX SkyBoxMatrix;
XMMATRIX GeometryMatrix;
XMMATRIX spaceShipMatrix;
XMMATRIX sunMatrix;
XMMATRIX moonMatrix;

XMMATRIX ViewMatrix;
XMMATRIX GeoViewMatrix;
XMMATRIX GeoProjectionMatrix;
XMMATRIX ProjectionMatrix;


//matrix
XMVECTOR Eye = XMVectorSet(0.0f, 8.5f, -15.0f, 0.0f);
XMVECTOR Focus = XMVectorSet(0.0f, 4.0f, 0.0f, 0.0f);
XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR ResetEye = Eye;

XMFLOAT4 m_MeshColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
XMFLOAT4 black = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

//texture
ID3D11Texture2D * texture2D;
//zbuffer
ID3D11DepthStencilView*	depthStencil;

ID3D11SamplerState*	samplerState;

//texture
ID3D11ShaderResourceView * cubeTextureResources;
ID3D11ShaderResourceView * fighterTextureResources;
ID3D11ShaderResourceView * quadTextureResources;
ID3D11ShaderResourceView * skyboxTextureResources;
ID3D11ShaderResourceView * spaceShipTextureResources;
ID3D11ShaderResourceView * sunTextureResources;
ID3D11ShaderResourceView * moonTextureResources;

//map stuff
ID3D11Texture2D* renderTargetTextureMap;
ID3D11RenderTargetView* renderTargetViewMap;
ID3D11ShaderResourceView* shaderResourceViewMap;


//Model
Object fighter;
Object spaceShip;
Object sun;
Object moon;

float zoom = 2;
float nearPlane = 0.1f;
float farPlane = 250.0f;

void DrawFighter();
void ObjectInit(const char * filename, Object &model, ID3D11Buffer *& modelBuffer, ID3D11Buffer *& indexbuffer, ID3D11Buffer *&constantBuffer);
void BoxInit();
void DrawBox();
void LightInit();
void PointLightInit();
void DrawQuad();
void QuadInit();
void SpotLightInit();
void SkyBoxInit();
void DrawSkyBox();
void DrawGeometry();
void MapTextureInit();
void DrawSpaceShip();
void DrawPlanet();
void DrawMoon();

void CameraControl()
{
	if (GetAsyncKeyState(VK_F1)) {
		ViewMatrix = XMMatrixLookAtLH(ResetEye, Focus, Up); //reset
	}

	if (GetAsyncKeyState(VK_SHIFT)) {
		XMMATRIX translate = XMMatrixTranslation(0, 0, -0.005f); //zoom
		ViewMatrix = XMMatrixMultiply(ViewMatrix, translate);
	}

	if (GetAsyncKeyState(VK_CONTROL)) {
		XMMATRIX translate = XMMatrixTranslation(0, 0, 0.005f); //zoom out
		ViewMatrix = XMMatrixMultiply(ViewMatrix, translate);
	}

	if (GetAsyncKeyState('W')) {
		XMMATRIX translate = XMMatrixTranslation(0, -0.005f, 0); // move up
		ViewMatrix = XMMatrixMultiply(ViewMatrix, translate);
	}
	if (GetAsyncKeyState('S')) {
		XMMATRIX translate = XMMatrixTranslation(0, 0.005f, 0); // move down
		ViewMatrix = XMMatrixMultiply(ViewMatrix, translate);
	}
	if (GetAsyncKeyState('D')) {
		XMMATRIX translate = XMMatrixTranslation(-0.005f, 0, 0); // move right
		ViewMatrix = XMMatrixMultiply(ViewMatrix, translate);
	}
	if (GetAsyncKeyState('A')) {
		XMMATRIX translate = XMMatrixTranslation(0.005f, 0, 0); // move left
		ViewMatrix = XMMatrixMultiply(ViewMatrix, translate);
	}
	
	if (GetAsyncKeyState(VK_UP)) {
		XMMATRIX rotation = XMMatrixRotationX(0.0005f);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, rotation); // rotate up
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		XMMATRIX rotation = XMMatrixRotationX(-0.0005f);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, rotation); // rotate down
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		XMMATRIX rotation = XMMatrixRotationY(0.0005f);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, rotation); // rotate left
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		XMMATRIX rotation = XMMatrixRotationY(-0.0005f);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, rotation); // rotate right
	}

	if (GetAsyncKeyState('Q') & 0x1)
	{
		zoom += 0.05f; // zoom in
	}

	if (GetAsyncKeyState('E') & 0x1)
	{
		if(zoom > 1.1f)
		zoom -= 0.05f; // zoom out
	}

	if (GetAsyncKeyState('Z') & 0x1) // nearplane ++
	{
		nearPlane += 0.05f; 
	}

	if (GetAsyncKeyState('X') & 0x1) // nearplane --
	{
		if(nearPlane > 0.11)
		nearPlane -= 0.05f;
	
	}

	if (GetAsyncKeyState('C') & 0x1) // farPlane ++
	{		
		farPlane += 5.0f;	
	}

	if (GetAsyncKeyState('V') & 0x1) // farPlane --
	{		
		farPlane -= 5.0f;
		if (farPlane < 0.1f)
		{
			farPlane = 1.0f;
		}
	}

	ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PI / zoom, SCREENWIDTH / static_cast<float>(SCREENHEIGHT), nearPlane, farPlane);

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

	//map mini
	MapTextureInit();

	// texture loading
	CreateDDSTextureFromFile(device, L"Box_Red2Dark.dds", NULL, &cubeTextureResources);
	CreateDDSTextureFromFile(device, L"Spacesuit_D.dds", NULL, &fighterTextureResources);
	CreateDDSTextureFromFile(device, L"swampfloor_seamless.dds", NULL, &quadTextureResources);
	CreateDDSTextureFromFile(device, L"OutputCube.dds", NULL, &skyboxTextureResources);
	CreateDDSTextureFromFile(device, L"gladiator.dds", NULL, &spaceShipTextureResources);
	CreateDDSTextureFromFile(device, L"Sun.dds", NULL, &sunTextureResources);
	CreateDDSTextureFromFile(device, L"newmoon.dds", NULL, &moonTextureResources);


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

	device->CreateVertexShader(SkyBox_VS, sizeof(SkyBox_VS), NULL, &skyboxVertexShader);
	device->CreatePixelShader(SkyBox_PS, sizeof(SkyBox_PS), NULL, &skyboxPixelShader);

	device->CreateGeometryShader(GeometryShader, sizeof(GeometryShader), NULL, &geometryShader);
	device->CreatePixelShader(Geometry_PS, sizeof(Geometry_PS), NULL, &geometryPixelShader);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	UINT nums = ARRAYSIZE(layout);

	D3D11_INPUT_ELEMENT_DESC skyLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	// input layout
	device->CreateInputLayout(layout, nums, Trivial_VS, sizeof(Trivial_VS), &cubeInputLayout);
	device->CreateInputLayout(skyLayout, ARRAYSIZE(skyLayout), SkyBox_VS, sizeof(SkyBox_VS), &SkyBoxInputLayout);

	//Init Object
	BoxInit();
	ObjectInit("Astronaut.obj",fighter,fighterBuffer,fighterIndexBuffer,fighterConstantBuffer);
	ObjectInit("talon.obj", spaceShip, spaceShipBuffer, spaceShipIndexBuffer, spaceShipConstantBuffer);
	ObjectInit("moon2.obj", sun, sunBuffer, sunIndexBuffer, sunConstantBuffer);
	ObjectInit("moon2.obj", moon, moonBuffer, moonIndexBuffer, moonConstantBuffer);
	LightInit();
	PointLightInit();
	QuadInit();
	SpotLightInit();
	SkyBoxInit();


	//cube world matrix
	XMMATRIX translateCube = XMMatrixTranslation(0, 1.0f, 0); // move right
	CubeWorldMatrix = XMMatrixIdentity();
	CubeWorldMatrix = XMMatrixMultiply(CubeWorldMatrix, translateCube);

	//fighter world matrix
	XMMATRIX translate = XMMatrixTranslation(-5.0f, 0, 0); // move right
	XMMATRIX scale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	FighterWorldMatrix = XMMatrixIdentity();
	FighterWorldMatrix = XMMatrixMultiply(FighterWorldMatrix, translate);
	FighterWorldMatrix = XMMatrixMultiply(FighterWorldMatrix, scale);

	//rotation for fighter
	FighterWorldMatrix = XMMatrixMultiply(XMMatrixRotationY(-10.0f), FighterWorldMatrix);

	//quad world matrix
	XMMATRIX translate1 = XMMatrixTranslation(0, -1.0f, 0);
	XMMATRIX scale1 = XMMatrixScaling(20.0f, 20.0f, 20.0f);
	QuadWorldMatrix = XMMatrixIdentity();		
	QuadWorldMatrix = XMMatrixMultiply(QuadWorldMatrix, translate1);
	QuadWorldMatrix = XMMatrixMultiply(QuadWorldMatrix, scale1);

	//skybox world matrix
	SkyBoxMatrix = XMMatrixIdentity();

	// geometry worldmatrix
	GeometryMatrix = XMMatrixIdentity();

	//sun matrix
	XMMATRIX translate2 = XMMatrixTranslation(0, 20, 20);
	XMMATRIX scale2 = XMMatrixScaling(1, 1, 1);
	sunMatrix = XMMatrixIdentity();
	sunMatrix = XMMatrixMultiply(sunMatrix, translate2);
	sunMatrix = XMMatrixMultiply(sunMatrix, scale2);

	// moon matrix
	XMMATRIX translate4 = XMMatrixTranslation(40, 20, 100);
	XMMATRIX scale4 = XMMatrixScaling(0.5, 0.5, 0.5);
	moonMatrix = XMMatrixIdentity();
	moonMatrix = XMMatrixMultiply(moonMatrix, translate4);
	moonMatrix = XMMatrixMultiply(moonMatrix, scale4);

	//space ship worldMatrix
	XMMATRIX translate3 = XMMatrixTranslation(1, 0.5f, 0); // move right
	XMMATRIX scale3 = XMMatrixScaling(5, 5,5);
	spaceShipMatrix = XMMatrixIdentity();
	spaceShipMatrix = XMMatrixMultiply(spaceShipMatrix, translate3);
	spaceShipMatrix = XMMatrixMultiply(spaceShipMatrix, scale3);
	spaceShipMatrix = XMMatrixMultiply(XMMatrixRotationY(-9.0f), spaceShipMatrix);

	// view matrix
	ViewMatrix = XMMatrixLookAtLH(Eye, Focus, Up);
	GeoViewMatrix = XMMatrixLookAtLH(XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), Up);
	
	//projection matrix
	GeoProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PI / 2, SCREENWIDTH / static_cast<float>(SCREENHEIGHT), nearPlane, farPlane);
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
	CubeWorldMatrix = XMMatrixMultiply(XMMatrixRotationY(0.0001f), CubeWorldMatrix);

	//sun rotation
	sunMatrix = XMMatrixMultiply(XMMatrixRotationY(0.0003f),sunMatrix);	

	//moon orbit
	moonMatrix = XMMatrixMultiply(XMMatrixTranslation(30 ,30, -30),sunMatrix);

	moonMatrix = XMMatrixMultiply(moonMatrix,XMMatrixTranslation(0, 0, 30));

	moonMatrix = XMMatrixMultiply(moonMatrix, XMMatrixScaling(0.45, 0.45, 0.45));

	moonMatrix = XMMatrixMultiply(XMMatrixRotationY(t*2),moonMatrix);	

	//render
	deviceContext->ClearRenderTargetView(RTV, Colors::DarkCyan);

	deviceContext->ClearRenderTargetView(renderTargetViewMap, Colors::DarkCyan);

	deviceContext->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);

	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);

	deviceContext->PSSetConstantBuffers(1, 1, &pointLightBuffer);

	deviceContext->PSSetConstantBuffers(2, 1, &spotLightBuffer);
	
	//send cube matrix to vram
	Matrix send_matrix_to_vram;
	send_matrix_to_vram.World = XMMatrixTranspose(CubeWorldMatrix);
	send_matrix_to_vram.View = XMMatrixTranspose(GeoViewMatrix);
	send_matrix_to_vram.Projection = XMMatrixTranspose(GeoProjectionMatrix);

	Matrix send_planet_to_vram;
	send_planet_to_vram.World = XMMatrixTranspose(sunMatrix);
	send_planet_to_vram.View = XMMatrixTranspose(GeoViewMatrix);
	send_planet_to_vram.Projection = XMMatrixTranspose(GeoProjectionMatrix);

	Matrix send_moon_to_vram;
	send_moon_to_vram.World = XMMatrixTranspose(moonMatrix);
	send_moon_to_vram.View = XMMatrixTranspose(GeoViewMatrix);
	send_moon_to_vram.Projection = XMMatrixTranspose(GeoProjectionMatrix);

	//send spaceship to vram
	Matrix send_spaceShip_to_vram;
	send_spaceShip_to_vram.World = XMMatrixTranspose(spaceShipMatrix);
	send_spaceShip_to_vram.View = XMMatrixTranspose(GeoViewMatrix);
	send_spaceShip_to_vram.Projection = XMMatrixTranspose(ProjectionMatrix);

	//send skybox matrix to vram
	Matrix send_skybox_to_vram;
	send_skybox_to_vram.World = XMMatrixTranspose(SkyBoxMatrix);
	send_skybox_to_vram.View = XMMatrixTranspose(GeoViewMatrix);
	send_skybox_to_vram.Projection = XMMatrixTranspose(GeoProjectionMatrix);

	//send quad to vram
	Matrix quad_matrix_to_vram;
	quad_matrix_to_vram.World = XMMatrixTranspose(QuadWorldMatrix);
	quad_matrix_to_vram.View = XMMatrixTranspose(GeoViewMatrix);
	quad_matrix_to_vram.Projection = XMMatrixTranspose(GeoProjectionMatrix);

	//send geometry to vram
	Matrix geometry_matrix_to_vram;
	geometry_matrix_to_vram.World = XMMatrixTranspose(GeometryMatrix);
	geometry_matrix_to_vram.View = XMMatrixTranspose(GeoViewMatrix);
	geometry_matrix_to_vram.Projection = XMMatrixTranspose(GeoProjectionMatrix);
	
	//send fighter matrix to vram
	Matrix send_fighter_to_vram;
	send_fighter_to_vram.World = XMMatrixTranspose(FighterWorldMatrix);
	send_fighter_to_vram.View = XMMatrixTranspose(GeoViewMatrix);
	send_fighter_to_vram.Projection = XMMatrixTranspose(GeoProjectionMatrix);

	DirectionalLight send_directional_light_to_vram;
	send_directional_light_to_vram.color = directionalLight.color;
	send_directional_light_to_vram.direction = directionalLight.direction;

	PointLight send_point_light_to_vram;
	send_point_light_to_vram.Color = pointLight.Color;
	send_point_light_to_vram.Position = pointLight.Position;
	send_point_light_to_vram.Radius = pointLight.Radius;

	SpotLight send_spot_light_to_vram;
	send_spot_light_to_vram.Color = spotLight.Color;
	send_spot_light_to_vram.Radius = spotLight.Radius;
	send_spot_light_to_vram.Position = spotLight.Position;
	send_spot_light_to_vram.Direction = spotLight.Direction;

	deviceContext->PSSetSamplers(0, 1, &samplerState);	
	//Draw

	DrawBox();
	DrawFighter();
	DrawQuad();
	DrawSkyBox();		
	DrawSpaceShip();
	DrawPlanet();
	DrawMoon();

	deviceContext->UpdateSubresource(cubeConstantBuffer, 0, NULL, &send_matrix_to_vram, 0, 0);

	deviceContext->UpdateSubresource(skyboxConstantBuffer, 0, NULL, &send_skybox_to_vram, 0, 0);

	deviceContext->UpdateSubresource(quadConstantBuffer, 0, NULL, &quad_matrix_to_vram, 0, 0);

	deviceContext->UpdateSubresource(fighterConstantBuffer, 0, NULL, &send_fighter_to_vram, 0, 0);

	deviceContext->UpdateSubresource(lightBuffer, 0, NULL, &send_directional_light_to_vram, 0, 0);

	deviceContext->UpdateSubresource(pointLightBuffer, 0, NULL, &send_point_light_to_vram, 0, 0);

	deviceContext->UpdateSubresource(spotLightBuffer, 0, NULL, &send_spot_light_to_vram, 0, 0);

	deviceContext->UpdateSubresource(geometryConstantBuffer, 0, NULL, &geometry_matrix_to_vram, 0, 0);

	deviceContext->UpdateSubresource(spaceShipConstantBuffer, 0, NULL, &send_spaceShip_to_vram, 0, 0);

	deviceContext->UpdateSubresource(sunConstantBuffer, 0, NULL, &send_planet_to_vram, 0, 0);

	deviceContext->UpdateSubresource(moonConstantBuffer, 0, NULL, &send_moon_to_vram, 0, 0);

	deviceContext->OMSetRenderTargets(1, &RTV, NULL);
	ID3D11ShaderResourceView * temp = nullptr;
	deviceContext->PSSetShaderResources(NULL, 1, &temp);
	deviceContext->OMSetRenderTargets(1, &renderTargetViewMap, NULL);

	//send cube matrix to vram
	Matrix send_matrix_to_vram1;
	send_matrix_to_vram1.World = XMMatrixTranspose(CubeWorldMatrix);
	send_matrix_to_vram1.View = XMMatrixTranspose(ViewMatrix);
	send_matrix_to_vram1.Projection = XMMatrixTranspose(ProjectionMatrix);

	//send spaceship matrix to vram
	Matrix send_spaceShip_to_vram1;
	send_spaceShip_to_vram1.World = XMMatrixTranspose(spaceShipMatrix);
	send_spaceShip_to_vram1.View = XMMatrixTranspose(ViewMatrix);
	send_spaceShip_to_vram1.Projection = XMMatrixTranspose(ProjectionMatrix);

	Matrix send_moon_to_vram1;
	send_moon_to_vram1.World = XMMatrixTranspose(moonMatrix);
	send_moon_to_vram1.View = XMMatrixTranspose(ViewMatrix);
	send_moon_to_vram1.Projection = XMMatrixTranspose(ProjectionMatrix);
	
	//send planet to vram
	Matrix send_planet_to_vram1;
	send_planet_to_vram1.World = XMMatrixTranspose(sunMatrix);
	send_planet_to_vram1.View = XMMatrixTranspose(ViewMatrix);
	send_planet_to_vram1.Projection = XMMatrixTranspose(ProjectionMatrix);

	//send skybox matrix to vram
	Matrix send_skybox_to_vram1;
	send_skybox_to_vram1.World = XMMatrixTranspose(SkyBoxMatrix);
	send_skybox_to_vram1.View = XMMatrixTranspose(ViewMatrix);
	send_skybox_to_vram1.Projection = XMMatrixTranspose(ProjectionMatrix);

	//send quad to vram
	Matrix quad_matrix_to_vram1;
	quad_matrix_to_vram1.World = XMMatrixTranspose(QuadWorldMatrix);
	quad_matrix_to_vram1.View = XMMatrixTranspose(ViewMatrix);
	quad_matrix_to_vram1.Projection = XMMatrixTranspose(ProjectionMatrix);

	//send fighter matrix to vram
	Matrix send_fighter_to_vram1;
	send_fighter_to_vram1.World = XMMatrixTranspose(FighterWorldMatrix);
	send_fighter_to_vram1.View = XMMatrixTranspose(ViewMatrix);
	send_fighter_to_vram1.Projection = XMMatrixTranspose(ProjectionMatrix);

	//send geometry to vram
	Matrix geometry_matrix_to_vram1;
	geometry_matrix_to_vram1.World = XMMatrixTranspose(GeometryMatrix);
	geometry_matrix_to_vram1.View = XMMatrixTranspose(GeoViewMatrix);
	geometry_matrix_to_vram1.Projection = XMMatrixTranspose(GeoProjectionMatrix);

	DirectionalLight send_directional_light_to_vram1;
	send_directional_light_to_vram1.color = directionalLight.color;
	send_directional_light_to_vram1.direction = directionalLight.direction;

	PointLight send_point_light_to_vram1;
	send_point_light_to_vram1.Color = pointLight.Color;
	send_point_light_to_vram1.Position = pointLight.Position;
	send_point_light_to_vram1.Radius = pointLight.Radius;

	SpotLight send_spot_light_to_vram1;
	send_spot_light_to_vram1.Color = spotLight.Color;
	send_spot_light_to_vram1.Radius = spotLight.Radius;
	send_spot_light_to_vram1.Position = spotLight.Position;
	send_spot_light_to_vram1.Direction = spotLight.Direction;

	deviceContext->PSSetSamplers(0, 1, &samplerState);

	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);

	deviceContext->PSSetConstantBuffers(1, 1, &pointLightBuffer);

	deviceContext->PSSetConstantBuffers(2, 1, &spotLightBuffer);

	DrawSkyBox();
	DrawQuad();
	DrawBox();
	DrawFighter();	
	DrawSpaceShip();
	DrawPlanet();
	DrawMoon();

	deviceContext->UpdateSubresource(cubeConstantBuffer, 0, NULL, &send_matrix_to_vram1, 0, 0);

	deviceContext->UpdateSubresource(skyboxConstantBuffer, 0, NULL, &send_skybox_to_vram1, 0, 0);

	deviceContext->UpdateSubresource(quadConstantBuffer, 0, NULL, &quad_matrix_to_vram1, 0, 0);

	deviceContext->UpdateSubresource(fighterConstantBuffer, 0, NULL, &send_fighter_to_vram1, 0, 0);

	deviceContext->UpdateSubresource(geometryConstantBuffer, 0, NULL, &geometry_matrix_to_vram1, 0, 0);

	deviceContext->UpdateSubresource(lightBuffer, 0, NULL, &send_directional_light_to_vram1, 0, 0);

	deviceContext->UpdateSubresource(pointLightBuffer, 0, NULL, &send_point_light_to_vram1, 0, 0);

	deviceContext->UpdateSubresource(spotLightBuffer, 0, NULL, &send_spot_light_to_vram1, 0, 0);

	deviceContext->UpdateSubresource(spaceShipConstantBuffer, 0, NULL, &send_spaceShip_to_vram1, 0, 0);

	deviceContext->UpdateSubresource(sunConstantBuffer, 0, NULL, &send_planet_to_vram1, 0, 0);

	deviceContext->UpdateSubresource(moonConstantBuffer, 0, NULL, &send_moon_to_vram1, 0, 0);

	deviceContext->OMSetRenderTargets(1, &RTV, depthStencil);

	DrawGeometry();

	swapChain->Present(0, 0);

	return true;
}

void Shutdown()
{
	RTV->Release();
	backBuffer->Release();	

	cubeInputLayout->Release(); 
	SkyBoxInputLayout->Release();
	
	cubeVertexShader->Release(); 
	cubePixelShader->Release();

	skyboxPixelShader->Release();
	skyboxVertexShader->Release();

	geometryShader->Release();
	geometryPixelShader->Release();

	skyboxBuffer->Release();
	skyboxConstantBuffer->Release();
	skyboxIndexBuffer->Release();

	geometryBuffer->Release();
	geometryIndexBuffer->Release();
	geometryConstantBuffer->Release();

	fighterBuffer->Release();
	fighterIndexBuffer->Release();
	fighterConstantBuffer->Release();

	spaceShipBuffer->Release();
	spaceShipIndexBuffer->Release();
	spaceShipConstantBuffer->Release();
	spaceShipTextureResources->Release();

	cubeBuffer->Release();
	cubeIndexBuffer->Release();
	cubeConstantBuffer->Release();

	quadBuffer->Release();
	quadIndexBuffer->Release();
	quadConstantBuffer->Release();

	lightBuffer->Release();
	pointLightBuffer->Release();
	spotLightBuffer->Release();

	texture2D->Release();
	depthStencil->Release();
	samplerState->Release();

	cubeTextureResources->Release();
	fighterTextureResources->Release();	
	quadTextureResources->Release();
	skyboxTextureResources->Release();

	sunBuffer->Release();
	sunConstantBuffer->Release();
	sunIndexBuffer->Release();
	sunTextureResources->Release();

	moonBuffer->Release();
	moonConstantBuffer->Release();
	moonIndexBuffer->Release();
	moonTextureResources->Release();
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

void ObjectInit(const char * filename, Object & model, ID3D11Buffer *& modelBuffer, ID3D11Buffer *& indexbuffer, ID3D11Buffer *&constantBuffer)
{
	Object tempModel;
	tempModel.Load(filename);
	model = tempModel;

	Vert * vertices = new Vert[model.GetModel().size()];
	unsigned int count = model.GetIndex().size();
	unsigned int * indicies = new unsigned int[count];

	for (unsigned int i = 0; i < model.GetModel().size(); i++)
		vertices[i] = model.GetModel()[i];

	for (unsigned int i = 0; i < model.GetIndex().size(); i++)
		indicies[i] = model.GetIndex()[i];

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
	
	device->CreateBuffer(&fighterBufferDesc, &fighterSubResources, &modelBuffer);

	D3D11_BUFFER_DESC fighterBufferDesc1;
	ZeroMemory(&fighterBufferDesc1, sizeof(D3D11_BUFFER_DESC));	

	fighterBufferDesc1.Usage = D3D11_USAGE_DEFAULT;
	fighterBufferDesc1.BindFlags = D3D11_BIND_INDEX_BUFFER;
	fighterBufferDesc1.CPUAccessFlags = NULL;
	fighterBufferDesc1.ByteWidth = sizeof(unsigned int) * count;

	fighterSubResources.pSysMem = indicies;

	device->CreateBuffer(&fighterBufferDesc1, &fighterSubResources, &indexbuffer);
	
	D3D11_BUFFER_DESC fighterBufferDesc2;
	ZeroMemory(&fighterBufferDesc2, sizeof(D3D11_BUFFER_DESC));
	fighterBufferDesc2.Usage = D3D11_USAGE_DEFAULT;
	fighterBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	fighterBufferDesc2.ByteWidth = sizeof(Matrix);
	

	device->CreateBuffer(&fighterBufferDesc2, nullptr, &constantBuffer);

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

	directionalLight.color.x = 0.0f;
	directionalLight.color.y = 0.0f;
	directionalLight.color.z = 50.0f;
	directionalLight.color.w = 1.0f;

	directionalLight.direction.x = 1.0f;
	directionalLight.direction.y = 0.0f;
	directionalLight.direction.z = 0.0f;

	//directionalLight.padding = 1.0f;

	D3D11_BUFFER_DESC ligthConstbuffdesc;

	ZeroMemory(&ligthConstbuffdesc, sizeof(D3D11_BUFFER_DESC));
	ligthConstbuffdesc.Usage = D3D11_USAGE_DEFAULT;
	ligthConstbuffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	ligthConstbuffdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ligthConstbuffdesc.ByteWidth = sizeof(DirectionalLight);

	device->CreateBuffer(&ligthConstbuffdesc, nullptr, &lightBuffer);	
}

void PointLightInit()
{
	//point light init value
	pointLight.Color.x = 0.0f;
	pointLight.Color.y = 5.0f;
	pointLight.Color.z = 0.0f;
	pointLight.Color.w = 0.0f;

	pointLight.Position.x = 0;
	pointLight.Position.y = 3.0f;
	pointLight.Position.z = -2.0f;
	pointLight.Position.w = 0;
	
	//directionalLight.padding = 1.0f;

	D3D11_BUFFER_DESC pointLightDesc;

	ZeroMemory(&pointLightDesc, sizeof(D3D11_BUFFER_DESC));
	pointLightDesc.Usage = D3D11_USAGE_DEFAULT;
	pointLightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//	ligthConstbuffdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pointLightDesc.ByteWidth = sizeof(PointLight);

	device->CreateBuffer(&pointLightDesc, nullptr, &pointLightBuffer);
}

void SpotLightInit()
{
	spotLight.Position.x = 2.5f; 
	spotLight.Position.y = 5; 
	spotLight.Position.z = 0; 
	spotLight.Position.w = 0;

	spotLight.Color.x = 25; 
	spotLight.Color.y = 25; 
	spotLight.Color.z = 25;
	spotLight.Color.w = 1;

	spotLight.Direction.x = -0.4f; 
	spotLight.Direction.y = -1; 
	spotLight.Direction.z = 0; 
	spotLight.Direction.w = 0;

	spotLight.Radius.x = .99f;
	spotLight.Radius.y = .90f; 
	spotLight.Radius.z = 10; 
	spotLight.Radius.w = 0;

	D3D11_BUFFER_DESC spotLightDesc;

	ZeroMemory(&spotLightDesc, sizeof(D3D11_BUFFER_DESC));
	spotLightDesc.Usage = D3D11_USAGE_DEFAULT;
	spotLightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//	ligthConstbuffdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	spotLightDesc.ByteWidth = sizeof(SpotLight);

	device->CreateBuffer(&spotLightDesc, nullptr, &spotLightBuffer);
	
}

void QuadInit()
{
	Vert quad[] = {

		// top face
		{ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),		XMFLOAT2(1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),		XMFLOAT2(0.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),			XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),		XMFLOAT2(0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),		XMFLOAT2(1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) }
	};

	UINT32 quadIndexes[] = {
		3,1,0,
		2,1,3		
	};

	// quad buffer
	D3D11_BUFFER_DESC quadBuffdesc;
	ZeroMemory(&quadBuffdesc, sizeof(D3D11_BUFFER_DESC));
	quadBuffdesc.Usage = D3D11_USAGE_DEFAULT;
	quadBuffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	quadBuffdesc.ByteWidth = sizeof(Vert) * 4;

	// Initializing SubSource
	D3D11_SUBRESOURCE_DATA data1;
	ZeroMemory(&data1, sizeof(data1));
	data1.pSysMem = quad;

	// Creating Vertex Buffer
	device->CreateBuffer(&quadBuffdesc, &data1, &quadBuffer);
	device->CreateBuffer(&quadBuffdesc, &data1, &geometryBuffer);

	// index buffer
	quadBuffdesc.Usage = D3D11_USAGE_DEFAULT;
	quadBuffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	quadBuffdesc.ByteWidth = sizeof(UINT32) * 6;
	data1.pSysMem = quadIndexes;
	device->CreateBuffer(&quadBuffdesc, &data1, &quadIndexBuffer);
	device->CreateBuffer(&quadBuffdesc, &data1, &geometryIndexBuffer);

	// constant
	quadBuffdesc.Usage = D3D11_USAGE_DEFAULT;
	quadBuffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	quadBuffdesc.ByteWidth = sizeof(Matrix);

	device->CreateBuffer(&quadBuffdesc, nullptr, &quadConstantBuffer);

	device->CreateBuffer(&quadBuffdesc, nullptr, &geometryConstantBuffer);
}

void DrawQuad()
{
	unsigned int	strides = sizeof(Vert);
	unsigned int	offsets = 0;

	deviceContext->IASetVertexBuffers(0, 1, &quadBuffer, &strides, &offsets);

	deviceContext->VSSetShader(cubeVertexShader, NULL, 0);

	deviceContext->PSSetShader(cubePixelShader, NULL, 0);

	deviceContext->PSSetShaderResources(0, 1, &quadTextureResources);

	deviceContext->IASetInputLayout(cubeInputLayout);

	deviceContext->IASetIndexBuffer(quadIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetConstantBuffers(0, 1, &quadConstantBuffer);

	deviceContext->DrawIndexed(6, 0, 0);
}

void SkyBoxInit()
{
	SkyBox Box[] =
	{
		XMFLOAT4(-100.0f, -100.0f, -100.0f, 1.0f),
		XMFLOAT4(100.0f, -100.0f, -100.0f, 1.0f),
		XMFLOAT4(100.0f, -100.0f, 100.0f, 1.0f),
		XMFLOAT4(-100.0f, -100.0f, 100.0f, 1.0f),

		XMFLOAT4(-100.0f, 100.0f, -100.0f, 1.0f),
		XMFLOAT4(100.0f, 100.0f, -100.0f, 1.0f),
		XMFLOAT4(100.0f, 100.0f, 100.0f, 1.0f),
		XMFLOAT4(-100.0f, 100.0f, 100.0f, 1.0f),

		XMFLOAT4(100.0f, -100.0f, 100.0f, 1.0f),
		XMFLOAT4(100.0f, -100.0f, -100.0f, 1.0f),
		XMFLOAT4(100.0f, 100.0f, -100.0f, 1.0f),
		XMFLOAT4(100.0f, 100.0f, 100.0f, 1.0f),

		XMFLOAT4(-100.0f, -100.0f, 100.0f, 1.0f),
		XMFLOAT4(-100.0f, -100.0f, -100.0f, 1.0f),
		XMFLOAT4(-100.0f, 100.0f, -100.0f, 1.0f),
		XMFLOAT4(-100.0f, 100.0f, 100.0f, 1.0f),
		//////
		XMFLOAT4(-100.0f, -100.0f, 100.0f, 1.0f),
		XMFLOAT4(100.0f, -100.0f, 100.0f, 1.0f),
		XMFLOAT4(100.0f, 100.0f, 100.0f, 1.0f),
		XMFLOAT4(-100.0f, 100.0f, 100.0f, 1.0f),
		//////
		XMFLOAT4(-100.0f, -100.0f, -100.0f, 1.0f),
		XMFLOAT4(100.0f, -100.0f, -100.0f, 1.0f),
		XMFLOAT4(100.0f, 100.0f, -100.0f, 1.0f),
		XMFLOAT4(-100.0f, 100.0f, -100.0f, 1.0f),
	};

	UINT32 buffer[] =
	{
		// Front // 
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
	
	//sky buffer desc
	D3D11_BUFFER_DESC buffdesc;
	ZeroMemory(&buffdesc, sizeof(D3D11_BUFFER_DESC));
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.ByteWidth = sizeof(SkyBox) * 24;

	// Initializing SubSource
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = Box;

	// Creating Vertex Buffer
	device->CreateBuffer(&buffdesc, &data, &skyboxBuffer);

	// index buffer
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.ByteWidth = sizeof(UINT32) * 36;
	data.pSysMem = buffer;
	device->CreateBuffer(&buffdesc, &data, &skyboxIndexBuffer);

	// constant buffer
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.ByteWidth = sizeof(Matrix);
	device->CreateBuffer(&buffdesc, nullptr, &skyboxConstantBuffer);
}

void DrawSkyBox()
{
	unsigned int	strides = sizeof(SkyBox);
	unsigned int	offsets = 0;

	deviceContext->IASetVertexBuffers(0, 1, &skyboxBuffer, &strides, &offsets);

	deviceContext->VSSetShader(skyboxVertexShader, NULL, 0);

	deviceContext->PSSetShader(skyboxPixelShader, NULL, 0);

	deviceContext->PSSetShaderResources(0, 1, &skyboxTextureResources);

	deviceContext->IASetInputLayout(SkyBoxInputLayout);

	deviceContext->IASetIndexBuffer(skyboxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetConstantBuffers(0, 1, &skyboxConstantBuffer);

	deviceContext->DrawIndexed(36, 0, 0);
}

void DrawSpaceShip()
{
	unsigned int	strides = sizeof(Vert);
	unsigned int	offsets = 0;

	deviceContext->VSSetShader(cubeVertexShader, NULL, 0);

	deviceContext->PSSetShader(cubePixelShader, NULL, 0);

	deviceContext->IASetVertexBuffers(0, 1, &spaceShipBuffer, &strides, &offsets);

	deviceContext->PSSetShaderResources(0, 1, &spaceShipTextureResources);

	deviceContext->IASetIndexBuffer(spaceShipIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetInputLayout(cubeInputLayout);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetConstantBuffers(0, 1, &spaceShipConstantBuffer);

	deviceContext->DrawIndexed(spaceShip.GetIndex().size(), 0, 0);
}

void DrawPlanet()
{
	unsigned int	strides = sizeof(Vert);
	unsigned int	offsets = 0;

	deviceContext->VSSetShader(cubeVertexShader, NULL, 0);

	deviceContext->PSSetShader(geometryPixelShader, NULL, 0);

	deviceContext->IASetVertexBuffers(0, 1, &sunBuffer, &strides, &offsets);

	deviceContext->PSSetShaderResources(0, 1, &sunTextureResources);

	deviceContext->IASetIndexBuffer(sunIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetInputLayout(cubeInputLayout);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetConstantBuffers(0, 1, &sunConstantBuffer);

	deviceContext->DrawIndexed(sun.GetIndex().size(), 0, 0);
}

void DrawGeometry()
{
	unsigned int	strides = sizeof(Vert);
	unsigned int	offsets = 0;

	deviceContext->IASetVertexBuffers(0, 1, &geometryBuffer, &strides, &offsets);

	deviceContext->VSSetShader(cubeVertexShader, NULL, 0);
	
	deviceContext->PSSetShader(geometryPixelShader, NULL, 0);

	deviceContext->PSSetConstantBuffers(0, 1, &geometryConstantBuffer);

	deviceContext->GSSetShader(geometryShader, NULL, 0);

	deviceContext->GSSetConstantBuffers(0, 1, &geometryConstantBuffer);

	deviceContext->PSSetShaderResources(0, 1, &shaderResourceViewMap);

	deviceContext->IASetInputLayout(cubeInputLayout);

	deviceContext->IASetIndexBuffer(geometryIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	deviceContext->VSSetConstantBuffers(0, 1, &geometryConstantBuffer);

	deviceContext->Draw(1, 0);

	deviceContext->GSSetShader(NULL, NULL, NULL);

}

void MapTextureInit()
{

	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right;
	UINT height = rc.bottom;

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	device->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureMap);

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	device->CreateRenderTargetView(renderTargetTextureMap, &renderTargetViewDesc, &renderTargetViewMap);

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(renderTargetTextureMap, &shaderResourceViewDesc, &shaderResourceViewMap);
}

void SpotLightControl()
{

}

void DrawMoon()
{
	unsigned int	strides = sizeof(Vert);
	unsigned int	offsets = 0;

	deviceContext->VSSetShader(cubeVertexShader, NULL, 0);

	deviceContext->PSSetShader(geometryPixelShader, NULL, 0);

	deviceContext->IASetVertexBuffers(0, 1, &moonBuffer, &strides, &offsets);

	deviceContext->PSSetShaderResources(0, 1, &moonTextureResources);

	deviceContext->IASetIndexBuffer(moonIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetInputLayout(cubeInputLayout);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetConstantBuffers(0, 1, &moonConstantBuffer);

	deviceContext->DrawIndexed(moon.GetIndex().size(), 0, 0);
}