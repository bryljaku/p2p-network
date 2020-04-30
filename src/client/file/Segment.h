#ifndef P2P_NETWORK_SEGMENT_H
#define P2P_NETWORK_SEGMENT_H

#include <vector>
#include "../utils/GeneralTypes.h"
enum SegmentState {
    FREE,
    DOWNLOADING,
    COMPLETE
};

class Segment {
    public:
        Segment(Id id, uint8_t *data);
        ~Segment() = default;
        Id getId() const;
        uint8_t *getDataPtr() const;
        void setSegmentState(SegmentState state);
        SegmentState getSegmentState();
    private:
        SegmentState state;
        Id id;
        uint8_t *data;
};


#endif //P2P_NETWORK_SEGMENT_H
