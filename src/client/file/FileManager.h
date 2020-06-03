#ifndef P2P_NETWORK_FILE_MANAGER_H
#define P2P_NETWORK_FILE_MANAGER_H

#include <cstddef> // size_t
#include <string>
#include <fstream>
#include <sstream> // stringstream
#include <mutex>
#include <condition_variable>
#include <vector>
#include <utility>
#include <sharedUtils.h>
#include "Segment.h"
#include "File.h"
#include "GeneralTypes.h"
#include "../../shared/Torrent.h"
#include "../database/Database.h"

class FileManager
{
    struct OpenedFile {
        Filename fileName;
        std::ifstream stream;
    };

    std::shared_ptr<Database> database; // client's local files

    std::vector<OpenedFile *> readLockedFiles;
    std::vector<Filename> writeLockedFiles;

    std::mutex readLockMutex;
    std::mutex writeLockMutex;

    std::mutex condVariableMutex;
    std::condition_variable condVariable;

public:
    FileManager();
    FileManager(std::shared_ptr<Database> database);
    ~FileManager();

    void storeFile(std::shared_ptr<File> file); //stores COMPLETE file
    void storeSegmentToFile(const Filename fileName, const std::string path, const Id segmentId, const std::string& segmentData);
    char* getSegment(const Filename fileName, const Id segmentId, const std::size_t segmentSize = DEFAULTSEGMENTSIZE);
    //void addFile(Id id, Filename name, int size, std::string path);
    //bool fileExists(const Filename fileName);
    void createLocalFile(Torrent& torrent, std::string path = "./"); //create BINARY file from given Torrent

    bool readLock(const Filename fileName);
    void readUnlock(const Filename fileName);


private:
    //these are used only inside storeFile and storeSegmentToFile methods:
    void writeLock(const Filename fileName);
    void writeUnlock(const Filename fileName);
};




#endif //P2P_NETWORK_FILE_MANAGER_H