#ifndef P2P_NETWORK_FILE_H
#define P2P_NETWORK_FILE_H


#include <vector>
#include <memory>
#include "Segment.h"
#include "../utils/GeneralTypes.h"
#include "PeerInfo.h"


class File {
    Id id;
    Filename name;
    std::vector<Segment> segments;
    int numOfSegments;
    int size;
    std::string path;
    std::vector<bool> completeSegmentsBool;
    std::vector<std::shared_ptr<PeerInfo>> peers;
    uint8_t *dataBegin;
    uint8_t *dataEnd;
    
public:
    File(Id id, Filename name, int size, std::string path);
    bool isComplete();
    Segment getSegment(int id);
    Id getId();
    Filename getName();
    std::string getPath();
    std::vector<std::shared_ptr<PeerInfo>> getPeers();
    int getNumOfSegments();
    int getSize();

    uint8_t* getDataBegin();
    SegmentId getSegmentIdToDownload();

    void addPeer(PeerInfo peer);
    Id getSegmentIdToDownload();

private:
    void generateSegments();
};


#endif //P2P_NETWORK_FILE_H
