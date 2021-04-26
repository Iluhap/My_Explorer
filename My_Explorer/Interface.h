#include "FileSystem.h"

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

	void CreateListView();

	bool InsertListViewItems();

	bool InitListViewColumns();

	void FillListViewTab();

public: // class public methods

	FolderView(Directory* dir, const RECT& cRect, HWND hwndParent, HINSTANCE hInst);

	void Create();

	void updateList();

	void setListViewRect(const RECT& cRect);
	void setDir(Directory& directory);

	vector<string> getElement(unsigned index) const;
	HWND getListHandle() const;
	Directory* getDir() const;

};


class Buttons
{
	typedef void (* buttonHandler)(FolderView*); // pointer to methods of Buttons class

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

class FolderTree
{
private:
	vector<Directory> directories;

private:
	FolderTree();

public:

};