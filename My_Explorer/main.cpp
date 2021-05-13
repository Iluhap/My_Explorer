#include <iostream>
#include "Interface.h"

#pragma comment(lib, "comctl32.lib")
#include "commctrl.h"

// Declaration of functions 
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void InitComponents(HWND hWnd, HINSTANCE hInst);
void ReleaseComponents();

//--- Global variables ---//

HINSTANCE hInst; // Program descriptor
LPCTSTR szWindowClass = "EXPLORER";
LPCTSTR szTitle = "MyExplorer";

FolderView* pFolderView;

FolderTree* pFolderTree;

Buttons* pButtons;

//--- End of variables ---//


// Main program
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	setlocale(LC_ALL, "rus");

	MSG msg;
	// Реєстрація класу вікна
	MyRegisterClass(hInstance);
	// Створення вікна програми
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	// Цикл обробки повідомлень
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; //стиль вікна
	wcex.lpfnWndProc = (WNDPROC)WndProc; //віконна процедура
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance; //дескриптор програми
	wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO); //визначення іконки
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //визначення курсору
	wcex.hbrBackground = GetSysColorBrush(COLOR_WINDOW + 1); //установка фону
	wcex.lpszMenuName = NULL; //визначення меню
	wcex.lpszClassName = szWindowClass; //ім’я класу
	wcex.hIconSm = NULL;
	return RegisterClassEx(&wcex); //реєстрація класу вікна
}

// FUNCTION: InitInstance (HANDLE, int)
// Створює вікно програми і зберігає дескриптор програми в змінній hInst
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	UINT sizeX = (UINT)round(GetSystemMetrics(SM_CXSCREEN) / 2);
	UINT sizeY = (UINT)round(GetSystemMetrics(SM_CYSCREEN) / 2);

	HWND hWnd;
	hInst = hInstance; //зберігає дескриптор додатка в змінній hInst
	hWnd = CreateWindow(szWindowClass, // ім’я класу вікна
		szTitle, // назва програми
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // стиль вікна
		CW_USEDEFAULT, // положення по Х
		CW_USEDEFAULT, // положення по Y
		sizeX, // розмір по Х
		sizeY, // розмір по Y
		NULL, // дескриптор батьківського вікна
		NULL, // дескриптор меню вікна
		hInstance, // дескриптор програми
		NULL); // параметри створення.
	if (!hWnd) //Якщо вікно не творилось, функція повертає FALSE
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow); //Показати вікно
	UpdateWindow(hWnd); //Оновити вікно
	return TRUE;
}

// FUNCTION: WndProc (HWND, unsigned, WORD, LONG)
// Віконна процедура. Приймає і обробляє всі повідомлення, що приходять в додаток
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;

	switch (message)
	{
	case WM_CREATE: // Повідомлення приходить при створенні вікна
	{
		InitComponents(hWnd, hInst);

		break;
	}
	case WM_PAINT: // Перемалювати вікно
	{
		hdc = BeginPaint(hWnd, &ps); // Почати графічний вивід
		GetClientRect(hWnd, &rt); // Область вікна для малювання

		EndPaint(hWnd, &ps); // Закінчити графічний вивід
		break;
	}
	case WM_COMMAND:
	{
		pButtons->Handler(lParam, pFolderView);
		break;
	}
	case WM_NOTIFY:
	{
		switch (((LPNMHDR)lParam)->code)
		{
		case TVN_SELCHANGED:
		{
			// MessageBox(NULL, "Notify", "Notify", MB_OK);

			// TODO implement changing of selected folder

			LPNMTREEVIEW pnmtv;

			pnmtv = (LPNMTREEVIEW)lParam;

			pFolderTree->setSelection(pnmtv->itemNew.hItem);

			break;
		}

		case NM_DBLCLK:
		{
			LPNMITEMACTIVATE item = (LPNMITEMACTIVATE)lParam;

			if (item->hdr.hwndFrom == pFolderView->getListHandle()) // Double click for FolderView
				pFolderView->openItem(item->iItem);

			break;
		}
		}
		break;
	}

	case WM_DESTROY: // Завершення роботи
	{
		ReleaseComponents();

		PostQuitMessage(0);
		break;
	}
	default:
		// Обробка повідомлень, які не оброблені користувачем
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void InitComponents(HWND hWnd, HINSTANCE hInst)
{
	// TODO 

	RECT rt;
	GetClientRect(hWnd, &rt);

	pFolderView = new FolderView(hWnd, hInst, rt);

	pButtons = new Buttons(hWnd, hInst);

	pFolderTree = new FolderTree(hWnd, hInst, pFolderView);
}

void ReleaseComponents()
{
	// TODO Release of memory

	delete pButtons;

	delete pFolderTree;

	delete pFolderView;
}
