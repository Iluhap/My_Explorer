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

void WM_COMMAND_Handler(LPARAM lParam);

//--- Global variables ---//

HINSTANCE hInst; // Program descriptor
LPCTSTR szWindowClass = "EXPLORER";
LPCTSTR szTitle = "MyExplorer";

FolderView* pFolderView;

Directory* directory;

FolderTree* pFolderTree;

Buttons* pButtons;

//--- End of variables ---//


// Main program
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	setlocale(LC_ALL, "rus");

	MSG msg;
	// ��������� ����� ����
	MyRegisterClass(hInstance);
	// ��������� ���� ��������
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	// ���� ������� ����������
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; //����� ����
	wcex.lpfnWndProc = (WNDPROC)WndProc; //������ ���������
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance; //���������� ��������
	wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO); //���������� ������
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //���������� �������
	wcex.hbrBackground = GetSysColorBrush(COLOR_WINDOW); //��������� ����
	wcex.lpszMenuName = NULL; //���������� ����
	wcex.lpszClassName = szWindowClass; //��� �����
	wcex.hIconSm = NULL;
	return RegisterClassEx(&wcex); //��������� ����� ����
}

// FUNCTION: InitInstance (HANDLE, int)
// ������� ���� �������� � ������ ���������� �������� � ������ hInst
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	unsigned sizeX = round(GetSystemMetrics(SM_CXSCREEN) / 2);
	unsigned sizeY = round(GetSystemMetrics(SM_CYSCREEN) / 2);

	HWND hWnd;
	hInst = hInstance; //������ ���������� ������� � ������ hInst
	hWnd = CreateWindow(szWindowClass, // ��� ����� ����
		szTitle, // ����� ��������
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // ����� ����
		CW_USEDEFAULT, // ��������� �� �
		CW_USEDEFAULT, // ��������� �� Y
		sizeX, // ����� �� �
		sizeY, // ����� �� Y
		NULL, // ���������� ������������ ����
		NULL, // ���������� ���� ����
		hInstance, // ���������� ��������
		NULL); // ��������� ���������.
	if (!hWnd) //���� ���� �� ���������, ������� ������� FALSE
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow); //�������� ����
	UpdateWindow(hWnd); //������� ����
	return TRUE;
}

// FUNCTION: WndProc (HWND, unsigned, WORD, LONG)
// ³����� ���������. ������ � �������� �� �����������, �� ��������� � �������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;

	switch (message)
	{
	case WM_CREATE: // ����������� ��������� ��� ��������� ����

		InitComponents(hWnd, hInst);

		break;
	case WM_PAINT: // ������������ ����
		hdc = BeginPaint(hWnd, &ps); // ������ ��������� ����
		GetClientRect(hWnd, &rt); // ������� ���� ��� ���������

		EndPaint(hWnd, &ps); // �������� ��������� ����
		break;

	case WM_COMMAND:
		pButtons->Handler(lParam, pFolderView);
		break;

	case WM_DESTROY: // ���������� ������

		ReleaseComponents();

		PostQuitMessage(0);
		break;
	default:
		// ������� ����������, �� �� ��������� ������������
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void InitComponents(HWND hWnd, HINSTANCE hInst)
{
	// TODO 

	directory = new Directory("C:", nullptr);

	RECT rt;
	GetClientRect(hWnd, &rt);

	pFolderView = new FolderView(directory, rt);

	pFolderView->Create(hWnd, hInst);

	pButtons = new Buttons(hWnd, hInst);

	// pFolderTreeWindow = new FolderTree();
}

void ReleaseComponents()
{
	// TODO Release of memory

	delete directory;

	delete pFolderView;

	delete pButtons;

	// delete pFolderTree;
}
