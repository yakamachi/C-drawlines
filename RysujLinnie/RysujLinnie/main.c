#include <windows.h>
#include <math.h>

LPSTR mainWindowClassName = "Main Window";
MSG message;
int x = -1;
int y = -1;

int opcja = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int RysujLiniePrzyrost(int, int, int, int, HDC);
int RysujLinieBresenham(int, int, int, int, HDC);
int RysujLinie(int, int, int, int, HDC);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = mainWindowClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Rejestracja okna nie powiod³a siê", "Cudowny Windows",
			MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}

	HWND hwnd;

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, mainWindowClassName, "Rysowanie linni", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Okno odmówi³o przyjœcia na œwiat!", "Ale kicha...", MB_ICONEXCLAMATION);
		return 1;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONUP:
		if (x == -1 && y == -1)
		{
			POINT p;
			if (GetCursorPos(&p))
			{
				if (ScreenToClient(hwnd, &p))
				{
					x = p.x;
					y = p.y;
				}
			}

			SetPixel(GetDC(hwnd), x, y, RGB(0, 0, 0));
		}
		else
		{

			HDC canvas = GetDC(hwnd);
			COLORREF color = RGB(0, 0, 0);
			int tmpx = x;
			int tmpy = y;
			POINT p;
			if (GetCursorPos(&p))
			{
				if (ScreenToClient(hwnd, &p))
				{
					x = p.x;
					y = p.y;
				}
			}
			SetPixel(canvas, x, y, color);

			RysujLinie(tmpx, tmpy, x, y, canvas);

			ReleaseDC(hwnd, canvas);
		}
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int RysujLiniePrzyrost(int x1, int y1, int x2, int y2, HDC canvas)
{
	float dy, dx, xrys, yrys, m;

	dy = y2 - y1;
	dx = x2 - x1;
	if (abs(dx) > abs(dy))
	{
		m = dy/dx;
		yrys = y1;
		xrys = x1;
		int warunek = x2;
		if (x1 > x2)
		{
			xrys = x2;
			yrys = y2;
			warunek = x1;
		}

			for (; xrys <= warunek; xrys++)
			{
				SetPixel(canvas, xrys, floor(yrys + 0.5), RGB(0, 0, 0));
				yrys += m;
			}
	}
	else
	{
		m = dx / dy;
		xrys = x1;
		yrys = y1;
		int warunek = y2;
		if (y1 > y2)
		{
			warunek = y1;
			yrys = y2;
			xrys = x2;
		}
			for (; yrys <= warunek; yrys++)
			{
				SetPixel(canvas, floor(xrys + 0.5) , yrys, RGB(0, 0, 0));
				xrys += m;
			}
	}
	return 0;
}

int RysujLinieBresenham(int x1, int y1, int x2, int y2, HDC canvas)
{
	int kx, ky, xrys, yrys, dx, dy, e;
	if (x1 > x2) 
	{
		kx = -1;
	}
	else
	{
		kx = 1;
	}
	if (y1 > y2)
	{
		ky = -1;
	}
	else
	{
		ky = 1;
	}

	dx = abs(x2 - x1);
	dy = abs(y2 - y1);

	if (dx >= dy)
	{
		e = dx / 2;
		xrys = x1;
		yrys = y1;
		for (size_t i = 0; i < dx; i++)
		{
			xrys = xrys + kx;
			e = e - dy;
			if (e < 0)
			{
				yrys = yrys + ky;
				e = e + dx;
			}
			SetPixel(canvas, xrys, yrys, RGB(0, 0, 0));
		}
	}
	else
	{
		e = dy / 2;
		xrys = x1;
		yrys = y1;
		for (size_t i = 0; i < dy; i++)
		{
			yrys = yrys + ky;
			e = e - dx;
			if (e < 0)
			{
				xrys = xrys + kx;
				e = e + dy;
			}
			SetPixel(canvas, xrys, yrys, RGB(0, 0, 0));
		}
	}

	return 0;
}

int RysujLinie(int x1, int y1, int x2, int y2, HDC canvas)
{
	if (opcja == 0)
	{
		RysujLiniePrzyrost(x1, y1, x2, y2, canvas);
		opcja += 1;
	}
	else
	{
		RysujLinieBresenham(x1, y1, x2, y2, canvas);
		opcja -= 1;
	}
	return 0;
}