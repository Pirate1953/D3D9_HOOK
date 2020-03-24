#pragma once
#ifndef HEADER_H
#define HEADER_H

#include <d3d9.h>
#include <d3dx9core.h>

#include <vector>
#include <iostream>
#include <fstream>

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

//typedef IDirect3D9* (WINAPI *DIRECTCREATE9)(unsigned int); //Pointer to D3D Device

typedef HRESULT(WINAPI *tEndScene)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(WINAPI *tDrawIndexedPrimitive)(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);

tDrawIndexedPrimitive oDrawIndexedPrimitive;
tEndScene oEndScene;
bool StartLog = false;
D3DVIEWPORT9 Vpt;
LPDIRECT3DTEXTURE9 Green = NULL;
LPDIRECT3DTEXTURE9 Red = NULL;
LPDIRECT3DTEXTURE9 pTx = NULL;
D3DLOCKED_RECT d3dlr;
LPD3DXFONT pFont = NULL;
char strbuff[250];

BYTE codeFragment_endscene[5] = { 0, 0, 0, 0, 0 };
BYTE jump_endscene[5] = { 0, 0, 0, 0, 0 };
DWORD savedProtection_endscene = 0;

BYTE codeFragment_drawindex[5] = { 0, 0, 0, 0, 0 };
BYTE jump_drawindex[5] = { 0, 0, 0, 0, 0 };
DWORD savedProtection_drawindex = 0;

//Methods from vTable
DWORD endsc = 0;
DWORD drawindex = 0;

//For logging
UINT iStride = 0;
UINT iBaseTex = 0;
LPDIRECT3DBASETEXTURE9 BTEX = NULL;
bool Found;
std::vector<DWORD> BASETEX;

DWORD btex = 0;
UINT BaseVertexIndexFound = 0;
UINT MinVertexIndexFound = 0;
UINT NumVerticesFound = 0;
UINT startIndexFound = 0;
UINT PrimitiveCountFound = 0;

#endif