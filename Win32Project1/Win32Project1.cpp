// Win32Project1.cpp: Okreœla punkt wejœcia dla aplikacji.
//

#include "stdafx.h"
#include "Win32Project1.h"


#include <Windows.h>
#include <iostream>
#include <vector>
#include <tuple>

#define M_PI 3.14159265358979323846

using namespace std;


HDC screenDC;
double kat;
double pktX;
double pktY;
vector<tuple<int, int>> wektory;

const int BEGIN_X = 1920 / 2;
const int BEGIN_Y = 1080 / 2;

void init() {
	pktX = BEGIN_X;
	pktY = BEGIN_Y;
	kat = -90;
	wektory.clear();

	screenDC = GetDC(0);
	HPEN pen = CreatePen(PS_SOLID, 3, RGB(255, 128, 128));
	SelectObject(screenDC, pen);
}

void rysuj(double x) {
	pktY += x * sin(kat*M_PI / 180.0);
	pktX += x * cos(kat*M_PI / 180.0);
	wektory.push_back(tuple<int, int>(pktX, pktY));
}

void obroc(double x) {
	kat = fmod(kat - x, 360);
}

void draw(HDC hdc) {
	MoveToEx(hdc, BEGIN_X, BEGIN_Y, NULL);
	for (int i = 0; i < wektory.size(); i++) {
		//Sleep(100);
		LineTo(hdc, get<0>(wektory[i]), get<1>(wektory[i]));
	}
}

//KOD DO RYSOWANIA
//      TUTAJ
//  \/\/\/\/\/\/
//   \/\/\/\/\/
//    \/\/\/\/
//     \/\/\/
//      \/\/
//       \/

void f(double x) {
	if (x < 8) return;

	rysuj(x);
	obroc(90);
	rysuj(x);
	obroc(90);
	rysuj(x);
	obroc(90);
	rysuj(x);
	obroc(90);
	rysuj(x / 2);
	obroc(45);
	f(x*sqrt(2) / 2);

}

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	init();



	//POCZATKOWA WARTOSC
	//      TUTAJ
	//  \/\/\/\/\/\/
	//   \/\/\/\/\/
	//    \/\/\/\/
	//     \/\/\/
	//      \/\/
	//       \/
	
	f(50);


	

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));
	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		draw(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
