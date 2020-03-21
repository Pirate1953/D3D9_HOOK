#pragma once	//Директива препроцессора для одиночного подключения файла
#ifndef HEADER_H	//Условная директива, которая проверяет не было ли значение HEADER_H определено ранее
#define HEADER_H	//Директива препроцессора, которая определяет константу HEADER_H

//Директивы препроцессора для автоматического определения библиотек во время компоновки
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <fstream>
#include <detours.h>

////////// НАСТРОЙКИ ДЛЯ МЕНЮ ///////////////////////////////////

int fontSize = 20;	//Размер шрифта

int MenuX = 0;	//Начальная позиция по оси X
int MenuY = 0;	//Начальная позиция по оси Y

int MenuW = 165;	//Ширина меню

bool isMenuOn = true;	//Флаг для проверки "открыто ли меню во время инжекта"

/**
	Объявляем тип как указатели на метод
	tSetStreamSource - указатель на метод SetStreamSource из абстрактного класса, который принимает определённые параметры и возвращает HRESULT
	Объявялем указатель, принадлежащий типу tSetStreamSource
*/
typedef HRESULT(WINAPI *tSetStreamSource)(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9 *pStreamData, UINT OffsetInBytes, UINT Stride);
tSetStreamSource oSetStreamSource;

typedef HRESULT(WINAPI *tEndScene)(LPDIRECT3DDEVICE9 pDevice);
tEndScene oEndScene;

typedef HRESULT(WINAPI *tDrawIndexedPrimitive)(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
tDrawIndexedPrimitive oDrawIndexedPrimitive;

unsigned int uiStride = NULL;

//////////// ИНИЦИАЛИЗИРУЕМ УКАЗАТЕЛИ НА АБСТРАКТНЫЕ КЛАССЫ /////////////////////////////

LPDIRECT3DDEVICE9 g_pDevice = NULL;
LPD3DXLINE g_Line = NULL;
LPD3DXFONT g_pFont = NULL;

LPDIRECT3DTEXTURE9 g_pTexRed = NULL;
LPDIRECT3DTEXTURE9 g_pTexBlue = NULL;
LPDIRECT3DTEXTURE9 g_pTexGreen = NULL;

D3DVIEWPORT9 g_ViewPort;

//////////////// ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ ///////////////////////

int selector = 0;	//Идентификатор сегмента
int itemSelector = -1;

//Для меню
int MenuH = 0;

bool isFirstRun = true;	//Флаг для проверки "Первый ли запуск"
bool isMouseOnButton = false;	//Флаг для проверки "На кнопке ли курсор мыши"
bool isMenuMoving = false;	//Флаг для провекри "Может ли меню двигаться"

/**
	Ставим псевдоним HOOK на функцию DetourFunction (v1.5) или DetourAttach (новая версия)
	DetourFunction принимает два параметра с типом (указателем на тип BYTE) и возвращает также указатель на тип BYTE
	нужна для обхода кода программы в область для исполнения нашего кода
*/
#define HOOK(func, addy) o##func = (t##func)DetourFunction((PBYTE)addy, (PBYTE)hk##func)

///////////////////// ОПРЕДЕЛЯЕМ КОНСТАНТЫ /////////////////////////////////////

#define ES 0
#define DIP	1
#define SSS	2

///////////////// ОПРЕДЕЛЕНИЕ МАКРОСОВ ДЛЯ ЦВЕТА ЧЕРЕЗ D3DCOLOR_ARGB /////////////////////////////

#define RED D3DCOLOR_ARGB(255, 255, 0, 0)
#define GREEN D3DCOLOR_ARGB(255, 0, 255, 0)
#define BLUE D3DCOLOR_ARGB(255, 0, 0, 255)
#define WHITE D3DCOLOR_ARGB(255, 255, 255, 255)
#define BLACKTransparent D3DCOLOR_ARGB(220, 0, 0, 0)
#define BLACK D3DCOLOR_ARGB(255, 0, 0, 0)
#define YELLOW D3DCOLOR_ARGB(255, 255, 255, 0)
#define TEAL D3DCOLOR_ARGB(255, 0, 255, 255)
#define PINK D3DCOLOR_ARGB(255, 255, 240, 0)
#define ORANGE D3DCOLOR_ARGB(255, 255, 132, 0)
#define LIME D3DCOLOR_ARGB(255, 198, 255, 0)
#define SKYBLUE D3DCOLOR_ARGB(255, 0, 180, 255)
#define MAROON D3DCOLOR_ARGB(255, 142, 30, 0)
#define LGRAY D3DCOLOR_ARGB(255, 174, 174, 174) 
#define DGRAY D3DCOLOR_ARGB(255, 71, 65, 64) 
#define BROWN D3DCOLOR_ARGB(255, 77, 46, 38)
#define SHIT D3DCOLOR_ARGB(255, 74, 38, 38)

/////////////// ОБЪЯВЛЕНИЕ КОНСТАНТ (МАССИВОВ) ТИПА UNSIGNED CHAR ДЛЯ ЦВЕТА ///////////////////////

const BYTE bRed[58] = {
	0x42, 0x4D, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00
};

const BYTE bGreen[60] = {
	0x42, 0x4D, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x12, 0x0B, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x20, 0xA0, 0x00, 0x00, 0xFF, 0xFF
};

const BYTE bBlue[60] = {
	0x42, 0x4D, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x12, 0x0B, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0x00, 0x00, 0x00
};

////////////////////// ОБЪЯВЛЯЕМ СТРУКТУРУ HACK ////////////////////////////

struct HACK
{
	int id;
	bool isMouseOver = false;
	int value = 0;
	int type;
	char *Name;
	int maxVal = 0;
};

//////////// ОБЪЯВЛЯЕМ СТРУКТУРУ ДЛЯ СТОЛБЦОВ МЕНЮ //////////////////////

struct Colum
{
	int y = 0;
	bool isMouseOver = false;
	bool on = false;
	int hacks = 0;
	char *Name;
	HACK hk[15];	//Объявляем массив структур HACK
};

/////////////////// ОБЪЯВЛЯЕМ КЛАСС TpMenu //////////////////////////

class TpMenu
{
public:
	int colCount = 0;	//Число столбцов
	int selector = 0;	//Селектор

	////////////////// ОБЪЯВЛЯЕМ МАССИВЫ СТРУКТУР ЧЕРЕЗ ОБЪЕКТЫ СТРУКТУР //////////////////

	Colum Col[10];
	HACK Hack[15];

	//////////////// ПРОТОТИПЫ МЕТОДОВ КЛАССА ////////////////////

	void AddColum(int id, char *name);	//Метод добавления столбца
	void AddItem(int colum, int id, char *name, int type);	//Метод добавления элемента
	void BuildMenu(char *name, int x, int y, int w);	//Метод постройки меню
	void KeyboardEvents();	//Обработчки событий клавиатуры
	void StartHack();	//Пуск (начать взлом)
};

#endif	//конец условия