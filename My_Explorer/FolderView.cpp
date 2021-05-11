#include "Interface.h"


//-----FolderView's METHODS-----//

FolderView::FolderView() :
	currDir(nullptr),
	hListBox(nullptr),
	hwndParent(nullptr),
	hInst(NULL),
	columns({ "Name", "Type", "Date" , "Size" }),
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
	for (string column : this->columns)
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

		for (size_t i = 1; i < elem.size(); i++)
			ListView_SetItemText(this->hListBox, index, i, (LPSTR)elem[i].c_str());

		index++;
	}

	return true;
}

void FolderView::FillListViewTab()
{

	this->listViewTab.clear();

	// Adding directories to listViewTab
	for (string subdir : this->currDir->getDirs())
	{
		this->listViewTab.push_back({ subdir, "Folder", "" });
	}

	// Adding files to listViewTab
	for (File file : this->currDir->getFiles())
	{
		char date[100];

		sprintf_s(date, "%02d.%02d.%04d", file.time.wDay, file.time.wMonth, file.time.wYear);

		this->listViewTab.push_back({ file.name, file.extension, date, file.size });
	}
}

//----------------------------------------------// 

FolderView::FolderView(Directory* dir, const RECT& cRect, HWND hwndParent, HINSTANCE hInst) : FolderView()
{
	this->currDir = dir;
	this->hwndParent = hwndParent;
	this->hInst = hInst;

	RECT folderView = cRect;
	Utilities::rectTransform(folderView, 0.75, 0.95, 1, 0.9);
	setRect(folderView);

	CreateListView(hwndParent, hInst);

	InitListViewColumns();

	FillListViewTab();

	InsertListViewItems();
}

void FolderView::setRect(const RECT& rect)
{
	this->area = rect;
}

void FolderView::setDir(Directory& directory)
{
	*(this->currDir) = directory;

	this->updateList();
}

vector<string> FolderView::getElement(unsigned index) const { return this->listViewTab[index]; }

HWND FolderView::getListHandle() const { return this->hListBox; }

Directory* FolderView::getDir() const { return this->currDir; }

// Updating the list of files and directories
void FolderView::updateList()
{
	this->currDir->Update(); // Updating current Directory

	SendMessage(this->hListBox, LVM_DELETEALLITEMS, 0, 0);

	FillListViewTab();

	InsertListViewItems();
}

//-----END OF FolderView's METHODS-----//
