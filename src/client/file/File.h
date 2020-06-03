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
    std::vector<std::shared_ptr<Segment>> segments;
    int numOfSegments = -1;
    int size = -1;
    std::string path = "";
    std::vector<std::shared_ptr<PeerInfo>> peers;
    uint8_t *dataBegin{};
    bool isCompleted{};
    std::mutex fileMutex;
public:
    File(const File& file);
    File(const Torrent& torrent, std::string path);
    bool isComplete();
    std::shared_ptr<Segment> getSegment(int id);
    Id getId() const;
    Torrent& getTorrent();
    std::string getPath();
    std::vector<std::shared_ptr<PeerInfo>> getPeers();
    int getNumOfSegments() const;
    int getSize() const;

    uint8_t* getDataBegin();

    void addPeer(PeerInfo peer);

    SegmentState getSegmentState(Id segmentId);
    void setSegmentState(int segmentId, SegmentState newState);

    bool tryToSetStateSegmentStateToDownload(Id id);

	void markComplete();

private:

    void generateSegments();

};


#endif //P2P_NETWORK_FILE_H