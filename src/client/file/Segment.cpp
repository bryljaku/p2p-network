#include "Segment.h"

Segment::Segment(Id id, uint8_t *data) {
    this->id = id;
    this->data = data;
}

uint8_t *Segment::getDataPtr() const {
    return data;
}

Id Segment::getId() const {
    return id;
}
