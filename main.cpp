#include "header.h"

//Get Virtual Methods Table
void GetDevice9Methods()
{
	HWND hWnd = CreateWindowA("STATIC", "dummy", 0, 0, 0, 0, 0, 0, 0, 0, 0);
	HMODULE hD3D9 = LoadLibrary("d3d9");
	IDirect3D9* d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DDISPLAYMODE d3ddm;
	d3d->GetAdapterDisplayMode(0, &d3ddm);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = d3ddm.Format;
	IDirect3DDevice9* d3dDevice = 0;
	d3d->CreateDevice(0, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice);
	DWORD* vTable = (DWORD*)(*((DWORD*)d3dDevice));
	endsc = vTable[42] - (DWORD)hD3D9;
	drawindex = vTable[82] - (DWORD)hD3D9;
	d3dDevice->Release();
	d3d->Release();
	FreeLibrary(hD3D9);
	CloseHandle(hWnd);
}

HRESULT WINAPI hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	//Restore bytes of original func
	BYTE* codeDest = (BYTE*)oEndScene;
	codeDest[0] = codeFragment_endscene[0];
	*((DWORD*)(codeDest + 1)) = *((DWORD*)(codeFragment_endscene + 1));

	pDevice->GetViewport(&Vpt);
	RECT FRect = {
		static_cast<long>(Vpt.Width - 500),
		static_cast<long>(Vpt.Height - 500),
		static_cast<long>(Vpt.Width),
		static_cast<long>(Vpt.Height)
	};
	if (pDevice->CreateTexture(8, 8, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &Green, 0) == S_OK)
		if (pDevice->CreateTexture(8, 8, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pTx, 0) == S_OK)
			if (pTx->LockRect(0, &d3dlr, 0, D3DLOCK_DONOTWAIT | D3DLOCK_NOSYSLOCK) == S_OK)
			{
				for (UINT xy = 0; xy < 8 * 8; xy++)
				{
					((PDWORD)d3dlr.pBits)[xy] = 0xFF00FF00;
				}
				pTx->UnlockRect(0);
				pDevice->UpdateTexture(pTx, Green);
				pTx->Release();
			}

	if (pFont == 0)
		D3DXCreateFontA(pDevice, 25, 15, 700, 0, 0, 1, 0, 0, DEFAULT_PITCH | FF_DONTCARE, "Calibri", &pFont);

	if (pFont && StartLog)
	{
		sprintf_s(strbuff, "Textures: %i\nStride: %i\nBase Tex Num: %i\n" \
			"Log Enable: %i\n\nNumPad1: Stride++ \nNumPad2: Stride-- \nNumPad4: BaseTexNum++ \n" \
			"NumPad5: BaseTexNum-- \nNumPad0: Log On/Off \n%i %i %i %i %i %i", \
			BASETEX.size(),
			iStride,
			iBaseTex + 1,
			StartLog,

			BaseVertexIndexFound,
			MinVertexIndexFound,
			NumVerticesFound,
			startIndexFound,
			PrimitiveCountFound,
			btex);
		pFont->DrawTextA(0, strbuff, -1, &FRect, DT_CENTER | DT_NOCLIP, 0xFF00FF00);
	}

	HRESULT res = oEndScene(pDevice);	//Call original func

	codeDest[0] = jump_endscene[0];
	*((DWORD*)(codeDest + 1)) = *((DWORD*)(jump_endscene + 1));
	
	return res;
}

HRESULT WINAPI hkDrawIndexedPrimitive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT PrimitiveCount)
{
	BYTE* codeDest = (BYTE*)oDrawIndexedPrimitive;
	codeDest[0] = codeFragment_drawindex[0];
	*((DWORD*)(codeDest + 1)) = *((DWORD*)(codeFragment_drawindex + 1));

	LPDIRECT3DVERTEXBUFFER9 Stream_Data;
	UINT Offset = 0;
	UINT Stride = 0;

	if (pDevice->GetStreamSource(0, &Stream_Data, &Offset, &Stride) == S_OK)
		Stream_Data->Release();

	if (StartLog == true)
	{
		if (Stride == iStride)
		{
			pDevice->GetTexture(0, &BTEX);
			Found = false;
			for (UINT i = 0; i < BASETEX.size(); i++) {
				if (BASETEX[i] == (DWORD)BTEX)
					Found = true; break;
			}
			if (Found == false)
				BASETEX.push_back((DWORD)BTEX);
			if (BASETEX[iBaseTex] == (DWORD)BTEX && Green)
			{
				btex = (DWORD)BTEX;
				BaseVertexIndexFound = BaseVertexIndex;
				MinVertexIndexFound = MinVertexIndex;
				NumVerticesFound = NumVertices;
				startIndexFound = startIndex;
				PrimitiveCountFound = PrimitiveCount;
				pDevice->SetTexture(0, Green);
				pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
				oDrawIndexedPrimitive(pDevice, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, PrimitiveCount);

				pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			}
		}
	}
	HRESULT res = oDrawIndexedPrimitive(pDevice, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, PrimitiveCount);

	codeDest[0] = jump_drawindex[0];
	*((DWORD*)(codeDest + 1)) = *((DWORD*)(jump_drawindex + 1));
	return res;
}

//Code Inject
void HookDevice9Methods()
{
	HMODULE hD3D9 = GetModuleHandle("d3d9.dll");

	oEndScene = (tEndScene)((DWORD)hD3D9 + endsc);

	jump_endscene[0] = 0xE9;
	DWORD addr_endscene = (DWORD)hkEndScene - (DWORD)oEndScene - 5;	//Address of Inject
	memcpy(jump_endscene + 1, &addr_endscene, sizeof(DWORD));
	memcpy(codeFragment_endscene, oEndScene, 5);	//Save code for hook of original func
	VirtualProtect(oEndScene, 8, PAGE_EXECUTE_READWRITE, &savedProtection_endscene);
	memcpy(oEndScene, jump_endscene, 5);	//Replace code with jump to Inject

	/////////////////////////////////////////////////////////////////////////////////////

	oDrawIndexedPrimitive = (tDrawIndexedPrimitive)((DWORD)hD3D9 + drawindex);
	jump_drawindex[0] = 0xE9;
	DWORD addr_drawindex = (DWORD)hkDrawIndexedPrimitive - (DWORD)oDrawIndexedPrimitive - 5;
	memcpy(jump_drawindex + 1, &addr_drawindex, sizeof(DWORD));
	memcpy(codeFragment_drawindex, oDrawIndexedPrimitive, 5);
	VirtualProtect(oDrawIndexedPrimitive, 8, PAGE_EXECUTE_READWRITE, &savedProtection_drawindex);
	memcpy(oDrawIndexedPrimitive, jump_drawindex, 5);
}

DWORD WINAPI TF(void* lParam)
{
	GetDevice9Methods();
	HookDevice9Methods();
	return 0;
}

DWORD WINAPI KeyboardHook(void* lParam)
{
	while (1)
	{
		if (GetAsyncKeyState(VK_NUMPAD0))
		{
			StartLog = !StartLog;
		}
		if (GetAsyncKeyState(VK_NUMPAD1))
		{
			iStride++;
		}
		if (GetAsyncKeyState(VK_NUMPAD2))
		{
			iStride--;
		}
		if (GetAsyncKeyState(VK_NUMPAD4))
		{
			iBaseTex++;
		}
		if (GetAsyncKeyState(VK_NUMPAD5))
		{
			iBaseTex--;
		}
		Sleep(100);
	}
	return 0;
}

int WINAPI DllMain(HINSTANCE hInst, DWORD ul_reason_for_call, void* lpReserver)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, &TF, 0, 0, 0);
		CreateThread(0, 0, &KeyboardHook, 0, 0, 0);
	}
	return 1;
}