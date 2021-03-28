#include "FileSystem.h"

namespace FileSystem
{
	//---------IMPLEMENTETION OF DIRECTORY'S METHODS---------//

	inline Directory::Directory(std::string name, Directory* parent) : name(name), parent(parent)
	{
		find_files();
	}

	inline void Directory::Update()
	{
		this->find_files();
	}

	inline void Directory::find_files()
	{
		std::string path = this->getPath();

		WIN32_FIND_DATA data;
		HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &data);      // DIRECTORY

		if (hFind != INVALID_HANDLE_VALUE) {
			do
			{
				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					this->subDirectories.push_back(data.cFileName); // filling list of subDirectories
				else
					this->files.push_back(File(data)); // filling list of files

			} while (FindNextFile(hFind, &data));
			FindClose(hFind);
		}
	}

	inline std::string Directory::getName() { return this->name; }
	inline std::vector<File> Directory::getFiles() { return this->files; }
	inline std::vector<std::string> Directory::getDirs() { return this->subDirectories; }

	inline File Directory::getFileInfo(std::string filename)
	{
		auto file = std::find_if(this->files.begin(), this->files.end(),
			[filename](const File& elem)
		{
			if (elem.name == filename)
				return true;
			else
				return false;
		}
		);
		return *file;
	}

	inline std::string Directory::getPath()
	{
		if (this->parent == nullptr)
			return this->name;
		else
			return parent->getPath() + "\\" + this->name;
	}
	//---------END OF METHODS---------//


	//---------IMPLEMENTETION OF FILE'S METHODS---------//
	inline File::File(const WIN32_FIND_DATA& data)
	{
		this->name = data.cFileName; // Saving name of file

		FileTimeToSystemTime(&data.ftLastWriteTime, &this->time); // Saving time

		// -----Saving file extension----- //

		std::string::size_type idx;

		idx = name.rfind('.');

		if (idx != std::string::npos)
			this->extension = name.substr(idx + 1);
		else
			this->extension = "\0";
		// ------------------------------- //
	}

	//---------END OF METHODS---------//


	namespace Utilities
	{
		// Implemention of utilities methods
		inline std::vector<std::string> Utilities::listDrives()
		{
			char buffer[128];

			GetLogicalDriveStrings(128, buffer);

			std::vector<std::string> drives;

			std::string acum;

			for (size_t i = 0; i < 40; i++)
			{
				if (buffer[i] == '\0' or buffer[i] == '\\')
				{
					drives.push_back(acum);
					acum = "";
				}
				else
					acum += buffer[i];
			}
			return drives;
		}


		// TODO find the way to notufy user about ERRORS acquired
		inline void Copy(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName)
		{
			BOOL status = TRUE;

			if (!CopyFile(lpExistingFileName, lpNewFileName, status))
			{
				// ERROR message
			}


		}
		inline void deleteFile(LPCSTR lpFileName)
		{
			if (!DeleteFile(lpFileName))
			{
				// ERROR message
			}
		}
		
		// TODO add comments 
		inline void DeleteDirectory(Directory* pDir)
		{
			for (File file : pDir->getFiles())
			{
				std::string filename = (pDir->getPath() + '\\' + file.name).c_str();

				deleteFile(filename.c_str());
			}

			std::vector<std::string> subDirs = pDir->getDirs();

			for (std::string dir : subDirs)
			{

				if (!(dir == "." or dir == ".."))
				{
					Directory sub_dir(dir, pDir);

					DeleteDirectory(&sub_dir);
				}
				
			}

			if (RemoveDirectory(pDir->getPath().c_str()))
			{
				// Error message
			}
		}

		inline void ChangeName(LPCSTR lpFileName, LPCSTR lpNewFileName)
		{
			Copy(lpFileName, lpNewFileName);
			deleteFile(lpFileName);
		}

		inline void clientRectToFolderRect(RECT& cRect)
		{
			double l_offset = 0.3;

			cRect = { (LONG)(cRect.right * l_offset), cRect.top, cRect.right, cRect.bottom };

		}
	}
}
