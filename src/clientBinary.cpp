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
#include <string.h>
#include <bitset>
#include <fstream>

//(Un-)Packing code courtesy to Beej's guide to network programming

#define pack754_32(f) (pack754((f), 32, 8))
#define pack754_64(f) (pack754((f), 64, 11))
#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))

uint64_t pack754(long double f, unsigned bits, unsigned expbits)
{
    long double fnorm;
    int shift;
    long long sign, exp, significand;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (f == 0.0) return 0; // get this special case out of the way

    // check sign and begin normalization
    if (f < 0) { sign = 1; fnorm = -f; }
    else { sign = 0; fnorm = f; }

    // get the normalized form of f and track the exponent
    shift = 0;
    while(fnorm >= 2.0) { fnorm /= 2.0; shift++; }
    while(fnorm < 1.0) { fnorm *= 2.0; shift--; }
    fnorm = fnorm - 1.0;

    // calculate the binary form (non-float) of the significand data
    significand = fnorm * ((1LL<<significandbits) + 0.5f);

    // get the biased exponent
    exp = shift + ((1<<(expbits-1)) - 1); // shift + bias

    // return the final answer
    return (sign<<(bits-1)) | (exp<<(bits-expbits-1)) | significand;
}

long double unpack754(uint64_t i, unsigned bits, unsigned expbits)
{
    long double result;
    long long shift;
    unsigned bias;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (i == 0) return 0.0;

    // pull the significand
    result = (i&((1LL<<significandbits)-1)); // mask
    result /= (1LL<<significandbits); // convert back to float
    result += 1.0f; // add the one back on

    // deal with the exponent
    bias = (1<<(expbits-1)) - 1;
    shift = ((i>>significandbits)&((1LL<<expbits)-1)) - bias;
    while(shift > 0) { result *= 2.0; shift--; }
    while(shift < 0) { result /= 2.0; shift++; }

    // sign it
    result *= (i>>(bits-1))&1? -1.0: 1.0;

    return result;
}

void writeVector(std::ofstream& outfile, std::vector<uint32_t>& data){
    if(!outfile.write((char*)&data[0], sizeof(data[0]) * data.size())){
        std::cout << "ERROR: COULD NOT WRITE SPILL" << std::endl;
    }
}

void writeVectorVector(std::ofstream& outfile, std::vector<std::vector<uint32_t>> data){
    for(const auto& subvector: data){
        std::vector<uint32_t> v = {subvector.size()};
        v.insert(v.end(), subvector.begin(), subvector.end());
        //std::cout << v.at(0) << std::endl;
        writeVector(outfile, v);
    }
}

std::vector<std::vector<uint32_t>> readVectors(std::ifstream& infile){
    infile.seekg(0, infile.end);
    int length = infile.tellg();
    infile.seekg(0, infile.beg);
    std::vector<uint32_t> b;
    b.resize(length/sizeof(uint32_t));
    if(!infile.read((char*)&b[0], length)){
        std::cout << "COULD NOT READ" << std::endl;
    }

    std::vector<std::vector<uint32_t>> sub;
    std::vector<uint32_t>::const_iterator iter = b.cbegin();
    std::vector<uint32_t>::const_iterator end = b.cend();
    while(true){
        std::cout << "Length: " << (int)*iter << std::endl;
        std::vector<uint32_t> subvector;
        for(int i = 1; i <= (int)*iter; i++){ subvector.push_back(*(iter+i));}
        for(const auto& k: subvector){ std::cout << k << std::endl;}
        //copy(iter+1,iter+(int)*iter+1, subvector);
        std::advance(iter, *iter+1);
        sub.push_back(subvector);

        if(iter == end){
            break;
        }
    }

    return sub;
}

//Does not compile if included before Dip?

class FiberTrackerClient {
    private:
        DipSubscription **sub;
        DipFactory *dip;

        class FiberTrackerDataListener:public DipSubscriptionListener{
            private:
                //To access subscriptions
                FiberTrackerClient* client;
                std::string runNumber;
                std::string fileNameBase;
                std::string outputPath;

            public:
                FiberTrackerDataListener(FiberTrackerClient* c):client(c){}

                void setRunNumber(std::string s){runNumber = s;};
                void setFileNameBase(std::string s){fileNameBase = s;};
                void setOutputPath(std::string s){outputPath = s;};

                void handleMessage(DipSubscription *subscription, DipData &message){
                    std::cout<<"Received data from "<<subscription->getTopicName()<<std::endl;

                    //Serialized data to be written to disk
                    std::vector<std::vector<uint32_t>> data;

                    //Print all available fields
                    /*
                    int nTags;
                    const char ** tags = message.getTags(nTags); 
                    std::cout << "nTags = " << nTags << std::endl;
                    for(int i = 0; i < nTags; i++){
                    std::cout << tags[i] << " : " << message.getValueType(tags[i]) << std::endl;	    
                    }
                    */

                    /*
                    1. acqMode TODO
                    
                    */

                    const int acqMode = message.extractInt("acqMode");
                    std::vector<uint32_t> acqModeVector = {1, (uint32_t)acqMode};
                    data.push_back(acqModeVector);
 
                    /*
                    2. acqStamp TODO
                    
                    */

                    const long acqStamp = message.extractLong("acqStamp");
                    //std::cout << "acqStamp = " <<acqStamp << std::endl;
                    
                    /*
                    3. actType TODO
                    
                
                    */
                    const int actType = message.extractInt("acqType");
                    
                    /*
                    4. acqTypeAllowed TDOO
                
                    */
                    const int acqTypeAllowed = message.extractInt("acqTypeAllowed");
                    
                    /*
                    5. coincidenceInUse TODO
                
                    */
                    const char* coincidenceInUse = message.extractCString( "coincidenceInUse");
                    
                    /*
                    6. counts 
                
                    */
                    const long counts = message.extractLong("counts");
                    std::vector<uint32_t> countsVector = {6, (uint32_t)counts};
                    data.push_back(countsVector);

                    /*
                    7. countsRecords TODO
                
                    */
                    const long countsRecords = message.extractLong("countsRecords");
                    //std::cout << "countsRecords = " << countsRecords << std::endl;
                    
                    /*
                    8. countsRecordsWithZeroEvents TODO
                
                    */
                    const long countsRecordsWithZeroEvents = message.extractLong("countsRecordsWithZeroEvents");
                    //std::cout << "countsRecordsWithZeroEvents = " << countsRecordsWithZeroEvents << std::endl;

                    /*
                    9. countTrigs TODO
                
                    */
                    const long countTrigs = message.extractLong("countsTrigs");

                    /*
                    10. cycleName TODO
                
                    */
                    const char* cycleName = message.extractCString("cycleName");
                    //std::cout << cycleName << std::endl;

                    /*
                    11. cycleStamp TODO
                
                    */
                    const long cycleStamp = message.extractLong("cycleStamp");

                    /*
                    12. equipmentName TODO
                
                    */
                    const char* equipmentName = message.extractCString("equipmentName");

                    /*
                    13. eventSelectionAcq TODO
                
                    */
                    const int eventSelectionAcq = message.extractInt("eventSelectionAcq");

                    /*
                    14. eventsData 
                
                    */
                    int eventsDataSize;
                    const int* eventsData = message.extractIntArray(eventsDataSize, "eventsData");

                    std::vector<uint32_t> eventsDataVector;
                    eventsDataVector.push_back(14); 
                    for(int i = 0; i < eventsDataSize; i++){
                        if(!(i % 10 == 0 && eventsData[i] == 0)){
                            eventsDataVector.push_back((uint32_t)eventsData[i]);
                            //if(eventsDataVector.at(eventsDataVector.size()-1) != eventsData[i]){
                            //    std::cout << "MISMATCH: " << eventsDataVector.at(eventsDataVector.size()-1) << ":" << eventsData[i] << std::endl;
                            //}
                        }
                        else{
                            break;
                        }
                    }

                    data.push_back(eventsDataVector);

                    //Uncomment to see some data
                    /*
                    for(int i = 0; i < 5; i++){
                    std::cout << eventsData[i*10+0] << " " <<eventsData[i*10+1] << " " << eventsData[i*10+2] << " "<< eventsData[i*10+3] << " : "<< std::bitset<32>(eventsData[i*10+4]) << " " <<std::bitset<32>(eventsData[i*10+5]) << " " <<
                         std::bitset<32>(eventsData[i*10+6]) << " " << std::bitset<32>(eventsData[i*10+7]) << " " << std::bitset<32>(eventsData[i*10+8]) << " "<< std::bitset<32>(eventsData[i*10+9]) << std::endl; 
                    }
                    */

                    /*
                    15. mean 
                
                    */
                    const double mean = message.extractDouble("mean");
                    //std::cout << mean << std::endl;
                    uint64_t meanS = pack754_64(mean);
                    uint32_t meanSlsb = meanS & 0xffffffff;
                    uint32_t meanSmsb = meanS >> 32;
                    //Procedure to de-serialize it
                    //uint64_t meanSNew = ((uint64_t)meanSmsb << 32) | (uint64_t)meanSlsb;
                    //double meanNew = unpack754_64(meanSNew);
                    //std::cout << meanNew << std::endl;

                    std::vector<uint32_t> meanVector = {15, meanSlsb, meanSmsb};
                    data.push_back(meanVector);
 
                    /*
                    16. message TODO
                
                    */
                    const char* msg = message.extractCString("message");
                    //std::cout << msg << std::endl;

                    /*
                    17. profile
                
                    */
                    int profileSize;
                    const double* profile = message.extractDoubleArray(profileSize, "profile");

                    std::vector<uint32_t> profileVector;
                    profileVector.push_back(17);
                    for(int i = 0; i < profileSize; i++){
                        uint64_t s = pack754_64(profile[i]);
                        uint32_t slsb = s & 0xffffffff;
                        uint32_t smsb = s >> 32;
                        profileVector.push_back(slsb);
                        profileVector.push_back(smsb);
                    }

                    data.push_back(profileVector);

                    /*
                    18. profile1D  TODO
                
                    */
                    int profile1DSize;
                    const DipLong* profile1D = message.extractLongArray(profile1DSize, "profile1D");
                    //std::cout << size << std::endl;
                    //for(int i = 0; i < size; i++){
                    //	std::cout << profile1D[i] << ",";
                    //}

                    /*
                    19. profleSourceSettingAcq  TODO
                
                    */
                    const int profileSourceSettingAcq = message.extractInt("profileSourceSettingAcq");
                    int profileStandAloneSize;

                    /*
                    20. profileStandAlone  TODO
                
                    */
                    const double* profileStandAlone = message.extractDoubleArray(profileStandAloneSize, "profileStandAlone");

                    /*
                    21. timeFirstEvent  TODO
                
                    */
                    const char* timeFirstEvent = message.extractCString("timeFirstEvent");
                    //std::cout << timeFirstEvent << std::endl;
                    
                    /*
                    22. timeFirstTrigger  TODO
                
                    */
                    const char* timeFirstTrigger = message.extractCString("timeFirstTrigger");

                    /*
                    23. timeLastEvent  TODO
                
                    */
                    const char* timeLastEvent = message.extractCString("timeLastEvent");
                    
                    /*
                    24. timeLastTrigger  TODO
                
                    */
                    const char* timeLastTrigger = message.extractCString("timeLastTrigger");

                    /*
                    25. trigger  TODO
                
                    */
                    const int trigger = message.extractInt("trigger");
                    
                    /*
                    26. triggerOffsetAcq  TODO
                
                    */
                    const int triggerOffsetAcq = message.extractInt("triggerOffsetAcq");

                    /*
                    27. triggerSelectionAcq  TODO
                
                    */
                    const int triggerSelectionAcq = message.extractInt("triggerSelectionAcq");

                    //Save data
                    if(strcmp(equipmentName, "ZT9.BXBPF041") == 0){
                        std::string filename = outputPath+fileNameBase+"run_"+runNumber+"_41.bin";
                        std::cout << "Saving to " << filename << std::endl;
                        std::ofstream outfile;
                        outfile.open(filename, std::ios::binary | std::ios::out | std::ios::app);
                        writeVectorVector(outfile, data);
                        outfile.close();
                    }
                    if(strcmp(equipmentName, "ZT9.BXBPF042") == 0){
                        std::string filename = outputPath+fileNameBase+"run_"+runNumber+"_42.bin";
                        std::cout << "Saving to " << filename << std::endl;
                        std::ofstream outfile;
                        outfile.open(filename, std::ios::binary | std::ios::out | std::ios::app);
                        writeVectorVector(outfile, data);
                        outfile.close();
                    }
                    if(strcmp(equipmentName, "ZT9.BXBPF050") == 0){
                        std::string filename = outputPath+fileNameBase+"run_"+runNumber+"_50.bin";
                        std::cout << "Saving to " << filename << std::endl;
                        std::ofstream outfile;
                        outfile.open(filename, std::ios::binary | std::ios::out | std::ios::app);
                        writeVectorVector(outfile, data);
                        outfile.close();
                    }
                    if(strcmp(equipmentName, "ZT9.BXBPF051") == 0){
                        std::string filename = outputPath+fileNameBase+"run_"+runNumber+"_51.bin";
                        std::cout << "Saving to " << filename << std::endl;
                        std::ofstream outfile;
                        outfile.open(filename, std::ios::binary | std::ios::out | std::ios::app);
                        writeVectorVector(outfile, data);
                        outfile.close();
                    }

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

        FiberTrackerClient(std::string runNumber, std::string fileNameBase, std::string outputPath){
            //Subscribe to several publications (the 4 trackers eventually)
            int numberOfPubs = 4;
            dip = Dip::create("dip-client");
            handler = new FiberTrackerDataListener(this);
            handler->setRunNumber(runNumber);
            handler->setFileNameBase(fileNameBase);
            handler->setOutputPath(outputPath);
            sub = new DipSubscription*[numberOfPubs];
            dip->setDNSNode("dipnsgpn1,dipnsgpn2");
            sub[0] = dip->createDipSubscription("dip/acc/EAST/XBZT9/DETECTORS/XBPF/T09.BXBPF041", handler);
            sub[1] = dip->createDipSubscription("dip/acc/EAST/XBZT9/DETECTORS/XBPF/T09.BXBPF042", handler);
            sub[2] = dip->createDipSubscription("dip/acc/EAST/XBZT9/DETECTORS/XBPF/T09.BXBPF050", handler);
            sub[3] = dip->createDipSubscription("dip/acc/EAST/XBZT9/DETECTORS/XBPF/T09.BXBPF051", handler);
        }

        ~FiberTrackerClient(){
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
    if(argc != 4){
        std::cout << "ERROR: Invalid arguments. Should be: ./clientBinary runNumber fileNameBase outputPath" << std::endl;
    }

    std::string runNumber(argv[1]);
    std::string fileNameBase(argv[2]);
    std::string outputPath(argv[3]);

    std::cout <<  outputPath + fileNameBase + runNumber + "_41.bin" << std::endl;

    std::cout << "Starting DIP Client" << std::endl;
    //Listens for data until destroyed
    FiberTrackerClient* client = new FiberTrackerClient(runNumber, fileNameBase, outputPath);

    //Sleep this program doesn't terminate
    sleep(120);

    delete client;

    return 0;
}
