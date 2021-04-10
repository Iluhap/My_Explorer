#include "FileSystem.h"

using namespace FileSystem;


class FolderView
{
	friend class Buttons;

private: // class data
	Directory* currDir; // Pointer to Directory structure

	HWND hListBox; // Handle of List-View Window

	std::vector<std::string> columns; // Columns of List-View

	std::vector<std::vector<std::string>> listViewTab;

	RECT area; // ListView area

private: // class private methods

	FolderView();

	void updateList();

	void CreateListView(HWND hwndParent, HINSTANCE hInst);

	bool InsertListViewItems();

	bool InitListViewColumns();

	void FillListViewTab();

	void rectTransform(RECT& cRect,
		double left_scale,
		double top_scale,
		double right_scale,
		double bottom_scale);

public: // class public methods

	FolderView(Directory* dir, const RECT& cRect);

	void Create(HWND hWmd, HINSTANCE hInst);

	void setListViewRect(const RECT& cRect);

	void setDir(Directory& directory);

	std::vector<std::string> getElement(unsigned index);

	HWND getListHandle();

	Directory* getDir();

};


class Buttons
{
	typedef void (*buttonHandler)(FolderView*); // pointer to methods of Buttons class

	friend void OpenHandler(FolderView*);
	friend void CopyHandler(FolderView*);
	friend void MoveHandler(FolderView*);
	friend void DeleteHandler(FolderView*);

private: // data

	struct Button
	{
		HWND handle;
		buttonHandler pFunction;
	};

	// structure of buttons handles and their handle functions 
	std::vector<Button> buttons; 

private:

	Buttons();

public:

	Buttons(HWND hParent, HINSTANCE hInst);

	void Handler(LPARAM lParam, FolderView* pFolderView);

};

void OpenHandler(FolderView*);
void CopyHandler(FolderView*);
void MoveHandler(FolderView*);
void DeleteHandler(FolderView*);

class FolderTree;