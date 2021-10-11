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
#include <vector>

#include <bitset>

//Does not compile if included before Dip?

class FiberTrackerClient {
    private:
        DipSubscription **sub;
        DipFactory *dip;

        class FiberTrackerDataListener:public DipSubscriptionListener{
            private:
                //To access subscriptions
                FiberTrackerClient* client;

            public:
                FiberTrackerDataListener(FiberTrackerClient* c):client(c){}

                void handleMessage(DipSubscription *subscription, DipData &message){
		    std::cout<<"Received data from "<<subscription->getTopicName()<<std::endl;

		    /*
		    int nTags;
		    const char ** tags = message.getTags(nTags); 
		    std::cout << "nTags = " << nTags << std::endl;
		    for(int i = 0; i < nTags; i++){
		    	std::cout << tags[i] << " : " << message.getValueType(tags[i]) << std::endl;	    
		    }
		    */
		   
		    const int acqMode = message.extractInt("acqMode");
		    const long acqStamp = message.extractLong("acqStamp");
		    const int actType = message.extractInt("acqType");
		    const int acqTypeAllowed = message.extractInt("acqTypeAllowed");
		    const char* coincidenceInUse = message.extractCString( "coincidenceInUse");
		    const long counts = message.extractLong("counts");
		    std::cout << counts << std::endl;
		    const long countsRecords = message.extractLong("countsRecords");
		    //std::cout << "countsRecords = " << countsRecords << std::endl;
		    const long countsRecordsWithZeroEvents = message.extractLong("countsRecordsWithZeroEvents");
		    //std::cout << "countsRecordsWithZeroEvents = " << countsRecordsWithZeroEvents << std::endl;
		    const long countTrigs = message.extractLong("countsTrigs");
		    const char* cycleName = message.extractCString("cycleName");
		    //std::cout << cycleName << std::endl;

		    const long cycleStamp = message.extractLong("cycleStamp");
		    const char* equipmentName = message.extractCString("equipmentName");
		    //std::cout << equipmentName << std::endl;
		    const int eventSelectionAcq = message.extractInt("eventSelectionAcq");

		    int eventsDataSize;
		    const DipInt* eventsData = message.extractIntArray(eventsDataSize, "eventsData");
		    //std::cout << eventsDataSize << std::endl;

		    for(int i = 0; i < 5; i++){
		    std::cout << eventsData[i*10+0] << " " <<eventsData[i*10+1] << " " << eventsData[i*10+2] << " "<< eventsData[i*10+3] << " : "<< std::bitset<32>(eventsData[i*10+4]) << std::bitset<32>(eventsData[i*10+5])
		        << std::bitset<32>(eventsData[i*10+6]) << std::bitset<32>(eventsData[i*10+7]) << std::bitset<32>(eventsData[i*10+8]) << std::bitset<32>(eventsData[i*10+9]) << std::endl; 
		    }
		    //std::cout << sizeof(int) << std::endl;

		    const double mean = message.extractDouble("mean");
		    const char* msg = message.extractCString("message");
		    //std::cout << msg << std::endl;

		    int profileSize;
		    const double* profile = message.extractDoubleArray(profileSize, "profile");
		    std::cout << profileSize << std::endl;

		    int profile1DSize;
		    const DipLong* profile1D = message.extractLongArray(profile1DSize, "profile1D");
		    //std::cout << size << std::endl;
		    //for(int i = 0; i < size; i++){
		    //	std::cout << profile1D[i] << ",";
		    //}
		    
		    const int profileSourceSettingAcq = message.extractInt("profileSourceSettingAcq");
		    int profileStandAloneSize;
		    const double* profileStandAlone = message.extractDoubleArray(profileStandAloneSize, "profileStandAlone");

		    const char* timeFirstEvent = message.extractCString("timeFirstEvent");
		    std::cout << timeFirstEvent << std::endl;
		    const char* timeFirstTrigger = message.extractCString("timeFirstTrigger");
		    const char* timeLastEvent = message.extractCString("timeLastEvent");
		    const char* timeLastTrigger = message.extractCString("timeLastTrigger");

		    const int trigger = message.extractInt("trigger");
		    const int triggerOffsetAcq = message.extractInt("triggerOffsetAcq");
		    const int triggerSelectionAcq = message.extractInt("triggerSelectionAcq");

                }

		void connected(DipSubscription *arg0) {
			std::cout << "\nPublication source  " << arg0->getTopicName()<< " available\n";
        	}

                void disconnected(DipSubscription *arg0, char *arg1){}

		void handleException(DipSubscription* subscription, DipException& ex){
                	printf("Subs %s has error %s\n", subscription->getTopicName(), ex.what());
                }

        };
    
    public:
        FiberTrackerDataListener* handler;

        FiberTrackerClient(const int argc, const char ** argv){
	    
            //Subscribe to several publications (the 4 trackers eventually)
            int numberOfPubs = 4;
            dip = Dip::create("dip-client");
            handler = new FiberTrackerDataListener(this);
            sub = new DipSubscription*[numberOfPubs];
            dip->setDNSNode("dipnsgpn1,dipnsgpn2");
	    //sub[0] = dip->createDipSubscription("dip/acc/SPS/Timing/Cycle/StartExtractionEvent", handler);
	    sub[0] = dip->createDipSubscription("dip/acc/EAST/XBZT9/DETECTORS/XBPF/T09.BXBPF041", handler);
	    sub[1] = dip->createDipSubscription("dip/acc/EAST/XBZT9/DETECTORS/XBPF/T09.BXBPF042", handler);
	    sub[2] = dip->createDipSubscription("dip/acc/EAST/XBZT9/DETECTORS/XBPF/T09.BXBPF050", handler);
	    sub[3] = dip->createDipSubscription("dip/acc/EAST/XBZT9/DETECTORS/XBPF/T09.BXBPF051", handler);

            //for(int i = 0 ; i < numberOfPubs ; i++){
            //    std::ostringstream oss;
            //    oss << "dipServerAddress" << "_" << i;
            //    sub[i] = dip->createDipSubscription(oss.str().c_str(), handler);
            //}
        }

        ~FiberTrackerClient(){
            // [0] ?
            dip->destroyDipSubscription(sub[0]);
            dip->destroyDipSubscription(sub[1]);
            dip->destroyDipSubscription(sub[2]);
            dip->destroyDipSubscription(sub[3]);
            delete handler;
            delete dip;
        }

};

//Mockup main loop running the client for a minute
int main(const int argc, const char ** argv){
	
    std::cout << "Starting DIP Client" << std::endl;
    //Listens for data until destroyed
    FiberTrackerClient* client = new FiberTrackerClient(argc, argv);

    //Sleep half a minute just so this program doesn't terminate
    sleep(120);

    delete client;

    return 0;
}
