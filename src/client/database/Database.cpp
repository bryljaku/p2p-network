#include <algorithm>
#include <spdlog/spdlog.h>
#include <fstream>
#include <sharedUtils.h>
#include "Database.h"
//created by Jakub
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

std::vector<std::shared_ptr<File>> &Database::getFiles() {
    return files;
}

std::shared_ptr<File>& Database::getFile(Id id) {
    for(auto &f: files)
        if (f.get()->getId() == id)
            return f;
   // return nullptr; //todo jak to poprawic? pls help
    spdlog::warn("Couldn't find file with id {}", id);
}

std::shared_ptr<File>& Database::getFile(std::string path) {
    for(auto &f: files)
        if (f.get()->getPath() == path)
            return f;
    spdlog::warn("Couldn't find file with path {}", path);
   // return nullptr; //todo jak to poprawic? pls help
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
		File f(t, t.fileName);	//TODO: czy to ok? zakladamy ze pliki pobierane i zapisywane są do tego samego folderu co program
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
