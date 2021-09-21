//Individual tracker events

#include <vector>
#include <cstdint>
#include <iostream> //Debugging

//Unique names for the four trackers
enum trackerNames{ FirstHorizontal, SecondHorizontal, FirstVertical, SecondVertical };

class TrackerEvent{
    private:
        uint32_t triggerTimeLSB;
        uint32_t triggerTimeMSB;
        uint32_t eventTimeLSB;
        uint32_t eventTimeMSB;

        uint32_t data[6];
        //Zeroed bits are unused
        uint32_t mask[6];
    
        //Which of the 4 trackers it concerns
        uint32_t trackerID;

    public:
        TrackerEvent(std::vector<uint32_t> serializedData, uint32_t trackerID);

        double getTriggerTime();
        uint32_t getTriggerTimeLSB();
        uint32_t getTriggerTimeMSB();

        double getEventTime();
        uint32_t getEventTimeLSB();
        uint32_t getEventTimeMSB();
        
        std::vector<uint32_t> getData();

        std::vector<uint32_t> serializeEvent();

};
