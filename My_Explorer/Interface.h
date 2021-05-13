#include "FileSystem.h"

#pragma comment(lib, "comctl32.lib")
#include "commctrl.h"

#include "resource.h"

#include <map>
#include <set>

using namespace FileSystem;

/*
* Class that represents an interface used to interact with files and directories
* Based on List-View control windows
*/
class FolderView
{
	friend class Buttons;

private: // class data
	Directory* currDir; // Pointer to Directory structure

	HWND hListBox; // Handle of List-View Window

	RECT area; // ListView area

	HWND hwndParent; // Handle of parent window
	HINSTANCE hInst; // Handle of application

	vector<string> columns; // Columns of List-View

	vector<vector<string>> listViewTab; // Items structure

	// Set of pointer to used directories
	set<Directory*> trashDirs;

private: // class private methods

	FolderView();

	// Creates a List-View control window
	void CreateListView(HWND hwndParent, HINSTANCE hInst);

	bool InitListViewColumns();

	bool InsertListViewItems();

	void FillListViewTab();

public: // class public methods

	FolderView(HWND hwndParent, HINSTANCE hInst, const RECT& cRect);

	~FolderView();

	void setRect(const RECT& cRect);
	void setDir(Directory* directory);

	vector<string> getElement(unsigned index) const;
	HWND getListHandle() const;
	Directory* getDir() const;

	// Method used for opening files and directories in List-View control
	void openItem(int itemID);

	// Updated state of List-View window
	void updateList();

};


/*
* This class contains data structures and methods
* based on Tree-View control window
* And used to represent tree based file sytem of current system
*/
class FolderTree
{
private:

	FolderView* pFolderView; // Pointer to FolderView object

	HWND hTreeView; // Handle of TreeView window
	HWND hwndParent; // Handle of parent window
	HINSTANCE hInst; // Handle of application

	RECT area; // TreeView area

	vector<Directory*> drives; // List of logical drives of current system

	set<HTREEITEM> loaded; // Contains handles of loaded items of tree

	// Map of Directories and respective handles of Tree
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

/*
* Class agregator of buttons
* Contains implementation of buttons logic
*/
class Buttons
{
	typedef void (*buttonHandler)(FolderView*); // pointer to methods of Buttons class

	struct Button
	{
		HWND handle;
		buttonHandler pFunction;
	};

private: // Data

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
