#include "FileManager.h"

FileManager::FileManager() {
}

FileManager::~FileManager() {
	for (OpenedFile *of : readLockedFiles) {
    	if (of != nullptr) {
      		delete of;
      	}
    }
    readLockedFiles.erase(readLockedFiles.begin(), readLockedFiles.end());
  	writeLockedFiles.erase(writeLockedFiles.begin(), writeLockedFiles.end());
}

void FileManager::storeFile(std::shared_ptr<File> file) {
	Filename fileName = file->getName();

	writeLock(fileName);

	std::ofstream fileStream(fileName, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

	if (fileStream.fail()) {
		//log error - couldn't open the file
		return;
	}

	//TODO - dataBegin is nullptr !!!!!!!!
	char* fileContents = reinterpret_cast<char *>(file->getDataBegin());

	fileStream.seekp(0);
	fileStream.write(fileContents, file->getSize());

	if(!fileStream) {
		//TODO log error while storing file
	} else {
		//TODO log success
	}

	fileStream.close();
	writeUnlock(fileName);
}

uint8_t* FileManager::getSegment(const Filename fileName, SegmentId segment, const std::size_t segmentSize) {
	if (segmentSize > DEFAULTSEGMENTSIZE) {
		//log error - requested size too big
		return nullptr;
	}

	readLockMutex.lock();
	OpenedFile* of;
	for (OpenedFile *f : readLockedFiles) {
		if (f->fileName == fileName) {
			of = f;
			break;
		}
	}
	readLockMutex.unlock();

	if (!of->stream.is_open()) {
		//log error - read failed
		return nullptr;
	}

	of->stream.seekg(segment * DEFAULTSEGMENTSIZE);
	if (of->stream.tellg() != segment * DEFAULTSEGMENTSIZE) {
		//log setting position error
		return nullptr;
	}

	uint8_t* buffer = static_cast<uint8_t *>(malloc(DEFAULTSEGMENTSIZE)); // not totally sure about that
	if (buffer == nullptr) {
		//log error
		return nullptr;
	}

	of->stream.read(reinterpret_cast<char *>(buffer), segmentSize);

	if (!of->stream || (of->stream.eof() && !of->stream.fail())) {
		//log error
		return nullptr;
	}

	// probably last segment of the file - fill with zeros
	if (segmentSize < DEFAULTSEGMENTSIZE) {
		std::fill(buffer + segmentSize, buffer + DEFAULTSEGMENTSIZE, '\0');
	}

	return buffer;
}

bool FileManager::readLock(const Filename fileName){
	bool isLocked = true;

	writeLockMutex.lock();

	while(isLocked) {
		isLocked = false;

		for (auto writeLocked : writeLockedFiles) {
			if(writeLocked == fileName) {
				isLocked = true;
				break;
			}
		}
	

		if (isLocked) {
			writeLockMutex.unlock();
			{
				std::unique_lock<std::mutex> uLock(condVariableMutex);
				condVariable.wait(uLock);
			}
			writeLockMutex.lock();
		}
	}
	writeLockMutex.unlock();

	readLockMutex.lock();
	for (OpenedFile *of : readLockedFiles) {
		if (of->fileName == fileName) {
			//file already locked 
			//no need to perform another one, multiple readers allowed
			readLockMutex.unlock();
			return true;
		}
	}

	//File is then not readLocked nor writeLocked
	OpenedFile* of = new OpenedFile;
	of->fileName = fileName;
  	of->stream.open(fileName, std::ifstream::in | std::ifstream::binary);

  	if (of->stream.fail()) {
  		readLockMutex.unlock();
  		//TODO log error - couldn't open for reading
  		return false;
  	}

  	readLockedFiles.push_back(of);

  	readLockMutex.unlock();
  	//TODO log opened for reading success
  	return true;
  }


void FileManager::readUnlock(const Filename fileName) {
	//TODO logging
	readLockMutex.lock();
	for (auto f = readLockedFiles.begin(); f != readLockedFiles.end(); ++f) {
		if (fileName == (*f)->fileName) {
			(*f)->stream.close();
			delete *f;
			readLockedFiles.erase(f);
		}
	}
	readLockMutex.unlock();
}

void FileManager::writeLock(const Filename fileName) {
	writeLockMutex.lock();

	//is the file readLocked
	bool isLocked = true;
	readLockMutex.lock();
	while (isLocked) {
		isLocked = false;
		for (OpenedFile* of : readLockedFiles) {
			if (fileName == of->fileName) {
				isLocked = true;
				//the file is readLocked
				break;
			}
			isLocked = false;
		}

		if (isLocked) {
			readLockMutex.unlock();
			writeLockMutex.unlock();
			{
				std::unique_lock<std::mutex> uLock(condVariableMutex);
				condVariable.wait(uLock);
			}
			writeLockMutex.lock();
			readLockMutex.lock();
		}
	}
	readLockMutex.unlock();

	writeLockedFiles.push_back(fileName);

	writeLockMutex.unlock();
}

void FileManager::writeUnlock(const Filename fileName) {
	//TODO logging
	writeLockMutex.lock();

	for (auto f = writeLockedFiles.begin(); f != writeLockedFiles.end(); ++f) {
	    if ((*f) == fileName) {
	      writeLockedFiles.erase(f);
	      writeLockMutex.unlock();
	      {
	        std::lock_guard<std::mutex> lGuard(condVariableMutex);
	        condVariable.notify_all();
	      }
	      return;
	    }
  	}
  writeLockMutex.unlock();
}

