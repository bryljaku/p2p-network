#include "File.h"

#include <utility>
#include <sharedUtils.h>
//#include <spdlog/spdlog.h>

bool File::isComplete() {
    for (auto s: segments)
        if (COMPLETE != s.getSegmentState())
            return false;
    return true;
}

Segment File::getSegment(int id) {
    return segments[id];
}

int File::getSize() {
    return size;
}

Torrent& File::getTorrent() {
	return torrent;
}


File::File(Id id, int size, Torrent& torrent, std::string path) {
    this->size = size;
    this->torrent = torrent;
    this->path = std::move(path);
    this->id = id;
    peers = std::vector<std::shared_ptr<PeerInfo>>();
    generateSegments();
    numOfSegments = segments.size();
    completeSegmentsBool = std::vector<bool>(this->numOfSegments, false);
    this->dataBegin = nullptr; // todo - it should be some dataptr to file on disk
//    this->dataEnd = this->dataBegin + size;
    syslogger->info("Created file with id {}", id);
    
}

void File::generateSegments() {
    if (size % DEFAULTSEGMENTSIZE == 0)
        this->numOfSegments = size / DEFAULTSEGMENTSIZE;
    else
        this->numOfSegments = size / DEFAULTSEGMENTSIZE + 1;
    
    for (int i = 0; i < numOfSegments; i++)
        this->segments.emplace_back(Segment(i, this->dataBegin + DEFAULTSEGMENTSIZE * i));
//    std::vector<std::mutex *> mut(numOfSegments);
//    my_mutexes = mut;
//    for (int i = 0; i < numOfSegments; i++)
//        my_mutexes[i] = new std::mutex();
    
    syslogger->info("Generated {} segments for file with id {}", numOfSegments, id);
}

std::vector<std::shared_ptr<PeerInfo>> File::getPeers() {
    return peers;
}

Id File::getId() {
    return id;
}

int File::getNumOfSegments() {
    return numOfSegments;
}

std::string File::getPath() {
    return path;
}

void File::addPeer(PeerInfo peer) {
    peers.emplace_back(std::make_shared<PeerInfo>(peer));
    syslogger->info("File {} added peer {}",id, peer.getId());
}

//todo add concurrency
void File::setSegmentState(int segmentId, SegmentState newState) {
    segments[segmentId].setSegmentState(newState);
    syslogger->info("File {} set segment {} state to {}",id,  segmentId, newState);
}

SegmentState File::getSegmentState(Id segmentId) {
    return segments[segmentId].getSegmentState();
}