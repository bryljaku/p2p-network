//created by Jakub
#include "File.h"

void File::addPeer(PeerInfo peer) {
    std::unique_lock<std::mutex> lk{fileMutex};
    peers.emplace_back(std::make_shared<PeerInfo>(peer));
    syslogger->info("File {} added peer {}", getId(), peer.getId());
}

bool File::tryToSetStateSegmentStateToDownload(Id id) {
    std::unique_lock<std::mutex> lk{fileMutex};
    if (completed_segments[id])
        return false;

    if (!busy_segments[id]) {
        busy_segments[id] = true;
        segments[id].setSegmentState(DOWNLOADING);
        return true;
    }
    return false;
}

void File::setSegmentState(int segmentId, SegmentState newState) {
    std::unique_lock<std::mutex> lk{fileMutex};
    segments[segmentId].setSegmentState(newState);
    if (newState == FREE) {
        completed_segments[segmentId] = false;
        busy_segments[segmentId] = false;
    } else if (newState == COMPLETE) {
      completed_segments[segmentId] = false;
      busy_segments[segmentId] = false;
    } else {
        syslogger->error("File {} invalid use of setSegmentState. ",getId());
        throw std::logic_error("");
    }
    syslogger->info("File {} set segment {} state to {}", getId(), segmentId, newState);
}

SegmentState File::getSegmentState(Id segmentId) {
    std::unique_lock<std::mutex> lk{fileMutex};
    return segments[segmentId].getSegmentState();
}
bool File::isComplete() {
    std::unique_lock<std::mutex> lk{fileMutex};
    if (!isCompleted) {
        auto maybeCompleted = true;
        for (int i = 0; i < segments.size(); i++)
            if (COMPLETE != segments[i].getSegmentState()) {
                maybeCompleted = false;
                break;
            }
        isCompleted = maybeCompleted;
    }
    return isCompleted;
}


File::File(const File& other) {}
File::File(const Torrent& torrent, std::string path) {
    this->size = torrent.size;
    this->torrent = torrent;
    this->path = std::move(path);
    peers = std::vector<std::shared_ptr<PeerInfo>>();
    generateSegments();
    numOfSegments = segments.size();
    this->busy_segments = boost::dynamic_bitset(segments.size());
    this->completed_segments = boost::dynamic_bitset(segments.size());
    this->dataBegin = nullptr;
    syslogger->info("Created file with id {}", getId());
}
Segment File::getSegment(int id) {
    std::unique_lock<std::mutex> lk{fileMutex};
    return segments[id];
}

int File::getSize() {
    return size;
}

Torrent& File::getTorrent() {
    return torrent;
}

void File::generateSegments() {
    if (size % DEFAULTSEGMENTSIZE == 0)
        this->numOfSegments = size / DEFAULTSEGMENTSIZE;
    else
        this->numOfSegments = size / DEFAULTSEGMENTSIZE + 1;

    for (int i = 0; i < numOfSegments; i++)
        this->segments.emplace_back(Segment(i, this->dataBegin + DEFAULTSEGMENTSIZE * i, SegmentState::FREE));
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