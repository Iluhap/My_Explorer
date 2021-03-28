#pragma once
#include "windows.h"
#include <vector>
#include <string>


namespace FileSystem
{
	/*
	*	Class used for getting info about files in directories 
	*/
	class File
	{
	public:
		std::string name;
		std::string extension;

		SYSTEMTIME time;

	private:
		File();

	public:
		File(const WIN32_FIND_DATA& data);
	};

	
	/*
	*	Class used for interaction with drive's directories 
	*/
	class Directory
	{
	private:
		std::string name = ""; // name of Directory

		Directory* parent = nullptr; // pointer to Parent directory

		std::vector<std::string> subDirectories; // list of subdirectories of current directory

		std::vector<File> files; // list of files of current directory

	private:

		Directory();

		void find_files();

	public:

		Directory(std::string name, Directory* parent);

		void Update();

		// Getters declaration

		std::vector<std::string> getDirs();
		std::vector<File> getFiles();

		File getFileInfo(std::string filename);

		std::string getName();
		std::string getPath();

		// End of getters declaration
	};

	/*
	* Namespace with helpers methods
	*/
	namespace Utilities
	{
		std::vector<std::string> listDrives(); // Returns a list of logical drives on current machine

		void Copy(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName);

		void deleteFile(LPCSTR lpFileName);

		void DeleteDirectory(Directory* pDir); 

		void ChangeName(LPCSTR lpFileName, LPCSTR lpNewFileName); // TODO implement changing name of file

		void CopyDirectory(); // TODO

		void MoveDirectory(); // TODO

		void clientRectToFolderRect(RECT &cRect);
	}
}
