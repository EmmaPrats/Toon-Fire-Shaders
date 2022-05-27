#include "FileManager.h"

FileManager::FileManager() {
	mFileIsOpened = false;
	mOpenMode == "closed";
	mOpenModeEnum = CLOSED;
}

FileManager::FileManager(const char* path, const char* mode) {
	mFileIsOpened = false;
	mOpenMode == "closed";
	mOpenModeEnum = CLOSED;
	openFile(path, mode);
}

bool FileManager::canSave(){
	return true;
}

FileManager::~FileManager() {

}

unsigned int FileManager::getFileSize(){
	struct stat results;
    if (stat(mFilePath.c_str(), &results) == 0){
        // The size of the file in bytes is in
        return results.st_size;
	}else{
        // An error occurred
		std::cout<<"Error when retrieving file size of: "<< mFilePath<<std::endl;
	}
	return 0;
}

void FileManager::openFile(const char* path, const char* mode){
	mFilePath = path;
	mOpenMode = mode;
	
	if(mOpenMode == "r"){
		mOpenModeEnum = READ;
		mOpenedFile.open(mFilePath, std::ios::in);
	}
	if(mOpenMode == "rb"){
		mOpenModeEnum = READ_BINARY;
		mOpenedFile.open(mFilePath, std::ios::in | std::ios::binary);
	}
	if(mOpenMode == "w"){
		mOpenModeEnum = WRITE;
		mOpenedFile.open(mFilePath, std::ios::out | std::ios::trunc);
	}
	if(mOpenMode == "wb"){
		mOpenModeEnum = WRITE_BINARY;
		mOpenedFile.open(mFilePath, std::ios::out | std::ios::binary | std::ios::trunc);
	}
	if(mOpenMode == "wa"){
		mOpenModeEnum = WRITE_ATE;
		mOpenedFile.open(mFilePath, std::ios::out | std::ios::ate);
	}
	if(mOpenMode == "wba"){
		mOpenModeEnum = WRITE_BINARY_ATE;
		mOpenedFile.open(mFilePath, std::ios::out | std::ios::binary | std::ios::ate);
	}
	if(mOpenedFile.is_open()){
		mFileIsOpened = true;
	}
	if(!mFileIsOpened){
		std::cout << "Error with open mode: " << mOpenMode << " - File : " << path << std::endl;
	}
}

bool FileManager::isEOF(){
	if(mFileIsOpened){
		return mOpenedFile.eof();
	}
	std::cout<<"The file is not opened"<<std::endl;
	return false;
}

void FileManager::close(){
	if(mFileIsOpened){
		mOpenedFile.close();
	}
}

char FileManager::getCharacter(){
	if(!mFileIsOpened){
		std::cout<<"The file is not opened"<<std::endl;
		return '\0';
	}
	if(mOpenModeEnum != READ){
		std::cout<<"Open mode: "<<mOpenMode << " prevents from reading"<<std::endl;
		return '\0';
	}
	char c;
	mOpenedFile.get(c);
	return c;
}

const char* FileManager::getCharacters(unsigned int count){
	if(!mFileIsOpened){
		std::cout<<"The file is not opened"<<std::endl;
		return "";
	}
	if(mOpenModeEnum != READ){
		std::cout<<"Open mode: "<<mOpenMode << " prevents from reading"<<std::endl;
		return "";
	}
	std::string returnValue;
	unsigned int i = 0;
	while(i < count && !isEOF()){
		returnValue.push_back(getCharacter());
		i++;
	}
	return returnValue.c_str();
}

std::string FileManager::getLine(){
	if(!mFileIsOpened){
		std::cout<<"The file is not opened"<<std::endl;
		return "";
	}
	if(mOpenModeEnum != READ){
		std::cout<<"Open mode: "<<mOpenMode << " prevents from reading"<<std::endl;
		return "";
	}
	std::string line;
	std::getline (mOpenedFile,line);
	return line;
}

unsigned int FileManager::read(void* pointer, unsigned int size, unsigned int count){
	if(!mFileIsOpened){
		std::cout<<"The file is not opened"<<std::endl;
		return 0;
	}
	if(mOpenModeEnum != READ_BINARY){
		std::cout<<"Open mode: "<<mOpenMode << " prevents from reading"<<std::endl;
		return 0;
	}
	unsigned int numberOfBytesToRead = size * count;
 
	mOpenedFile.read((char*)pointer,numberOfBytesToRead);
	//return mOpenedFile.gcount(); //Número de bytes leídos (?)
	return count;
}

void FileManager::write(void* pointer, unsigned int size, unsigned int count){
	if(!mFileIsOpened){
		std::cout<<"The file is not opened"<<std::endl;
		return ;
	}
	if(mOpenModeEnum != WRITE_BINARY && mOpenModeEnum != WRITE_BINARY_ATE){
		std::cout<<"Open mode: "<<mOpenMode << " prevents from reading"<<std::endl;
		return;
	}
	unsigned int numberOfBytesToWrite = size * count;
	mOpenedFile.write((char*)pointer,numberOfBytesToWrite);
}

void FileManager::writeText(std::string text){
	if(!mFileIsOpened){
		std::cout<<"The file is not opened"<<std::endl;
		return;
	}
	if(mOpenModeEnum != WRITE && mOpenModeEnum != WRITE_ATE){
		std::cout<<"Open mode: "<<mOpenMode << " prevents from reading"<<std::endl;
		return;
	}
	mOpenedFile << text;
}

void FileManager::setPointerPosInput(unsigned int position, PointerPosition way){
	if(!mFileIsOpened){
		std::cout<<"The file is not opened"<<std::endl;
		return;
	}
	if(position == 0){
		if(way == BEGIN){
			mOpenedFile.seekg(0, mOpenedFile.beg);
		}
		if(way == END){
			mOpenedFile.seekg(0, mOpenedFile.end);
		}
		if(way == CURRENT){
			mOpenedFile.seekg(0, mOpenedFile.cur);
		}
	}else{
		mOpenedFile.seekg(position);
	}
}

void FileManager::setPointerPosOutput(unsigned int position, PointerPosition way){
	if(!mFileIsOpened){
		std::cout<<"The file is not opened"<<std::endl;
		return;
	}
	if(position == 0){
		if(way == BEGIN){
			mOpenedFile.seekp(0, mOpenedFile.beg);
		}
		if(way == END){
			mOpenedFile.seekp(0, mOpenedFile.end);
		}
		if(way == CURRENT){
			mOpenedFile.seekp(0, mOpenedFile.cur);
		}
	}else{
		mOpenedFile.seekp(position);
	}
}

unsigned int FileManager::getPointerPosInput(){
	if(!mFileIsOpened){
		std::cout<<"The file is not opened"<<std::endl;
		return 0;
	}
	return (unsigned int)(mOpenedFile.tellg());
}

unsigned int FileManager::getPointerPosOutput(){
	if(!mFileIsOpened){
		std::cout<<"The file is not opened"<<std::endl;
		return 0;
	}
	return (unsigned int)(mOpenedFile.tellp());
}

char FileManager::peek(){
	if(!mFileIsOpened){
		std::cout<<"The file is not opened"<<std::endl;
		return 0;
	}
	return mOpenedFile.peek();
}

void FileManager::synchronize(){
	if(!mFileIsOpened){
		std::cout<<"The file is not opened"<<std::endl;
		return;
	}
	if(mOpenModeEnum == READ || mOpenModeEnum == READ_BINARY || mOpenModeEnum == CLOSED){
		std::cout<<"Open mode: "<<mOpenMode << " prevents from reading"<<std::endl;
		return;
	}
	mOpenedFile.flush();
}

void FileManager::ignoreUntil(unsigned int n, unsigned int delim){
	if(!mFileIsOpened){
		std::cout<<"The file is not opened"<<std::endl;
		return;
	}
	if(delim == 0){
		mOpenedFile.ignore(n);
	}else{
		mOpenedFile.ignore(n,delim);
	}
}