#pragma once

#include "FileSystem.cpp"


// TODO ����������� ��������� ������ ������� ����������
class Folder
{
private:
	FileSystem::Directory currDir;

	RECT area;

public:
	Folder(const FileSystem::Directory &dir, const RECT& area);

	void updateDir();

	void updateArea();
};

class FolderTree;