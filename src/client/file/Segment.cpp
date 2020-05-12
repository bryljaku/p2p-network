#include "Segment.h"
//created by Jakub
Segment::Segment(Id id, uint8_t *data) {
    this->id = id;
    this->data = data;
    this->state = FREE;
}

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
