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
vector<tuple<double, double>> wektory;

const int BEGIN_X = 1920 / 2;
const int BEGIN_Y = 1080 / 2;

void init() {
	pktX = BEGIN_X;
	pktY = BEGIN_Y;
	kat = -90;
	wektory.clear();

	screenDC = GetDC(0);
	HPEN pen = CreatePen(PS_SOLID, 3, RGB(255, 128,128));
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

void draw() {
	MoveToEx(screenDC, BEGIN_X, BEGIN_Y, NULL);
	for (int i = 0; i < wektory.size(); i++) {
		//Sleep(10);
		LineTo(screenDC, get<0>(wektory[i]), get<1>(wektory[i]));
	}
}

void f(double x) {
	if (x < 30*4) return;
	obroc(45);
	rysuj(x);
	f(0.8*x);
	obroc(180);
	rysuj(x);
	obroc(90);
	rysuj(x);
	f(0.8*x);
	obroc(180);
	rysuj(x);
	obroc(225);
}

HBITMAP hBitmap = NULL;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_CREATE:
		//hBitmap = (HBITMAP)LoadImage(hInst, L"c:\\test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		break;
	case WM_PAINT:
		PAINTSTRUCT     ps;
		HDC             hdc;
		BITMAP          bitmap;
		HDC             hdcMem;
		HGDIOBJ         oldBitmap;

		hdc = BeginPaint(hWnd, &ps);

		hdcMem = CreateCompatibleDC(hdc);
		oldBitmap = SelectObject(hdcMem, hBitmap);

		GetObject(hBitmap, sizeof(bitmap), &bitmap);
		BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int main(void) {
	init();
	f(50*4);


	while (true) {
		draw();
	}
	system("pause");
	return 0;
}