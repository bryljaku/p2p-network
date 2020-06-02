
#include "Database.h"


void Database::addFile(File file) {
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

int Database::loadFromFile(std::string filename) {
	struct stat statRes;
	if(stat(filename.c_str(), &statRes) != 0) { // file can't be opened
		return 1;
	}

	getFiles().clear();

	std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
	uint16_t torrentCount;
	file.read((char*)&torrentCount, sizeof(uint16_t));
	for(int i=0; i<torrentCount; i++) {
		uint32_t nameSize;
		file.read((char*)&nameSize, sizeof(uint32_t));
		std::string torrentPath;
		torrentPath.resize(nameSize);
		file.read((char*)torrentPath.data(), nameSize);
		if(stat(torrentPath.c_str(), &statRes) != 0) {
			syslogger->info("Missing torrent file '{}'!", torrentPath);
			continue;
		}
		Torrent t(torrentPath);
		File f(t, t.fileName);
		addFile(f);
	}
	file.close();
	return 0;
}

int Database::saveToFile(std::string filename) {
	std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);
	if(!file.is_open()) {
		return 1;
	}
	uint16_t torrentCount = getFiles().size();
	file.write((char*)&torrentCount, sizeof(uint16_t));
	for(auto f : getFiles()) {
		if(!f->getTorrent().hasPath) {
			continue;
		}
		uint32_t nameSize = f->getTorrent().currentPath.size();
		file.write((char*)&nameSize, sizeof(uint32_t));
		file.write(f->getTorrent().currentPath.data(), nameSize);
	}
	file.close();
	return 0;
}
