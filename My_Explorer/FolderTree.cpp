#include "Interface.h"

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

void FolderTree::AddItemToTree(LPTSTR lpszItem, int nLevel)
{
	TVITEM tvi;
	TVINSERTSTRUCT tvins;
	static HTREEITEM hPrev = (HTREEITEM)TVI_FIRST;
	static HTREEITEM hPrevRootItem = NULL;
	static HTREEITEM hPrevLev2Item = NULL;
	HTREEITEM hti;

	tvi.mask = TVIF_TEXT | TVIF_PARAM;

	// Set the text of the item. 
	tvi.pszText = lpszItem;
	tvi.cchTextMax = sizeof(tvi.pszText) / sizeof(tvi.pszText[0]);

	// Assume the item is not a parent item, so give it a 
	// document image. 
	// tvi.iImage = g_nDocument;
	// tvi.iSelectedImage = g_nDocument;

	// Save the heading level in the item's application-defined 
	// data area. 
	tvi.lParam = (LPARAM)nLevel;
	tvins.item = tvi;
	tvins.hInsertAfter = hPrev;

	// Set the parent item based on the specified level. 
	if (nLevel == 1)
		tvins.hParent = TVI_ROOT;
	else if (nLevel == 2)
		tvins.hParent = hPrevRootItem;
	else
		tvins.hParent = hPrevLev2Item;

	// Add the item to the tree-view control. 
	hPrev = (HTREEITEM)SendMessage(this->hTreeView, TVM_INSERTITEM,
		0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);

	if (hPrev == NULL)
		return;

	// Save the handle to the item. 
	if (nLevel == 1)
		hPrevRootItem = hPrev;
	else if (nLevel == 2)
		hPrevLev2Item = hPrev;

	// The new item is a child item. Give the parent item a 
	// closed folder bitmap to indicate it now has child items. 
	if (nLevel > 1)
	{
		hti = TreeView_GetParent(this->hTreeView, hPrev);
		tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvi.hItem = hti;
		// tvi.iImage = g_nClosed;
		// tvi.iSelectedImage = g_nClosed;
		TreeView_SetItem(this->hTreeView, &tvi);
	}
}

BOOL FolderTree::InitTreeViewItems()
{
	HTREEITEM hti;

	// g_rgDocHeadings is an application-defined global array of 
	// the following structures: 
	//     typedef struct 
	//       { 
	//         TCHAR tchHeading[MAX_HEADING_LEN]; 
	//         int tchLevel; 
	//     } Heading; 
	for (int i = 0; i < this->drives.size(); i++)
	{
		// Add the item to the tree-view control. 
		AddItemToTree((LPSTR)this->drives[i].getName().c_str(), 1);
		/*if (hti == NULL)
			return FALSE;*/
	}
	return TRUE;
}

// End of private methods

FolderTree::FolderTree()
	:
	hTreeView(nullptr),
	hwndParent(nullptr),
	hInst(NULL),
	area({ 0,0,0,0 })
{
	vector<string> drives = Utilities::listDrives();

	for (string drive_name : drives)
	{
		this->drives.push_back(Directory(drive_name, nullptr));
	}
}

FolderTree::FolderTree(HWND hWnd, HINSTANCE hInst) : FolderTree()
{
	this->hwndParent = hWnd;
	this->hInst = hInst;

	GetClientRect(hWnd, &this->area);
	Utilities::rectTransform(this->area, 1, 0.95, 0.25, 0.9);

	CreateTreeView(hWnd, hInst);

	InitTreeViewItems();
}

FolderTree::~FolderTree()
{
}

void FolderTree::setRect(const RECT& cRect)
{
	Utilities::rectTransform(this->area, 0, 0, 0.25, 0);
}

//-----END OF FolderTree's METHODS-----//
