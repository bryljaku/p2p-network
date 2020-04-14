#include <algorithm>
#include <spdlog/spdlog.h>
#include "Database.h"

void Database::removeFile(std::string path) {
    auto oldSize = files.size();
    std::vector<std::shared_ptr<File>>::iterator new_end;
    new_end = std::remove_if(files.begin(), files.end(),
                             [path](std::shared_ptr<File> file) { return file.get()->getPath() == path; });
    files.erase(new_end, files.end());
    
    if (files.size() != oldSize)
        spdlog::info("Deleted file with path {}", path);
    else
        spdlog::info("File with path {} not found. Not deleted", path);
}

void Database::removeFile(Id id) {
    auto oldSize = files.size();
    std::vector<std::shared_ptr<File>>::iterator new_end;
    new_end = std::remove_if(files.begin(), files.end(),
                             [id](std::shared_ptr<File> file) { return file.get()->getId() == id; });
    files.erase(new_end, files.end());
    
    if (files.size() != oldSize)
        spdlog::info("Deleted file with id {}", id);
    else
        spdlog::info("File with id {} not found. Not deleted", id);
}

void Database::addFile(File file) {
    files.emplace_back(std::make_shared<File>(file));
    spdlog::info("Added file {} to database", file.getId());
}

std::vector<std::shared_ptr<File>> Database::getFiles() {
    return files;
}

std::shared_ptr<File> Database::getFile(Id id) {
    for(auto &f: files)
        if (f.get()->getId() == id)
            return f;
    return nullptr; //todo jak to poprawic? pls help
    spdlog::warn("Couldn't find file with id {}", id);
}

std::shared_ptr<File> Database::getFile(std::string path) {
    for(auto &f: files)
        if (f.get()->getPath() == path)
            return f;
    spdlog::warn("Couldn't find file with path {}", path);
    return nullptr; //todo jak to poprawic? pls help
}
