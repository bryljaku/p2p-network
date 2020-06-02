#ifndef P2P_NETWORK_FILE_H
#define P2P_NETWORK_FILE_H


#include <vector>
#include <memory>
#include "Segment.h"
#include "GeneralTypes.h"
#include "PeerInfo.h"
#include <mutex>
#include <Torrent.h>
#include <utility>
#include <sharedUtils.h>
//created by Jakub

class File {
	Torrent torrent;
    std::vector<Segment> segments;
    int numOfSegments = -1;
    int size = -1;
    std::string path = "";
    std::vector<std::shared_ptr<PeerInfo>> peers;
    uint8_t *dataBegin;
    bool isCompleted;
public:
    File(const Torrent& torrent, std::string path);
    bool isComplete();
    Segment getSegment(int id);
    Id getId();
    Torrent& getTorrent();
    std::string getPath();
    std::vector<std::shared_ptr<PeerInfo>> getPeers();
    int getNumOfSegments();
    int getSize();

    uint8_t* getDataBegin();

    void addPeer(PeerInfo peer);

    SegmentState getSegmentState(Id segmentId);
    void setSegmentState(Id segmentId, SegmentState newState);

    bool tryToSetStateSegmentStateToDownload(Id id);
private:

    void generateSegments();
};


#endif //P2P_NETWORK_FILE_H