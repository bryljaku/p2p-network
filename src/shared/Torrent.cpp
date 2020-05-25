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

Torrent::Torrent(std::string torrentFilename) : Torrent() {
	struct stat statRes;
	if(stat(torrentFilename.c_str(), &statRes) == 0) {
		std::ifstream file(torrentFilename.c_str(), std::ios::in | std::ios::binary);
		file.read((char*)&hashed, sizeof(Hash));
		file.read((char*)&size, sizeof(uint32_t));
		uint32_t nameSize;
		file.read((char*)&nameSize, sizeof(uint32_t));
		fileName.resize(nameSize);
		file.read((char*)fileName.data(), nameSize);
		setPath(torrentFilename);
		file.close();
	} else {	// file can't be opened
		size = -1;
	}
}

void Torrent::saveToFile(std::string torrentFilename) {
	std::ofstream file(torrentFilename.c_str(), std::ios::out | std::ios::binary);
	file.write((char*)&hashed, sizeof(Hash));
	file.write((char*)&size, sizeof(uint32_t));
	uint32_t nameSize = fileName.length();
	file.write((char*)&nameSize, sizeof(uint32_t));
	file.write((char*)fileName.data(), nameSize);
	setPath(fileName);
	file.close();
}

void Torrent::setPath(std::string path) {
	hasPath = true;
	this->currentPath = std::move(path);
}

