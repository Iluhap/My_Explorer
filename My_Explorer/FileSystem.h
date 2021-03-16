#pragma once
#include "windows.h"
#include <vector>
#include <string>
#include <filesystem>


namespace FileSystem
{

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

	class Directory
	{
	private:
		std::string name = ""; // name of Directory

		Directory* parent = nullptr; // pointer to Parent directory

		std::vector<std::string> subDirectories; // list of subdirectories of current directory

		std::vector<File> files; // list of files of current directory

	private:
		Directory() {}

		void find_dirs();

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

	namespace Utilities
	{
		std::vector<std::string> listDrives(); // Returns a list of logical drives on current machine
	}
}
