// DIP Client to read the XBPF fiber tracker data during the LDMX testbeam
//
// Contact: Erik Wallin
//
// Adapted from dip-client.cpp example


#include "Dip.h"
#include "DipSubscription.h"

#include <string>
#include <unistd.h>
#include <sstream>

class FiberTrackerClient {
    private:
        DipSubscription **sub;
        DipFactory *dip;

        class FiberTrackerDataListener:public DipSubscriptionListener{
            private:
                //To access subscriptions
                FiberTrackerClient* client;

            public:
                FiberTrackerDataListener(FiberTrackerClient* c):client(c){};

                void handleMessage(DipSubscription *subscription, DipData &message){
                    //What kind of strings are returned throughout? Several possibilities found in DIPData.h
                    
                    //Are fields zero-indexed and in the order given in the specification document?
                    //Are the "field_x" tags always like this or user defined to be something else?
                    
                    //Check with DipData::getValueType and DipData::getTags

                    //const std::string acqMode = message.extractStringArray("field_0");
                    //int acqStamp = message.extractInt("field_1");
                    //std::string cycleName = message.extractStringArray("field_2");
                    //Some more types available, such as Dipint etc. check with getValueType()
                    //int cycleStamp = message.extractInt("field_3");
                    //std::string equipmentName = message.extractStringArray("field_4");
                    //int countsRecords = message.extractInt("field_5");
                    //Note that only 96 of 192 fibers are used, s.t. the first and last 36 bits are zero
                    //int[10*countsRecords] eventsData = message.extractIntArray(countsRecord*10, "field_6");
                    //int countsTrigs = message.extractInt("field_7");
                    //int countsRecordsWithZeroEvents = message.extractInt("field_8");
                    //int counts = message.extractInt("field_9");
                    //int[192] profile = message.extractInt("field_10");
                    //double mean = message.extractInt("field_11");
                    //int timeFirstEvent = message.extractInt("field_12");
                    //int timeFirstTrigger = message.extractInt("field_13");
                    //int timeLastEvent = message.extractInt("field_14");
                    //int timeLastTrigger = message.extractInt("field_15");
                    //std::string msg = message.extractInt("field_16");
                    //std::string acqType = message.extractInt("field_17");
                    //std:string acqTypeAllowed = message.extractInt("field_18");
                    //int[192] profileMm = message.extractInt("field_19");

                    //Do something with this, e.g. send it to Rogue
                }

                void connected(DipSubscription *arg0){}

                void disconnected(DipSubscription *arg0, char *arg1){}

                void handleException(DipSubscription* subscription, DipException& ex){}
        };
    
    public:
        FiberTrackerDataListener* handler;

        FiberTrackerClient(const int argc, const char ** argv){
            //Subscribe to several publications
            //Like the 4 (?) trackers
            int numberOfPubs = 1;
            dip = Dip::create("dip-client");
            handler = new FiberTrackerDataListener(this);
            sub = new DipSubscription*[numberOfPubs];
            //What should this be?
            dip->setDNSNode("localhost");

            for(int i = 0 ; i < numberOfPubs ; i++){
                std::ostringstream oss;
                oss << "dipServerAddress" << "_" << i;
                sub[i] = dip->createDipSubscription(oss.str().c_str(), handler);
            }
        }

        ~FiberTrackerClient(){
            // [0] ?
            dip->destroyDipSubscription(sub[0]);
            delete handler;
            delete dip;
        }

};

//Mockup main loop running the client for a minute
int main(const int argc, const char ** argv){
    //Fiber tracker class created and will listen until it is destroyed
    //We should figure out where this happens
    FiberTrackerClient* client = new FiberTrackerClient(argc, argv);

    //Sleep a minute just so this program doesn't terminate
    sleep(60000);

    delete client;

    return 0;
}
