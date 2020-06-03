#include "Segment.h"
//created by Jakub

void Segment::setSegmentState(SegmentState _state){
    this->state = _state;
}

Id Segment::getId() const {
    return id;
}

SegmentState Segment::getSegmentState() {
    return state;
}

Segment::Segment(Id id, SegmentState segmentState) {
    this->id = id;
    this->state = segmentState;
}

bool Segment::tryToSetToDownload() {
    if (state == FREE) {
        state = DOWNLOADING;
        return true;
    }
    return false;
}
