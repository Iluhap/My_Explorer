#include "Interface.h"

#include "resource.h"

#define _WIN32_IE 0x0400
#include "commctrl.h"

inline HWND Folder::CreateListView(HWND hwndParent, HINSTANCE hInst)
{
	INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	RECT rcClient;                       // The parent window's client area.

	GetClientRect(hwndParent, &rcClient);

	// Create the list-view window in report view with label editing enabled.
	HWND hWndListView = CreateWindow(WC_LISTVIEW,
		"",
		WS_CHILD | LVS_REPORT | LVS_EDITLABELS,
		0, 0,
		rcClient.right - rcClient.left,
		rcClient.bottom - rcClient.top,
		hwndParent,
		(HMENU)NULL,
		hInst,
		NULL);

	return (hWndListView);
}
inline VOID Folder::SetView(HWND hWndListView, DWORD dwView)
{
	// Retrieve the current window style. 
	DWORD dwStyle = GetWindowLong(hWndListView, GWL_STYLE);

	// Set the window style only if the view bits changed.
	if ((dwStyle & LVS_TYPEMASK) != dwView)
	{
		SetWindowLong(hWndListView,
			GWL_STYLE,
			(dwStyle & ~LVS_TYPEMASK) | dwView);
	}               // Logical OR'ing of dwView with the result of 
}                   // a bitwise AND between dwStyle and 
inline BOOL Folder::InitListViewImageLists(HWND hWndListView, HINSTANCE hInst)
{
	HICON hiconItem;     // Icon for list-view items.
	HIMAGELIST hLarge;   // Image list for icon view.
	HIMAGELIST hSmall;   // Image list for other views.

	// Create the full-sized icon image lists. 
	hLarge = ImageList_Create(GetSystemMetrics(SM_CXICON),
		GetSystemMetrics(SM_CYICON),
		ILC_MASK, 1, 1);

	hSmall = ImageList_Create(GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		ILC_MASK, 1, 1);

	// Add an icon to each image list.  
	hiconItem = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON));

	ImageList_AddIcon(hLarge, hiconItem);
	ImageList_AddIcon(hSmall, hiconItem);

	DestroyIcon(hiconItem);

	// When you are dealing with multiple icons, you can use the previous four lines of 
	// code inside a loop. The following code shows such a loop. The 
	// icons are defined in the application's header file as resources, which 
	// are numbered consecutively starting with IDS_FIRSTICON. The number of 
	// icons is defined in the header file as C_ICONS.
	/*
		for(index = 0; index < C_ICONS; index++)
		{
			hIconItem = LoadIcon (g_hinst, MAKEINTRESOURCE(IDS_FIRSTICON + index));
			ImageList_AddIcon(hSmall, hIconItem);
			ImageList_AddIcon(hLarge, hIconItem);
			Destroy(hIconItem);
		}
	*/

	// Assign the image lists to the list-view control. 
	ListView_SetImageList(hWndListView, hLarge, LVSIL_NORMAL);
	ListView_SetImageList(hWndListView, hSmall, LVSIL_SMALL);

	return TRUE;
}


inline Folder::Folder(Directory* dir, RECT& rt)
{
	this->currDir = dir;

	this->area = rt;
}

inline void Folder::create(HWND hWnd ,HINSTANCE hInst)
{

	// TODO import Comctl32.dll to project

	/*
	CreateListView(hWnd, hInst);

	InitListViewImageLists(this->hListBox, hInst);
	*/



}

inline void Folder::setRect(const RECT& rect)
{
	this->area = rect;
}

inline void Folder::setDir(Directory& directory)
{
	*this->currDir = directory;

	this->updateList();
}

inline void Folder::updateDir()
{
	this->currDir->Update();
	this->updateList();
}

inline void Folder::updateList()
{
	SendMessage(this->hListBox, LB_RESETCONTENT, 0, 0);

	std::string str = "";
	for (std::string dir : this->currDir->getDirs())
	{
		str = dir + "     Folder";

		SendMessage(this->hListBox, LB_ADDSTRING, 0, (LPARAM)str.c_str());
	}

	str = "";
	for (FileSystem::File file : this->currDir->getFiles())
	{
		str = file.name + "    File    " + file.extension;

		SendMessage(this->hListBox, LB_ADDSTRING, 0, (LPARAM)str.c_str());
	}
}
