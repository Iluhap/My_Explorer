#include "Interface.h"

#pragma comment(lib, "comctl32.lib")
#include "commctrl.h"


FolderView::FolderView() :
	currDir(nullptr),
	hListBox(nullptr),
	columns({ "Name", "Type", "Date" }),
	area({ 0,0,0,0 }) {}

void FolderView::CreateListView(HWND hwndParent, HINSTANCE hInst)
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

bool FolderView::InitListViewColumns()
{
	// Initialize the LVCOLUMN structure.
	// The mask specifies that the format, width, text,
	// and subitem members of the structure are valid.

	LVCOLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.cx = 150;	// Width of column in pixels.
	lvc.fmt = LVCFMT_LEFT;  // Left-aligned column

	int iCol = 0;

	// Add the columns.
	for (std::string column : this->columns)
	{
		lvc.iSubItem = iCol;
		lvc.pszText = (LPSTR)(column.c_str());

		// Insert the columns into the list view.
		if (ListView_InsertColumn(this->hListBox, iCol++, &lvc) == -1)
			return false;
	}
	return true;
}

bool FolderView::InsertListViewItems() // TODO implement adding other info about items
{
	LVITEM lvI;

	unsigned index = 0;

	// Initialize LVITEM members that are common to all items.
	lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvI.stateMask = 0;
	lvI.iSubItem = 0;
	lvI.state = 0;

	// Initialize LVITEM members that are different for each item.

	for (auto elem : this->listViewTab)
	{
		lvI.iItem = index;
		lvI.iImage = index;

		lvI.pszText = (LPSTR)(elem[0].c_str());

		// Insert items into the list.
		if (ListView_InsertItem(this->hListBox, &lvI) == -1)
			return false;

		for (int i = 1; i < elem.size(); i++)
			ListView_SetItemText(this->hListBox, index, i, (LPSTR)elem[i].c_str());

		index++;
	}

	return true;
}

void FolderView::FillListViewTab()
{
	// Adding directories to listViewTab
	for (std::string subdir : this->currDir->getDirs())
	{
		this->listViewTab.push_back({ subdir, "Folder", "" });
	}

	// Adding files to listViewTab
	for (File file : this->currDir->getFiles())
	{
		char date[100];

		sprintf_s(date, "%d.%d.%d", file.time.wDay, file.time.wMonth, file.time.wYear);

		this->listViewTab.push_back({ file.name, file.extension, date });
	}
}

void FolderView::rectTransform(RECT& cRect, double left_scale,
	double top_scale,
	double right_scale,
	double bottom_scale)
{
	size_t xSize = (cRect.right - cRect.left);
	size_t ySize = (cRect.bottom - cRect.top);

	cRect =
	{
		(LONG)round(cRect.left + xSize * (1 - left_scale)),
		(LONG)round(cRect.top + ySize * (1 - top_scale)),
		(LONG)round(cRect.right - (xSize * (1 - right_scale))),
		(LONG)round(cRect.bottom - (ySize * (1 - bottom_scale)))
	};
}

//----------------------------------------------// 

FolderView::FolderView(Directory* dir, const RECT& cRect) : FolderView()
{
	this->currDir = dir;

	RECT folderView = cRect;
	rectTransform(folderView, 0.75, 0.95, 1, 0.9);
	setListViewRect(folderView);
}

void FolderView::Create(HWND hWnd, HINSTANCE hInst)
{
	CreateListView(hWnd, hInst);

	InitListViewColumns();

	FillListViewTab();

	InsertListViewItems();

}

void FolderView::setListViewRect(const RECT& rect)
{
	this->area = rect;
}

void FolderView::setDir(Directory& directory)
{
	*(this->currDir) = directory;

	this->updateList();
}

// Updating the list of files and directories
void FolderView::updateList()
{

	this->currDir->Update(); // Updating current Directory

	InsertListViewItems();

}
