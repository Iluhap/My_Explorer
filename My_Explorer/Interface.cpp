#pragma comment(lib, "comctl32.lib")


#include "Interface.h"

#include "resource.h"

// #define _WIN32_IE 0x0400
#include "commctrl.h"

// Method used to interact with List-View window //

inline void FolderView::CreateListView(HWND hwndParent, HINSTANCE hInst)
{
	INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	// Create the list-view window in report view with label editing enabled.
	this->hListBox = CreateWindow(WC_LISTVIEW,
		"",
		WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS | WS_BORDER,
		this->area.left, this->area.top,
		(this->area.right - this->area.left),
		(this->area.bottom - this->area.top),
		hwndParent,
		(HMENU)NULL,
		hInst,
		NULL);
}

inline bool FolderView::InitListViewColumns()
{
	LVCOLUMN lvc;
	int iCol = 0;

	// Initialize the LVCOLUMN structure.
	// The mask specifies that the format, width, text,
	// and subitem members of the structure are valid.
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	// Add the columns.
	for (std::string column : this->columns)
	{
		lvc.iSubItem = iCol;
		lvc.pszText = (LPSTR)(column.c_str());
		lvc.cx = 100;               // Width of column in pixels.

			lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
			// lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.

		// Insert the columns into the list view.
		if (ListView_InsertColumn(this->hListBox, iCol++, &lvc) == -1)
			return false;
	}

	return true;
}

inline bool FolderView::InsertListViewItems() // TODO implement adding other info about items
{
	LVITEM lvI;

	// Initialize LVITEM members that are common to all items.
	lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvI.stateMask = 0;
	lvI.iSubItem = 3;
	lvI.state = 0;

	std::vector<std::string> dirs = this->currDir->getDirs(); // List of directories

	std::vector<File> files = this->currDir->getFiles(); // List of files

	size_t index = 0;

	// Initialize LVITEM members that are different for each item.
	for (std::string dirName : dirs)
	{
		lvI.iItem = index;
		lvI.iImage = index;

		lvI.pszText = (LPSTR)(dirName.c_str());

		// Insert items into the list.
		if (ListView_InsertItem(this->hListBox, &lvI) == -1)
			return false;

		index++;
	}

	for (File file : files)
	{
		lvI.iItem = index;
		lvI.iImage = index;

		lvI.pszText = (LPSTR)(file.name.c_str());

		// Insert items into the list.
		if (ListView_InsertItem(this->hListBox, &lvI) == -1)
			return false;

		index++;
	}

	return true;
}

//----------------------------------------------// 

inline FolderView::FolderView(Directory* dir, const RECT& cRect)
{
	this->currDir = dir;

	setRect(RECT{ (int)round(cRect.right * 0.3), cRect.top, cRect.right, cRect.bottom });
}

inline void FolderView::Create(HWND hWnd ,HINSTANCE hInst)
{

	// TODO Implement creating of list view

	CreateListView(hWnd, hInst);

	InitListViewColumns();

	InsertListViewItems();

}

inline void FolderView::setRect(const RECT& rect)
{
	this->area = rect;
	// TODO implement changing size of List-View
}

inline void FolderView::setDir(Directory& directory)
{
	*(this->currDir) = directory;

	this->updateList();
}

// Updating the list of files and directories
inline void FolderView::updateList()
{

	this->currDir->Update(); // Updating current Directory

	InsertListViewItems();

}
