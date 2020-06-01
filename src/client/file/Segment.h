#ifndef P2P_NETWORK_SEGMENT_H
#define P2P_NETWORK_SEGMENT_H
// created by Jakub
#include <vector>
#include "GeneralTypes.h"
#include <atomic>
enum SegmentState {
    FREE,
    DOWNLOADING,
    COMPLETE
};

class Segment {
    public:
        Segment(Id id, uint8_t *data, SegmentState segmentState = SegmentState::FREE);
        ~Segment() = default;
        Id getId() const;
        uint8_t *getDataPtr() const;
        void setSegmentState(SegmentState _state);
        SegmentState getSegmentState();
        bool tryToSetToDownload();
    Segment(const Segment& pOther) {
        this->id= pOther.getId();
        this->data = pOther.getDataPtr();
        this->state = pOther.state.load();
    }
private:
    std::atomic<SegmentState> state {SegmentState::FREE};
    Id id;
    uint8_t *data;
};


#endif //P2P_NETWORK_SEGMENT_H
