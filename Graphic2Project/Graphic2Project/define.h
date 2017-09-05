#pragma once
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
#include "DDSTextureLoader.h"
#include <vector>

#include "Trivial_VS.csh"
#include "Trivial_PS.csh"


using namespace std;
using namespace DirectX;

struct Vert
{
	XMFLOAT4 coordination;
	XMFLOAT4 Color;
	XMFLOAT2 uvs;
	XMFLOAT4 normal;
};


struct Matrix
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;

};