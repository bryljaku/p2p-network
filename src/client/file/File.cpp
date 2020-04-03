#include "File.h"

#include <utility>

bool File::isComplete() {
    for (auto i: completeSegmentsBool)
        if (!i)
            return false;
    return true;
}

Segment File::getSegment(int id) {
    return segments[id];
}

void File::updateSegment(const Segment& segment) {
    auto segmentId = segment.getId();
    auto dataPtr = segments[segmentId].getDataPtr();
    // write to dataPtr //todo
    completeSegmentsBool[segmentId] = true;
}

int File::getSize() {
    return size;
}

SegmentId File::getSegmentIdToDownload() {
    return 0;
}

File::File(int size, std::string path) {
    this->size = size;
    this->path = path;
    
    segments = generateSegments(size);
    numOfSegments = segments.size();
    completeSegmentsBool = std::vector<bool>(this->numOfSegments, false);
    
    
}

std::vector<Segment> File::generateSegments(int size) {
    return std::vector<Segment>();
}
