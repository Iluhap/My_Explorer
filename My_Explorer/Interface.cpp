#include "Interface.h"

#pragma comment(lib, "comctl32.lib")
#include "commctrl.h"

#include "resource.h"


//-----FolderView's METHODS-----//

FolderView::FolderView() :
	currDir(nullptr),
	hListBox(nullptr),
	hwndParent(nullptr),
	hInst(NULL),
	columns({ "Name", "Type", "Date" , "Size"}),
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
	setListViewRect(folderView);

	CreateListView(hwndParent, hInst);

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


//-----FolderView's METHODS-----//

// Private methods

void FolderTree::CreateTreeView(HWND hWnd, HINSTANCE hInst)
{
	// Ensure that the common control DLL is loaded. 
	INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	// Get the dimensions of the parent window's client area, and create 
	// the tree-view control. 

	this->hTreeView = CreateWindow(WC_TREEVIEW,
		TEXT("Tree View"),
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES,
		this->area.left, // X
		this->area.top, // Y
		this->area.right - this->area.left, // Width
		this->area.bottom - this->area.top, // Height
		hWnd,
		(HMENU)NULL,
		hInst,
		NULL);
}

// End of private methods

FolderTree::FolderTree()
	:
	hTreeView(nullptr),
	hwndParent(nullptr),
	hInst(NULL),
	area({0,0,0,0})
{
	vector<string> drives = Utilities::listDrives();

	for (string drive_name : drives)
	{
		this->directories.push_back(Directory(drive_name, nullptr));
	}
}

FolderTree::FolderTree(HWND hWnd, HINSTANCE hInst) : FolderTree()
{
	this->hwndParent = hWnd;
	this->hInst = hInst;

	GetClientRect(hWnd, &this->area);
	Utilities::rectTransform(this->area, 1, 0.95, 0.25, 0.9);

	CreateTreeView(hWnd, hInst);
}

FolderTree::~FolderTree()
{
}

void FolderTree::setRect(const RECT& cRect)
{
	Utilities::rectTransform(this->area, 0, 0, 0.25, 0);
}

//-----END OF FolderTree's METHODS-----//

//-----BUTTONS'S METHODS-----//

string Buttons::edit_control_text = ""; // STATIC VARIABLE OF BUTTONS

LRESULT Buttons::DlgFunc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hEdit;
	CHAR s_text[256] = { 0 };

	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:

			hEdit = GetDlgItem(hDlg, IDC_EDIT1);

			SendMessage(hEdit, WM_GETTEXT, (WPARAM)255, (LPARAM)s_text);

			Buttons::edit_control_text = s_text;

			MessageBox(hDlg, s_text, "Output", MB_OK);

			EndDialog(hDlg, LOWORD(wParam));

			return TRUE;

		case IDCANCEL:

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}
	return FALSE;
}

Buttons::Buttons(HWND hParent, HINSTANCE hInst)
{
	vector<string> names = {"Open", "Copy", "Move", "Delete"}; // Names of buttons
	vector<buttonHandler> functions = { OpenHandler , CopyHandler, MoveHandler, DeleteHandler}; // Handlers functions for buttons

	RECT rt;
	GetClientRect(hParent, &rt);
	Utilities::rectTransform(rt, 0.70, 0.1, 1, 1); // Preparing area for buttons

	for (size_t i = 0; i < names.size(); i++)
	{
		size_t left = rt.left + (i) * (rt.right - rt.left) / names.size(); // Calculating position for current button

		HWND hButton = CreateWindow("button", names[i].c_str(),
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			left, rt.top + 10,
			70,25,
			hParent, (HMENU)NULL, hInst, NULL);
		this->buttons.push_back({ hButton, functions[i]});
	}
}

void Buttons::Handler(LPARAM lParam, FolderView* pFolderView)
{
	HWND hPressedButton = (HWND)lParam;

	for (Button button : this->buttons)
	{
		if (button.handle == hPressedButton)
			button.pFunction(pFolderView);
	}
}

void Buttons::OpenHandler(FolderView* pFolderView)
{
	HWND hList = pFolderView->getListHandle();

	unsigned id = (unsigned)SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

	if (id < 0 or id >= pFolderView->listViewTab.size())
		MessageBox(NULL, "You should choose element", "Open ERROR", MB_ICONWARNING);
	else
	{
		vector<string> elem = pFolderView->getElement(id);

		string name = elem[0];
		string type = elem[1];

		if (type == "Folder")
		{
			/*
			* TODO implement opening of directory
			*/
		}
		else
			Utilities::openFile(pFolderView->getDir()->getPath() + "\\" + name); // Opening of file
	}
}
void Buttons::CopyHandler(FolderView* pFolderView)
{
	HWND hList = pFolderView->getListHandle();

	unsigned id = (unsigned)SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

	if (id < 0 or id >= pFolderView->listViewTab.size())
		MessageBox(NULL, "You should choose element", "Open ERROR", MB_ICONWARNING);
	else
	{
		vector<string> elem = pFolderView->getElement(id);

		string name = elem[0];
		string type = elem[1];

		DialogBox(pFolderView->hInst, MAKEINTRESOURCE(IDD_DIALOG), pFolderView->hwndParent, (DLGPROC)&Buttons::DlgFunc);

		if (type == "Folder")
			Utilities::copyDirectory(pFolderView->getDir(), Buttons::edit_control_text);
		else
			Utilities::copyFile(pFolderView->getDir()->getPath() + "\\" + name, Buttons::edit_control_text); 

		Buttons::edit_control_text = "";
		pFolderView->updateList();
	}
}
void Buttons::MoveHandler(FolderView* pFolderView)
{
	HWND hList = pFolderView->getListHandle();

	unsigned id = (unsigned)SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

	if (id < 0 or id >= pFolderView->listViewTab.size())
		MessageBox(NULL, "You should choose element", "Open ERROR", MB_ICONWARNING);
	else
	{
		vector<string> elem = pFolderView->getElement(id);

		string name = elem[0];
		string type = elem[1];

		DialogBox(pFolderView->hInst, MAKEINTRESOURCE(IDD_DIALOG), pFolderView->hwndParent, (DLGPROC)&Buttons::DlgFunc);

		if (type == "Folder")
			Utilities::moveDirectory(pFolderView->getDir(), Buttons::edit_control_text);
		else
			Utilities::changeFileName(pFolderView->getDir()->getPath() + "\\" + name, Buttons::edit_control_text);

		Buttons::edit_control_text = "";
		pFolderView->updateList();
	}
}
void Buttons::DeleteHandler(FolderView* pFolderView)
{
	HWND hList = pFolderView->getListHandle();

	unsigned id = (unsigned)SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

	if (id < 0 or id >= pFolderView->listViewTab.size())
		MessageBox(NULL, "You should choose element", "Open ERROR", MB_ICONWARNING);
	else
	{
		vector<string> elem = pFolderView->getElement(id);

		string name = elem[0];
		string type = elem[1];

		string path = pFolderView->getDir()->getPath() + "\\" + name;

		Directory tmpDir = Directory(path, nullptr);

		if (type == "Folder")
			Utilities::deleteDirectory(&tmpDir);
		else
			Utilities::deleteFile(path);

		pFolderView->updateList(); // Updating of FolderView
	}
}

//-----END OF BUTTONS'S METHODS-----//
