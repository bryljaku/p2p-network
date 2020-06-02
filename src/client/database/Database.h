#ifndef P2P_NETWORK_DATABASE_H
#define P2P_NETWORK_DATABASE_H
#include <vector>
#include "../file/File.h"
#include <algorithm>
#include <fstream>
#include <sharedUtils.h>
#include <memory>

class Database {
    std::vector<std::shared_ptr<File>> files = std::vector<std::shared_ptr<File>>();
public:
    std::vector<std::shared_ptr<File>> &getFiles();
    void addFile(File file);
    std::shared_ptr<File> &getFile(Id id);

    bool isFileInDatabase(Torrent &torrent); //checks file existence using its torrent's hashed value

    int loadFromFile(std::string filename);
    int saveToFile(std::string filename);
};


#endif //P2P_NETWORK_DATABASE_H
