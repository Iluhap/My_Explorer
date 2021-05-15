#include "Interface.h"

//-----BUTTONS'S METHODS-----//

string Buttons::edit_control_text = ""; // STATIC VARIABLE OF BUTTONS

LRESULT Buttons::DlgFunc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hEdit;
	CHAR s_text[256] = { 0 };

	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:

			hEdit = GetDlgItem(hDlg, IDC_EDIT1);

			SendMessage(hEdit, WM_GETTEXT, (WPARAM)255, (LPARAM)s_text);

			Buttons::edit_control_text = s_text;

			EndDialog(hDlg, LOWORD(wParam));

			return TRUE;

		case IDCANCEL:

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}
	return FALSE;
}

Buttons::Buttons(HWND hParent, HINSTANCE hInst)
{
	vector<string> names = { "Open", "Copy", "Move", "Delete" }; // Names of buttons
	vector<buttonHandler> functions = { OpenHandler , CopyHandler, MoveHandler, DeleteHandler }; // Handlers functions for buttons

	RECT rt;
	GetClientRect(hParent, &rt);
	Utilities::rectTransform(rt, 0.70, 0.1, 1, 1); // Preparing area for buttons

	for (size_t i = 0; i < names.size(); i++)
	{
		size_t left = rt.left + (i) * (rt.right - rt.left) / names.size(); // Calculating position for current button

		HWND hButton = CreateWindow("button", names[i].c_str(),
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			left, rt.top + 10,
			70, 25,
			hParent, (HMENU)NULL, hInst, NULL);
		this->buttons.push_back({ hButton, functions[i] });
	}
}

void Buttons::Handler(LPARAM lParam, FolderView* pFolderView)
{
	HWND hPressedButton = (HWND)lParam;

	for (Button button : this->buttons)
	{
		if (button.handle == hPressedButton)
			button.pFunction(pFolderView);
	}
}

void Buttons::OpenHandler(FolderView* pFolderView)
{
	HWND hList = pFolderView->getListHandle();

	unsigned id = (unsigned)SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

	pFolderView->openItem(id);
}
void Buttons::CopyHandler(FolderView* pFolderView)
{
	HWND hList = pFolderView->getListHandle();

	unsigned id = (unsigned)SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

	if (id < 0 or id >= pFolderView->listViewTab.size())
		MessageBox(NULL, "You should choose element", "Open ERROR", MB_ICONWARNING);
	else
	{
		vector<string> elem = pFolderView->getElement(id);

		string name = elem[0];
		string type = elem[1];

		DialogBox(pFolderView->hInst, MAKEINTRESOURCE(IDD_DIALOG), pFolderView->hwndParent, (DLGPROC)&Buttons::DlgFunc);

		if (type == "Folder")
			Utilities::copyDirectory(pFolderView->getDir(), Buttons::edit_control_text);
		else
			Utilities::copyFile(pFolderView->getDir()->getPath() + "\\" + name, Buttons::edit_control_text);

		Buttons::edit_control_text = "";
		pFolderView->updateList();
	}
}
void Buttons::MoveHandler(FolderView* pFolderView)
{
	HWND hList = pFolderView->getListHandle();

	unsigned id = (unsigned)SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

	if (id < 0 or id >= pFolderView->listViewTab.size())
		MessageBox(NULL, "You should choose element", "Open ERROR", MB_ICONWARNING);
	else
	{
		vector<string> elem = pFolderView->getElement(id);

		string name = elem[0];
		string type = elem[1];

		DialogBox(pFolderView->hInst, MAKEINTRESOURCE(IDD_DIALOG), pFolderView->hwndParent, (DLGPROC)&Buttons::DlgFunc);

		if (type == "Folder")
			Utilities::moveDirectory(pFolderView->getDir(), Buttons::edit_control_text);
		else
			Utilities::changeFileName(pFolderView->getDir()->getPath() + "\\" + name, Buttons::edit_control_text);

		Buttons::edit_control_text = "";
		pFolderView->updateList();
	}
}
void Buttons::DeleteHandler(FolderView* pFolderView)
{
	HWND hList = pFolderView->getListHandle();

	unsigned id = (unsigned)SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

	if (id < 0 or id >= pFolderView->listViewTab.size())
		MessageBox(NULL, "You should choose element", "Open ERROR", MB_ICONWARNING);
	else
	{
		vector<string> elem = pFolderView->getElement(id);

		string name = elem[0];
		string type = elem[1];

		string path = pFolderView->getDir()->getPath() + "\\" + name;

		Directory tmpDir = Directory(path, nullptr);

		if (type == "Folder")
			Utilities::deleteDirectory(&tmpDir);
		else
			Utilities::deleteFile(path);

		pFolderView->updateList(); // Updating of FolderView
	}
}

//-----END OF BUTTONS'S METHODS-----//
