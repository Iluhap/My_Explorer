#include "FileSystem.h"

using namespace FileSystem;


class FolderView
{
private: // class data
	Directory* currDir; // Pointer to Directory structure

	HWND hListBox; // Handle of List-View Window

	std::vector<std::string> columns; // Columns of List-View

	RECT area; // ListView area

private: // class private methods
	// FolderView();

	void updateList();

	void CreateListView(HWND hwndParent, HINSTANCE hInst);

	bool InsertListViewItems();

	bool InitListViewColumns();

	void rectTransform(RECT& cRect, double left_scale,
									double top_scale, 
									double right_scale, 
									double bottom_scale);
	

public: // class public methods

	FolderView(Directory* dir, const RECT& cRect);

	void Create(HWND hWmd, HINSTANCE hInst);

	void setListViewRect(const RECT& cRect);

	void setDir(Directory& directory);

	void HandleWM_Notify(LPARAM lParam);

};

class FolderTree;