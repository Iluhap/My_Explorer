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
	*	Class used for interaction with directories 
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
	* Namespace with helper methods
	*/
	namespace Utilities
	{
		// Returns a vector of names of logical drives on current machine
		std::vector<std::string> listDrives();
		
		//--- Methods below returns non-zero value if falure acquired ---//

		bool copyFile(std::string lpExistingFileName, std::string lpNewFileName);

		bool deleteFile(std::string lpFileName);

		bool openFile(std::string fileName);

		bool changeFileName(std::string lpFileName, std::string lpNewFileName);

		// Recursive deleting of directory
		bool deleteDirectory(Directory* pDir); 
		
		// Recursive copying of directory
		bool copyDirectory(Directory* pDir, std::string newPath); 

		// Uses copy and delete methods for replacing directory
		bool moveDirectory(Directory* pDir, std::string newPath);
	}
}
