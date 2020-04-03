#ifndef P2P_NETWORK_FILE_H
#define P2P_NETWORK_FILE_H


#include <vector>
#include "Segment.h"
#include "../utils/GeneralTypes.h"


class File {
    std::vector<Segment> segments;
    int numOfSegments;
    int size;
    std::string path;
    std::vector<bool> completeSegmentsBool;
public:
    File(int size, std::string path);
    bool isComplete();
    Segment getSegment(int id);
    void updateSegment(const Segment& segment);
    int getSize();
    SegmentId getSegmentIdToDownload();

private:
    
    std::vector<Segment> generateSegments(int size);
    
    
    
};


#endif //P2P_NETWORK_FILE_H
