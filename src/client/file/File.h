#ifndef P2P_NETWORK_FILE_H
#define P2P_NETWORK_FILE_H


#include <vector>
#include <memory>
#include "Segment.h"
#include "../utils/GeneralTypes.h"
#include "PeerInfo.h"
#include <mutex>
#include <Torrent.h>
//created by Jakub

class File {
    Id id = -1;
    Torrent torrent;
    std::vector<Segment> segments;
    int numOfSegments = -1;
    int size = -1;
    std::string path = "";
    std::vector<bool> completeSegmentsBool;
    std::vector<std::shared_ptr<PeerInfo>> peers;
    uint8_t *dataBegin;
    uint8_t *dataEnd;
    std::vector<std::mutex *> my_mutexes;
public:
    File(Id id, int size, Torrent& torrent, std::string path);
    bool isComplete();
    Segment getSegment(int id);
    Id getId();
    Torrent& getTorrent();
    std::string getPath();
    std::vector<std::shared_ptr<PeerInfo>> getPeers();
    int getNumOfSegments();
    int getSize();
    void addPeer(PeerInfo peer);
    SegmentState getSegmentState(Id segmentId);
    void setSegmentState(Id segmentId, SegmentState newState);
private:
    void generateSegments();
};


#endif //P2P_NETWORK_FILE_H