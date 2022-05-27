#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

//! FileManager class
/*!
	Class for write and read files
*/
class FileManager{
	public:
		enum OpenMode{READ, WRITE, READ_BINARY, WRITE_BINARY, WRITE_ATE, WRITE_BINARY_ATE, CLOSED};
		enum PointerPosition{BEGIN, END, CURRENT, NOTHING};
		//! Constructor.
		/*!
			Empty constructor
		*/
		FileManager();

		//! Constructor.
		/*!
			Constructor with path
			\param path Path for the file
			\param mode Open Mode: "r"-read "rb"-read binary "w"-write "wb"-write binary "wa"-write append "wba"-write binary append
		*/
		FileManager(const char* path, const char* mode = "rb");

		//! Destructor
		~FileManager();

		//! Opens a file to work with it
		void openFile(const char* path, const char* mode = "rb");

		//! Returns if file is opened
		bool isOpened(){return mFileIsOpened;};

		//! Checks if the file pointer is at End of File
		bool isEOF();

		//! Closes the file
		void close();

		//! Returns the file size in bytes
		unsigned int getFileSize();

		//! Reads a single character of the file (only if in nonbinary)
		char getCharacter();

		//! Reads as many characters as defined in count (only if in nonbinary)
		const char* getCharacters(unsigned int count);

		//! Reads a line of the file (only if in nonbinary)
		std::string getLine();

		//! Reads from file in binary format. Returns number of elements read
		unsigned int read(void* pointer, unsigned int size, unsigned int count = 1);

		//! Writes on file in binary format
		void write(void* pointer, unsigned int size, unsigned int count = 1);

		//! Writes on file in text format
		void writeText(std::string text);

		//! Sets the position of the pointer at input
		void setPointerPosInput(unsigned int position, PointerPosition way = NOTHING);

		//! Sets the position of the pointer at output
		void setPointerPosOutput(unsigned int position, PointerPosition way = NOTHING);

		//! Gets the position of the pointer at input
		unsigned int getPointerPosInput();

		//! Gets the position of the pointer at output
		unsigned int getPointerPosOutput();

		//! Returns the following character without extracting it
		char peek();

		//!  Synchronizes the associated stream buffer with its controlled output sequence.
		void synchronize();

		/*!  Extracts characters from the input sequence and discards them, 
		until either n characters have been extracted, or one compares equal to delim.
		*/
		void ignoreUntil(unsigned int n, unsigned int delim = 0);

		static bool canSave();
	private:
		std::string	mFilePath;		/*!<  Path of the file*/
		std::fstream mOpenedFile;	/*!<  File*/
		std::string	mOpenMode;		/*!<  Open mode for the file*/
		OpenMode mOpenModeEnum;		/*!<  Open mode for the file expressed as enum*/
			
		bool mFileIsOpened;			/*!<  True if the file has been opened*/
};

#endif /* FILEMANAGER_H */