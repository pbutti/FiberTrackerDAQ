#include "trackerEvent.h"

TrackerEvent::TrackerEvent(std::vector<uint32_t> s, uint32_t id){
    std::cout << s.at(0) << std::endl;
    triggerTimeLSB = s.at(0);
    triggerTimeMSB = s.at(1);
    eventTimeLSB = s.at(2);
    eventTimeMSB = s.at(3);
    data[0] = s.at(4);
    data[1] = s.at(5);
    data[2] = s.at(6);
    data[3] = s.at(7);
    data[4] = s.at(8);
    data[5] = s.at(9);

    trackerID = id;
};
