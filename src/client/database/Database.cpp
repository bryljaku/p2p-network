
#include "Database.h"


void Database::addFile(File &file) {
    if(isFileInDatabase(file.getTorrent())) {
        syslogger->info("Trying to add already existing torrent ", file.getId());
        return;
    }
    files.emplace_back(std::make_shared<File>(file));
    syslogger->info("Added file {} to database", file.getId());
}

std::vector<std::shared_ptr<File>> &Database::getFiles() {
    return files;
}

std::shared_ptr<File>& Database::getFile(Id id) {
    for(auto &f: files)
        if (f.get()->getId() == id)
            return f;
    syslogger->warn("Couldn't find file with id {}", id);
}

bool Database::isFileInDatabase(Torrent &torrent) const {
    for(auto &f: files)
        if (f.get()->getTorrent().hashed == torrent.hashed)
            return true;
    syslogger->warn("Couldn't find requested file");
    return false;
}

int Database::loadFromFile(const std::string& filename) {
    struct stat statRes{};
    if(stat(filename.c_str(), &statRes) != 0) { // file can't be opened
        return 1;
    }

    getFiles().clear();

    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
    std::string line;
    if (file.is_open())
    {
        while ( getline (file,line) ) {
            const auto& torrentPath = line;
            if(stat(torrentPath.c_str(), &statRes) != 0) {
                syslogger->info("Missing torrent file '{}'!", torrentPath);
                continue;
            }
            Torrent t(torrentPath);
            File f(t, t.fileName);
            addFile(f);
        }
        file.close();
    }
    file.close();
    return 0;
}

int Database::saveToFile(const std::string& filename) {
    if(getFiles().empty()) {
        return 0;
    }
    std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);
    if(!file.is_open()) {
        return 1;
    }
    for(auto f : getFiles()) {
        if(!f->getTorrent().hasPath) {
            continue;
        }
        file << f->getTorrent().currentPath + ".torrent" << '\n';
    }
    file.close();
    return 0;
}
