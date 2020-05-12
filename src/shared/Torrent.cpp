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
	fileName = nullptr;
}

Torrent::Torrent(uint32_t size, std::string fileName):size(size),fileName(std::move(fileName)) {
	hashed = 0;
}

Torrent::Torrent(size_t hashed, uint32_t size, std::string fileName):hashed(hashed),size(size),fileName(std::move(fileName)) {
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

