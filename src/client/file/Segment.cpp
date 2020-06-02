#include "Segment.h"
//created by Jakub

uint8_t *Segment::getDataPtr() const {
    return data;
}
void Segment::setSegmentState(SegmentState _state){
    this->state = _state;
}

Id Segment::getId() const {
    return id;
}

SegmentState Segment::getSegmentState() {
    return state;
}

Segment::Segment(Id id, uint8_t *data,SegmentState segmentState) {
    this->id = id;
    this->data = data;
    this->state = segmentState;
}

bool Segment::tryToSetToDownload() {
    if (state == FREE) {
        state = DOWNLOADING;
        return true;
    }
    return false;
}
