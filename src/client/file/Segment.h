#ifndef P2P_NETWORK_SEGMENT_H
#define P2P_NETWORK_SEGMENT_H

#include <vector>
#include "../utils/GeneralTypes.h"

class Segment {
    public:
        Segment(Id id, uint8_t *data);
        ~Segment() = default;
        Id getId() const;
        uint8_t *getDataPtr() const;
    private:
        Id id;
        uint8_t *data;
};


#endif //P2P_NETWORK_SEGMENT_H
