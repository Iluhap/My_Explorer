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

HTREEITEM FolderTree::AddItemToTree(LPTSTR lpszItem, int nLevel, HTREEITEM hParentItem)
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

	// Save the heading level in the item's application-defined 
	// data area. 
	tvi.lParam = (LPARAM)nLevel;
	tvins.item = tvi;
	tvins.hInsertAfter = hPrev;

	// Set the parent item based on the specified level. 
	if (nLevel == 1)
		tvins.hParent = TVI_ROOT;
	else if (nLevel == 2)
		tvins.hParent = hParentItem;

	// Add the item to the tree-view control. 
	hPrev = (HTREEITEM)SendMessage(
		this->hTreeView,
		TVM_INSERTITEM,
		0,
		(LPARAM)(LPTVINSERTSTRUCT)&tvins
	);

	if (hPrev == NULL)
		return nullptr;

	return hPrev;
}

BOOL FolderTree::InitTreeViewItems()
{
	HTREEITEM hti = nullptr;

	string name = "";

	for (int i = 0; i < this->drives.size(); i++)
	{
		// Add the item to the tree-view control. 

		name = this->drives[i]->getName();

		hti = AddItemToTree((LPSTR)name.c_str(), 1, NULL);

		this->treeItems.insert(
			{
				hti,
				this->drives[i]
			}
		);
	}

	return TRUE;
}

// End of private methods

FolderTree::FolderTree()
	:
	hTreeView(nullptr),
	hwndParent(nullptr),
	pFolderView(nullptr),
	hInst(NULL),
	area({ 0,0,0,0 })
{
	vector<string> drives = Utilities::listDrives();

	for (string drive_name : drives)
	{
		Directory* pDir = new Directory(drive_name, nullptr);
		this->drives.push_back(pDir);
	}
}

FolderTree::FolderTree(HWND hWnd, HINSTANCE hInst, FolderView* pFV) : FolderTree()
{
	this->hwndParent = hWnd;
	this->hInst = hInst;
	this->pFolderView = pFV;

	GetClientRect(hWnd, &this->area);
	Utilities::rectTransform(this->area, 1, 0.95, 0.25, 0.9);

	CreateTreeView(hWnd, hInst);

	InitTreeViewItems();

	this->pFolderView->setDir(this->drives[0]); // Setting a start FolderView window state
}

FolderTree::~FolderTree()
{
	for (auto& elem : treeItems)
	{
		delete elem.second;
	}
}

void FolderTree::setRect(const RECT& cRect)
{
	Utilities::rectTransform(this->area, 0, 0, 0.25, 0);
}

void FolderTree::setSelection(HTREEITEM hItem)
{
	HTREEITEM hti = nullptr;

	Directory* pDir = nullptr;

	if (loaded.find(hItem) == loaded.end()) // Subitems of hItem is not loaded
	{
		pDir = this->treeItems[hItem];

		for (const auto& name : pDir->getDirs())
		{
			hti = AddItemToTree((LPSTR)name.c_str(), 2, hItem);

			this->treeItems.insert(
				{ 
					hti,
					new Directory(name, pDir)
				}
			);
		}

		loaded.insert(hItem); // Tag treeView item as loaded
	}
	this->pFolderView->setDir(this->treeItems[hItem]); // Updating of FolderView state
}

//-----END OF FolderTree's METHODS-----//
