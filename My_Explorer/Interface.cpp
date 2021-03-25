#include "Interface.h"

inline Folder::Folder(Directory* dir, RECT& rt)
{
	this->currDir = dir;

	this->area = rt;
}

inline void Folder::create(HWND hWnd ,HINSTANCE hInst)
{
	this->hListBox = CreateWindowExW(WS_EX_CLIENTEDGE, 
		L"LISTBOX", NULL,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL,
		area.left, area.top, area.right - area.left, area.bottom - area.top,
		hWnd, NULL, hInst, NULL);
		
	if (!this->hListBox)
	{
		MessageBox(hWnd, "Unable to create window", "Warning", MB_ICONWARNING);
	}

	this->updateList();
}

inline void Folder::setRect(const RECT& rect)
{
	this->area = rect;
}

inline void Folder::setDir(Directory& directory)
{
	*this->currDir = directory;

	this->updateList();
}

inline void Folder::updateDir()
{
	this->currDir->Update();
	this->updateList();
}

inline void Folder::updateList()
{
	SendMessage(this->hListBox, LB_RESETCONTENT, 0, 0);

	std::string str = "";
	for (std::string dir : this->currDir->getDirs())
	{
		str = dir + "     Folder";

		SendMessage(this->hListBox, LB_ADDSTRING, 0, (LPARAM)str.c_str());
	}

	str = "";
	for (FileSystem::File file : this->currDir->getFiles())
	{
		str = file.name + "    File    " + file.extension;

		SendMessage(this->hListBox, LB_ADDSTRING, 0, (LPARAM)str.c_str());
	}
}
