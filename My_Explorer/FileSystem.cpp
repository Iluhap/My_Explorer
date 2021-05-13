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

		string tmp_str = "";

		bool cond;

		HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &data);      // DIRECTORY

		if (hFind != INVALID_HANDLE_VALUE) {
			do
			{
				tmp_str = data.cFileName;
				cond = !(tmp_str == ".." or tmp_str == ".");
				if (cond)
				{
					if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						this->subDirectories.push_back(tmp_str); // filling list of subDirectories
					else
						this->files.push_back(File(data)); // filling list of files
				}
			} while (FindNextFile(hFind, &data));
			FindClose(hFind);
		}
	}

	//----GETTERS-----//

	string Directory::getName() const { return this->name; }
	vector<File> Directory::getFiles() const { return this->files; }
	vector<string> Directory::getDirs() const { return this->subDirectories; }
	File Directory::getFileInfo(string filename) const
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
	string Directory::getPath() const
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
		this->size = to_string((data.nFileSizeLow / 1000) ? data.nFileSizeLow / 1000 : 1) + " KB";

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

			const UINT buff_size = 128;

			char buffer[buff_size];

			GetLogicalDriveStrings(buff_size, buffer);

			vector<string> drives;

			string acum;

			for (size_t i = 0; i < buff_size; i++)
			{
				if (buffer[i] == '\\')
					continue;

				if (buffer[i] == '\0' and !acum.empty())
				{
					drives.push_back(acum);
					acum = "";
				}
				else
					acum += buffer[i];
			}
			return drives;
		}

		void Utilities::rectTransform(
			RECT& cRect,
			double left_scale,
			double top_scale,
			double right_scale,
			double bottom_scale
		)
		{
			size_t xSize = (cRect.right - cRect.left);
			size_t ySize = (cRect.bottom - cRect.top);

			cRect =
			{
				(LONG)round(cRect.left + xSize * (1 - left_scale)),
				(LONG)round(cRect.top + ySize * (1 - top_scale)),
				(LONG)round(cRect.right - (xSize * (1 - right_scale))),
				(LONG)round(cRect.bottom - (ySize * (1 - bottom_scale)))
			};
		}

		bool endWith(string str, string subStr)
		{
			string::size_type idx;

			string sub;

			idx = str.rfind('\\');

			if (idx != string::npos)
				sub = str.substr(idx + 1);

			if (sub == subStr)
				return true;
			else
				return false;
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
			if (endWith(pDir->getName(), ".") or endWith(pDir->getName(), ".."))
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
