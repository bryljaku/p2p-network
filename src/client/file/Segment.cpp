#include "Segment.h"

Segment::Segment(Id id, uint8_t *data) {
    this->id = id;
    this->data = data;
    this->state = FREE;
}

uint8_t *Segment::getDataPtr() const {
    return data;
}
void Segment::setSegmentState(SegmentState state){
    this->state = state;
}

Id Segment::getId() const {
    return id;
}

SegmentState Segment::getSegmentState() {
    return state;
}
