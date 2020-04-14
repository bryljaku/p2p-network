#include "Segment.h"

Segment::Segment(SegmentId id, uint8_t *data) {
    this->id = id;
    this->data = data;
}

uint8_t *Segment::getDataPtr() const {
    return data;
}

SegmentId Segment::getId() const {
    return id;
}
