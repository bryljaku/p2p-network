// created by Piotr
#include "Torrent.h"

#include <utility>

void Torrent::genDefaultHash() {
    hashed = std::hash<std::string>{}(fileName);
}

void Torrent::genSaltedHash(uint32_t salt) {
    hashed = std::hash<std::string>{}(fileName.append(std::to_string(salt)));
}

Torrent::Torrent() {
    hashed = 0;
    size = 0;
    fileName = "";
}

Torrent::Torrent(uint32_t size, std::string fileName):size(size),fileName(std::move(fileName)) {
    hashed = 0;
}

Torrent::Torrent(Hash hashed, uint32_t size, std::string fileName):hashed(hashed),size(size),fileName(std::move(fileName)) {
}

Torrent::Torrent(const TorrentMessage &msg) {
    hashed = msg.hashed();
    size = msg.size();
    fileName = msg.filename();
}

TorrentMessage* Torrent::toMsg() {
    auto n = new TorrentMessage;
    n->set_hashed(hashed);
    n->set_size(size);
    n->set_filename(fileName);
    return n;
}

Torrent::Torrent(const Torrent &obj) {
    this->hashed = obj.hashed;
    this->size = obj.size;
    this->fileName = obj.fileName;
    this->currentPath = obj.currentPath;
    this->hasPath = obj.hasPath;
}

Torrent::Torrent(std::string fn) : Torrent() {
    struct stat statRes;
    std::string line;
    std::string torrentFilename = fn;
    if(stat(torrentFilename.c_str(), &statRes) == 0) {
        std::ifstream file(torrentFilename.c_str(), std::ios::in | std::ios::binary);

        getline(file, line);
        auto read_hashed = line;
        getline(file, line);
        std::string read_size = line;
        getline(file, line);
        auto read_filename = line;
        hashed = std::atol(read_hashed.c_str());
        size = std::atoi(read_size.c_str());
        fileName = read_filename;

        setPath(torrentFilename);
        file.close();
    } else {	// file can't be opened
        size = -1;
    }
}

void Torrent::saveToFile(std::string torrentFilename) {
    std::ofstream file(torrentFilename.c_str(), std::ios::out | std::ios::binary);
    file << hashed << '\n';
    file << size << '\n';
    file << fileName << '\n';
    setPath(fileName);
    file.close();
}

void Torrent::setPath(std::string path) {
    hasPath = true;
    this->currentPath = std::move(path);
}

bool Torrent::operator==(const Torrent &other) {
    return hashed == other.hashed && size == other.size && fileName == other.fileName;
}

