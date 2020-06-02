//created by Jakub
#include "File.h"
#include <utility>
#include <sharedUtils.h>

bool File::isComplete() {
    for (int i = 0; i < segments.size();i++)
        if (COMPLETE != segments[i].getSegmentState())
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



File::File(const Torrent& torrent, std::string path) {
    this->size = torrent.size;
    this->torrent = torrent;
    this->path = std::move(path);
    peers = std::vector<std::shared_ptr<PeerInfo>>();
    generateSegments();
    numOfSegments = segments.size();
    completeSegmentsBool = std::vector<bool>(this->numOfSegments, false);
    this->dataBegin = nullptr; // todo - it should be some dataptr to file on disk
//    this->dataEnd = this->dataBegin + size;
    syslogger->info("Created file with id {}", getId());
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
    
    syslogger->info("Generated {} segments for file with id {}", numOfSegments, getId());
}

std::vector<std::shared_ptr<PeerInfo>> File::getPeers() {
    return peers;
}

Id File::getId() {
    return torrent.hashed;
}

int File::getNumOfSegments() {
    return numOfSegments;
}

std::string File::getPath() {
    return path;
}

uint8_t* File::getDataBegin() {
    return dataBegin;
}

void File::addPeer(PeerInfo peer) {
    peers.emplace_back(std::make_shared<PeerInfo>(peer));
    syslogger->info("File {} added peer {}", getId(), peer.getId());
}



bool File::tryToSetStateSegmentStateToDownload(Id id) {
    return segments[id].tryToSetToDownload();
}

void File::setSegmentState(int segmentId, SegmentState newState) {
    segments[segmentId].setSegmentState(newState);
    syslogger->info("File {} set segment {} state to {}", getId(), segmentId, newState);
}

SegmentState File::getSegmentState(Id segmentId) {
    return segments[segmentId].getSegmentState();
}
