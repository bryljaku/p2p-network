#ifndef P2P_NETWORK_SEGMENT_H
#define P2P_NETWORK_SEGMENT_H

#include <vector>
#include "../utils/GeneralTypes.h"

class Segment {
    public:

        Segment(SegmentId id, uint8_t *data);
        
        ~Segment();
        SegmentId getId() const;
        uint8_t *getDataPtr() const;
        
    private:
        SegmentId id;
        uint8_t *data;
};


#endif //P2P_NETWORK_SEGMENT_H
