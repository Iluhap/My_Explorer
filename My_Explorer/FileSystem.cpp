#include "FileSystem.h"


namespace FileSystem
{
	//---------IMPLEMENTETION OF DIRECTORY'S METHODS---------//

	Directory::Directory(string name, Directory* parent) : name(name), parent(parent)
	{
		find_files();
	}

	void Directory::Update()
	{
		this->find_files();
	}

	void Directory::find_files()
	{
		this->files.clear();
		this->subDirectories.clear();

		string path = this->getPath();

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

	string Directory::getName() { return this->name; }
	vector<File> Directory::getFiles() { return this->files; }
	vector<string> Directory::getDirs() { return this->subDirectories; }
	File Directory::getFileInfo(string filename)
	{
		auto file = find_if(this->files.begin(), this->files.end(),
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
	string Directory::getPath()
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

		string::size_type idx;

		idx = name.rfind('.');

		if (idx != string::npos)
			this->extension = name.substr(idx + 1);
		else
			this->extension = "\0";
	}

	//---------END OF METHODS---------//


	namespace Utilities
	{
		//---------IMPLEMENTETION OF UTILITIES METHODS---------//

		vector<string> Utilities::listDrives()
		{
			char buffer[128];

			GetLogicalDriveStrings(128, buffer);

			vector<string> drives;

			string acum;

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

		bool openFile(string fileName)
		{
			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			string cmd = "notepad \"" + fileName + "\"";

			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));
			if (!CreateProcess(NULL, (LPSTR)(cmd.c_str()), NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi))
				return false;
			else
				return true;
		}

		bool copyFile(string existingFileName, string newFileName)
		{
			BOOL status = TRUE;

			if (!CopyFile(existingFileName.c_str(), newFileName.c_str(), status))
				return false;
			else
				return true;


		}

		bool deleteFile(string fileName)
		{
			if (!DeleteFile(fileName.c_str()))
				return false;
			else
				return true;
		}

		bool changeFileName(string fileName, string newFileName)
		{
			if (!MoveFileEx(fileName.c_str(), newFileName.c_str(), MOVEFILE_COPY_ALLOWED))
				return false;
			else
				return true;
		}

		bool copyDirectory(Directory* pDir, string copy_path)
		{
			if (!CreateDirectory(copy_path.c_str(), NULL))
				return false;
			else
			{
				// Copying all files from current directory
				for (File file : pDir->getFiles())
				{
					string filename = (pDir->getPath() + '\\' + file.name);
					string copyFileName = (copy_path + "\\" + file.name);

					copyFile(filename.c_str(), copyFileName.c_str());
				}

				vector<string> subDirs = pDir->getDirs(); // List all subdirectories

				for (string dir : subDirs)
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

		bool moveDirectory(Directory* pDir, string newPath)
		{
			if (copyDirectory(pDir, newPath))
				return false;

			if (deleteDirectory(pDir))
				return false;

			return true;
		}

		bool deleteDirectory(Directory* pDir)
		{
			if (pDir->getName() == "." or pDir->getName() == "..")
				return false;

			// Deleting all files from current directory
			for (File file : pDir->getFiles())
			{
				string filename = (pDir->getPath() + '\\' + file.name).c_str();

				deleteFile(filename.c_str());
			}

			vector<string> subDirs = pDir->getDirs(); // List all subdirectories

			for (string dir : subDirs)
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
				return true;
		}

		//---------END OF METHODS---------//	
	}
}
