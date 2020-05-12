#ifndef P2P_NETWORK_FILE_H
#define P2P_NETWORK_FILE_H


#include <vector>
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
    std::vector<PeerInfo> peers;
    uint8_t *dataBegin;
    uint8_t *dataEnd;
    
public:
    File(Id id, Filename name, int size, std::string path);
    bool isComplete();
    Segment getSegment(int id);
    Id getId();
    Filename getName();
    std::string getPath();
    std::vector<PeerInfo> getPeers();
    int getNumOfSegments();
    int getSize();
    uint8_t* getDataBegin();
    SegmentId getSegmentIdToDownload();


private:
    void generateSegments();
    
};


#endif //P2P_NETWORK_FILE_H
