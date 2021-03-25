#pragma once

#include "FileSystem.cpp"

using namespace FileSystem;

class Folder
{
private:
	Directory *currDir = nullptr;

	HWND hListBox = nullptr;

	RECT area;

	Folder();

public:

	Folder(Directory* dir, RECT& rt);

	void create(HWND hWmd, HINSTANCE hInst);

	void setRect(const RECT& rect);

	void setDir(Directory& directory);

	void updateDir();

	void updateList();
};

class FolderTree;