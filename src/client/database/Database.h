#ifndef P2P_NETWORK_DATABASE_H
#define P2P_NETWORK_DATABASE_H
//created by Jakub

#include <vector>
#include "../file/File.h"

#include <memory>

class Database {
    std::vector<std::shared_ptr<File>> files = std::vector<std::shared_ptr<File>>();
public:
    std::vector<std::shared_ptr<File>> getFiles();
    void addFile(File file);
    void removeFile(Id id);
    void removeFile(std::string path);
    std::shared_ptr<File> getFile(Id id);
    std::shared_ptr<File> getFile(std::string path);

};


#endif //P2P_NETWORK_DATABASE_H
