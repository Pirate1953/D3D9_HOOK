#include "header.h"

using namespace std;

void PrintText(char pString[], int x, int y, D3DCOLOR col, ID3DXFont *font)
{
	RECT FontRect = { x, y, x + 500, y + 30 };
	font->DrawText(NULL, pString, -1, &FontRect, DT_LEFT | DT_WORDBREAK, col);
}

int GetTextWidth(const char *szText, ID3DXFont *pFont)
{
	RECT rcRect = { 0, 0, 0, 0 };
	if (pFont)
	{
		pFont->DrawText(NULL, szText, (int)strlen(szText), &rcRect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
	}
	return rcRect.right - rcRect.left;
}

void FillRGB(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9 *pDevice)
{
	if (w < 0) w = 1;
	if (h < 0) h = 1;
	if (x < 0) x = 1;
	if (y < 0) y = 1;

	D3DRECT rec = { x, y, x + w, y + h };
	pDevice->Clear(1, &rec, D3DCLEAR_TARGET, color, 0, 0);
}

void FillARGB(int x, int y, int w, int h, D3DCOLOR color)
{
	D3DXVECTOR2 DotPosition[2];
	DotPosition[0] = D3DXVECTOR2((float)(x + w / 2), (float)y);
	DotPosition[1] = D3DXVECTOR2((float)(x + w / 2), (float)(y + h));

	g_Line->SetAntialias(false);
	g_Line->SetWidth((float)w);
	g_Line->Draw(DotPosition, 2, color);
}

void DrawBorder(int x, int y, int w, int h, int px, D3DCOLOR BorderColor, IDirect3DDevice9 *pDevice)
{
	FillRGB(x, (y + h - px), w, px, BorderColor, pDevice);
	FillRGB(x, y, px, h, BorderColor, pDevice);
	FillRGB(x, y, w, px, BorderColor, pDevice);
	FillRGB((x + w - px), y, px, h, BorderColor, pDevice);
}

void DrawBoxArgb(int x, int y, int w, int h, D3DCOLOR BoxColor, D3DCOLOR BorderColor, IDirect3DDevice9* pDevice)
{
	FillARGB(x, y, w, h, BoxColor);
	DrawBorder(x, y, w, h, 1, BorderColor, pDevice);
}

void DrawBox(int x, int y, int w, int h, D3DCOLOR BoxColor, D3DCOLOR BorderColor, IDirect3DDevice9* pDevice)
{
	FillRGB(x, y, w, h, BoxColor, pDevice);
	DrawBorder(x, y, w, h, 1, BorderColor, pDevice);
}


TpMenu menu;

void TpMenu::StartHack()
{
	if (Col[2].hk[0].value == 1)
	{
		FillRGB(g_ViewPort.Width / 2 - 10, g_ViewPort.Height / 2, 20, 1, RED, g_pDevice);
		FillRGB(g_ViewPort.Width / 2, g_ViewPort.Height / 2 - 10, 1, 20, RED, g_pDevice);
	}
}

void TpMenu::AddColum(int id, char *name)
{
	Col[id].Name = name;
	colCount++;
}

void TpMenu::AddItem(int colum, int id, char *name, int type)
{
	Col[colum].hk[id].id = id;
	Col[colum].hk[id].Name = name;
	Col[colum].hk[id].type = type;
	switch (type)
	{
	case 0:
		Col[colum].hk[id].maxVal = 1;
		break;
	case 1:
		Col[colum].hk[id].maxVal = 99;
		break;
	case 2:
		Col[colum].hk[id].maxVal = 4;
		break;
	}
	Col[colum].hacks++;
}

void TpMenu::BuildMenu(char *name, int x, int y, int w)
{
	if (isMenuOn)
	{
		FillARGB(x, y, w, fontSize + 5, BLUE);
		DrawBorder(x, y, w, fontSize + 5, 1, RED, g_pDevice);

		PrintText(name, x + (w - GetTextWidth(name, g_pFont)) / 2, y + 2, GREEN, g_pFont);

		FillARGB(x + 3, y + fontSize + 5, MenuW - 6, MenuH - (fontSize + 5), BLUE); 
		DrawBorder(x + 3, y + fontSize + 5, MenuW - 6, MenuH - (fontSize + 5), 1, RED, g_pDevice);


		MenuH = fontSize + 10;


		for (int i = 0; i < colCount; i++)
		{
			char hName[15];
			sprintf_s(hName, "[%s]", Col[i].Name);

			DrawBorder(x + 5, y + MenuH, w - 10, fontSize + 10, 1, (selector == i &&itemSelector == -1) ? GREEN : RED, g_pDevice);
			PrintText(hName, x + (w - 10 - GetTextWidth(hName, g_pFont)) / 2, y + MenuH + 5, Col[i].on ? GREEN : RED, g_pFont);
			Col[i].y = y + MenuH;
			MenuH += fontSize + 13;
			if (Col[i].on)
				for (int a = 0; a < Col[i].hacks; a++)
				{
					PrintText(Col[i].hk[a].Name, x + 10, y + MenuH, selector == i && itemSelector == a ? GREEN : RED, g_pFont);
					PrintText(Col[i].hk[a].value == 0 ? "N" : "Y", x + w - 35, y + MenuH, Col[i].hk[a].value == 0 ? RED : GREEN, g_pFont);
					MenuH += fontSize + 3;

				}

		}
		FillRGB(x + 3, y + MenuH + 3, w - 6, 3, GREEN, g_pDevice);
		PrintText("This is footer!", x + (w - GetTextWidth("This is footer!", g_pFont)) / 2, y + MenuH + 6, GREEN, g_pFont);
		MenuH += fontSize + 8;
	}
}

void TpMenu::KeyboardEvents()
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
		isMenuOn = !isMenuOn;

	if (GetAsyncKeyState(VK_UP) & 1)
	{
		if (selector > 0 && !Col[selector].on)
		{
			selector--;
			if (Col[selector].on)
			{
				itemSelector = Col[selector].hacks - 1;
				return;
			}
		}

		if (Col[selector].on && itemSelector > -1)  itemSelector--;
		else
		{
			if (selector > 0)
			{

				if (Col[selector].on)
				{
					selector--;
					if (Col[selector].on)
						itemSelector = Col[selector].hacks - 1;
				}
				else
					itemSelector = -1;

			}
		}
	}
	if (GetAsyncKeyState(VK_DOWN) & 1)
	{
		if (selector < colCount - 1 && !Col[selector].on)
		{
			selector++;
			if (Col[selector].on)
				itemSelector = -1;
			return;
		}

		if (Col[selector].on && itemSelector < Col[selector].hacks - 1) itemSelector++;
		else
		{
			if (selector < colCount - 1)
			{
				if (Col[selector].on)
					selector++;
				itemSelector = -1;
			}
		}
	}
	if (GetAsyncKeyState(VK_RIGHT) & 1)
	{
		if (itemSelector == -1)
			Col[selector].on = true;
		if (Col[selector].on &&  Col[selector].hk[itemSelector].value < Col[selector].hk[itemSelector].maxVal)
			Col[selector].hk[itemSelector].value++;

	}
	if (GetAsyncKeyState(VK_LEFT) & 1)
	{
		if (itemSelector == -1)
			Col[selector].on = false;

		if (Col[selector].on &&  Col[selector].hk[itemSelector].value > 0)
			Col[selector].hk[itemSelector].value--;
	}
}

HRESULT WINAPI hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (g_pDevice == NULL) g_pDevice = pDevice;

	if (g_pTexRed == NULL)	D3DXCreateTextureFromFileInMemory(pDevice, (LPCVOID)&bRed, sizeof(bRed), &g_pTexRed);
	if (g_pTexBlue == NULL)	D3DXCreateTextureFromFileInMemory(pDevice, (LPCVOID)&bBlue, sizeof(bBlue), &g_pTexBlue);
	if (g_pTexGreen == NULL)	D3DXCreateTextureFromFileInMemory(pDevice, (LPCVOID)&bGreen, sizeof(bGreen), &g_pTexGreen);

	if (g_pFont == NULL) D3DXCreateFont(pDevice, fontSize, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Lao UI", &g_pFont);

	if (g_Line == NULL) D3DXCreateLine(pDevice, &g_Line);

	pDevice->GetViewport(&g_ViewPort);



	if (g_pFont != NULL && g_Line != NULL) {
		if (isFirstRun)
		{
			menu.AddColum(0, "All WH");

			menu.AddItem(0, 0, "Polygons", 0);


			isFirstRun = false;
		}
		menu.BuildMenu("My Menu", MenuX, MenuY, MenuW);
		menu.KeyboardEvents();
		menu.StartHack();

	}
	return oEndScene(pDevice);
}

HRESULT WINAPI hkDrawIndexedPrimitive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT PrimitiveCount)
{
	__asm nop

	HRESULT hRet = oDrawIndexedPrimitive(pDevice, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, PrimitiveCount);
	return hRet;
}

HRESULT WINAPI hkSetStreamSource(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
	__asm nop

	if (StreamNumber == 0) {
		uiStride = Stride;
	}

	return oSetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride);
}

LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void DX_Init(DWORD *table)
{
	WNDCLASSEX wc = 
	{ 
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "DX", NULL 
	};
	RegisterClassEx(&wc);
	
	HWND hWnd = CreateWindow("DX", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, GetDesktopWindow(), NULL, wc.hInstance, NULL);
	
	LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	LPDIRECT3DDEVICE9 pd3dDevice;

	pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice);
	
	DWORD* pVTable = (DWORD*)pd3dDevice;
	pVTable = (DWORD*)pVTable[0];

	table[ES] = pVTable[42];
	table[DIP] = pVTable[82];
	table[SSS] = pVTable[100];

	DestroyWindow(hWnd);
}

DWORD WINAPI MyThread(LPVOID)
{
	DWORD VTable[3] = { 0 };

	while (GetModuleHandle("d3d9.dll") == NULL)
	{
		Sleep(250);
	}

	DX_Init(VTable);

	HOOK(EndScene, VTable[ES]);
	HOOK(DrawIndexedPrimitive, VTable[DIP]);
	HOOK(SetStreamSource, VTable[SSS]);

	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, MyThread, 0, 0, 0);
	}

	return TRUE;
}
