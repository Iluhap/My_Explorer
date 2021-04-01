#pragma once

#include "FileSystem.cpp"

using namespace FileSystem;

class FolderView
{
private: // class data
	Directory *currDir = nullptr; // Pointer to Directory structure

	HWND hListBox = nullptr; // Handle of List-View Window

	std::vector<std::string> columns{ "Name", "Type", "Date" }; // Columns of List-View

	RECT area; // ListView area

private: // class private methods
	FolderView();

	void updateList();

	void CreateListView(HWND hwndParent, HINSTANCE hInst);

	bool InsertListViewItems();

	bool InitListViewColumns();

public: // class public methods

	FolderView(Directory* dir, const RECT& cRect);

	void Create(HWND hWmd, HINSTANCE hInst);

	void setRect(const RECT& cRect);

	void setDir(Directory& directory);
};

class FolderTree;