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

void OpenHandler();
void CopyHandler();
void MoveHandler();
void DeleteHandler();

class Buttons
{
	typedef void (*buttonHandler)(); // pointer to methods of Buttons class

private: // data

	// structure of buttons handles and their handle functions 
	std::vector<std::pair<HWND, buttonHandler>> buttons; 

private:

	Buttons();

	friend void OpenHandler();
	friend void CopyHandler();
	friend void MoveHandler();
	friend void DeleteHandler();

public:

	Buttons(HWND hParent, HINSTANCE hInst);

	void Handler(LPARAM lParam);

};


class FolderTree;