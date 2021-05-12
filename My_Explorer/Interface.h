#include "FileSystem.h"

#pragma comment(lib, "comctl32.lib")
#include "commctrl.h"

#include "resource.h"

#include <map>
#include <set>

using namespace FileSystem;


class FolderView
{
	friend class Buttons;

private: // class data
	Directory* currDir; // Pointer to Directory structure

	HWND hListBox; // Handle of List-View Window

	HWND hwndParent;
	HINSTANCE hInst;

	vector<string> columns; // Columns of List-View

	vector<vector<string>> listViewTab;

	RECT area; // ListView area

private: // class private methods

	FolderView();

	void CreateListView(HWND hwndParent, HINSTANCE hInst);

	bool InsertListViewItems();

	bool InitListViewColumns();

	void FillListViewTab();

public: // class public methods

	FolderView(HWND hwndParent, HINSTANCE hInst, const RECT& cRect);

	void updateList();

	void setRect(const RECT& cRect);
	void setDir(Directory* directory);

	vector<string> getElement(unsigned index) const;
	HWND getListHandle() const;
	Directory* getDir() const;

};


class FolderTree
{
private:

	FolderView* pFolderView; // Pointer to FolderView object

	HWND hTreeView; // Handle of TreeView window
	HWND hwndParent; // Handle of parent window
	HINSTANCE hInst; // Handle of application

	RECT area; // TreeView area

	vector<Directory*> drives;

	set<HTREEITEM> loaded;

	std::map<HTREEITEM, Directory*> treeItems;

private:
	FolderTree();

	void CreateTreeView(HWND hWnd, HINSTANCE hInst);
	HTREEITEM AddItemToTree(LPTSTR lpszItem, int nLevel, HTREEITEM parentItem);
	BOOL InitTreeViewItems();

public:

	FolderTree(HWND hWnd, HINSTANCE hInst, FolderView* pFV);

	~FolderTree();

	void setRect(const RECT&);

	void setSelection(HTREEITEM hItem);

};


class Buttons
{
	typedef void (*buttonHandler)(FolderView*); // pointer to methods of Buttons class

	struct Button
	{
		HWND handle;
		buttonHandler pFunction;
	};

private: // data

	// structure of buttons handles and their handle functions 
	vector<Button> buttons;

	static string edit_control_text;

private:

	Buttons();

public:

	Buttons(HWND hParent, HINSTANCE hInst);

	void Handler(LPARAM lParam, FolderView* pFolderView);

	static LRESULT CALLBACK DlgFunc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	//-----Hadler functions for buttons-----//
	static void OpenHandler(FolderView*);
	static void CopyHandler(FolderView*);
	static void MoveHandler(FolderView*);
	static void DeleteHandler(FolderView*);

};
