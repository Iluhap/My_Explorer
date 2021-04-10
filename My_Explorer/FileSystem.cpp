#include "FileSystem.h"


namespace FileSystem
{
	//---------IMPLEMENTETION OF DIRECTORY'S METHODS---------//

	Directory::Directory(std::string name, Directory* parent) : name(name), parent(parent)
	{
		find_files();
	}

	void Directory::Update()
	{
		this->find_files();
	}

	void Directory::find_files()
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

	//----GETTERS-----//

	std::string Directory::getName() { return this->name; }
	std::vector<File> Directory::getFiles() { return this->files; }
	std::vector<std::string> Directory::getDirs() { return this->subDirectories; }
	File Directory::getFileInfo(std::string filename)
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
	std::string Directory::getPath()
	{
		if (this->parent == nullptr)
			return this->name;
		else
			return parent->getPath() + "\\" + this->name;
	}

	//---------END OF METHODS---------//


	//---------IMPLEMENTETION OF FILE'S METHODS---------//
	File::File(const WIN32_FIND_DATA& data)
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
	}

	//---------END OF METHODS---------//


	namespace Utilities
	{
		//---------IMPLEMENTETION OF UTILITIES METHODS---------//

		std::vector<std::string> Utilities::listDrives()
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

		bool openFile(std::string fileName)
		{
			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			std::string cmd = "notepad \"" + fileName + "\"";

			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));
			if (!CreateProcess(NULL, (LPSTR)(cmd.c_str()), NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi))
				return false;
			else
				return true;
		}

		bool copyFile(std::string lpExistingFileName, std::string lpNewFileName)
		{
			BOOL status = TRUE;

			if (!CopyFile(lpExistingFileName.c_str(), lpNewFileName.c_str(), status))
				return false;
			else
				return true;


		}

		bool deleteFile(std::string lpFileName)
		{
			if (!DeleteFile(lpFileName.c_str()))
				return false;
			else
				return true;
		}

		bool changeFileName(std::string lpFileName, std::string lpNewFileName)
		{
			if (!MoveFileEx(lpFileName.c_str(), lpNewFileName.c_str(), MOVEFILE_COPY_ALLOWED))
				return false;
			else
				return true;
		}

		bool copyDirectory(Directory* pDir, std::string copy_path)
		{
			if (!CreateDirectory(copy_path.c_str(), NULL))
				return false;
			else
			{
				// Copying all files from current directory
				for (File file : pDir->getFiles())
				{
					std::string filename = (pDir->getPath() + '\\' + file.name);
					std::string copyFileName = (copy_path + "\\" + file.name);

					copyFile(filename.c_str(), copyFileName.c_str());
				}

				std::vector<std::string> subDirs = pDir->getDirs(); // List all subdirectories

				for (std::string dir : subDirs)
				{
					// Copying all directories except system dirs
					if (!(dir == "." or dir == ".."))
					{
						Directory sub_dir(dir, pDir);

						copyDirectory(&sub_dir, copy_path + "\\" + dir);
					}

				}
				return true;
			}

		}

		bool moveDirectory(Directory* pDir, std::string newPath)
		{
			if (copyDirectory(pDir, newPath))
				return false;

			if (deleteDirectory(pDir))
				return false;

			return true;
		}

		bool deleteDirectory(Directory* pDir)
		{
			// Deleting all files from current directory
			for (File file : pDir->getFiles())
			{
				std::string filename = (pDir->getPath() + '\\' + file.name).c_str();

				deleteFile(filename.c_str());
			}

			std::vector<std::string> subDirs = pDir->getDirs(); // List all subdirectories

			for (std::string dir : subDirs)
			{
				// Deleteing all directories except system dirs
				if (!(dir == "." or dir == ".."))
				{
					Directory sub_dir(dir, pDir);

					deleteDirectory(&sub_dir);
				}

			}

			// Deleting current directory
			if (!RemoveDirectory(pDir->getPath().c_str()))
				return false;
			else
				return 0;
		}


		//---------END OF METHODS---------//	
	}
}
