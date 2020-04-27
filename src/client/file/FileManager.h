#ifndef P2P_NETWORK_FILE_MANAGER_H
#define P2P_NETWORK_FILE_MANAGER_H

#include <cstddef> // size_t
#include <string>
#include <fstream>
#include <sstream> // stringstream
#include <mutex>
#include <condition_variable>
#include <vector>
#include "Segment.h"
#include "File.h"
#include "../utils/GeneralTypes.h"

class FileManager
{
	struct OpenedFile {
    	Filename fileName;
    	std::ifstream stream;
  	};

  	std::vector<OpenedFile *> readLockedFiles; 
  	std::vector<Filename> writeLockedFiles; //names of files locked for writing

  	std::mutex readLockMutex;
  	std::mutex writeLockMutex;
  	
  	std::mutex condVariableMutex;
  	std::condition_variable condVariable;

public:
	FileManager();
	~FileManager();

	void storeFile(std::shared_ptr<File> file); //stores COMPLETE file
	uint8_t* getSegment(const Filename fileName, SegmentId segment, const std::size_t segmentSize);

	bool readLock(const Filename fileName);
  	void readUnlock(const Filename fileName);

private:
	//these are used only inside storeFile method:
  	void writeLock(const Filename fileName);
  	void writeUnlock(const Filename fileName);
};




#endif //P2P_NETWORK_FILE_MANAGER_H