#include "FileSystem.h"

namespace FileSystem
{
	//---------IMPLEMENTETION OF DIRECTORY METHODS---------//

	inline Directory::Directory(std::string name, Directory* parent) : name(name), parent(parent)
	{
		find_dirs();

		find_files();
	}

	inline void Directory::Update()
	{
		this->find_dirs();
		this->find_files();
	}

	inline void Directory::find_dirs()
	{
		std::string path = this->getPath();

		WIN32_FIND_DATA data;
		HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &data);      // DIRECTORY

		if (hFind != INVALID_HANDLE_VALUE) {
			do
			{
				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					this->subDirectories.push_back(data.cFileName); // filling list of subDirectories

			} while (FindNextFile(hFind, &data));
			FindClose(hFind);
		}
	}
	inline void Directory::find_files()
	{
		std::string path = this->getPath();

		WIN32_FIND_DATA data;
		HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &data);      // DIRECTORY

		if (hFind != INVALID_HANDLE_VALUE) {
			do
			{

				if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					File file(data);
					this->files.push_back(file); // filling list of files
				}
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

		/*if (file == this->files.end())
		{

		}*/

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

	inline File::File(const WIN32_FIND_DATA& data)
	{
		{
			this->name = data.cFileName; // Saving name of file

			FileTimeToSystemTime(&data.ftLastWriteTime, &this->time); // Saving time

			// -----Saving file extension----- //

			std::string::size_type idx;

			idx = name.rfind('.');

			if (idx != std::string::npos)
				this->extension = name.substr(idx + 1);
			else
				this->extension = "NO_EXTENSION";
			// ------------------------------- //
		}
	}
}
