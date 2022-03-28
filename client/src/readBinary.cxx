
#include <fstream>
#include <iostream>
#include <vector>
#include "FiberTrackerUtils.h"


int main(int argc, char *argv[]){
    std::ifstream infile;
    infile.open(argv[1], std::ios::binary | std::ios::in);
    std::vector<std::vector<uint32_t>> result = readVectors(infile);

    infile.close();

    for(const auto& k: result){ 

        //std::cout << "Header: " << k[0] << std::endl;
        if( k[0] == 1 ){
            //std::cout << "acqMode" << std::endl;
            //std::cout << k[1] << std::endl;
        }
        else if( k[0] == 2 ){
            //std::cout << "acqStamp" << std::endl;
            uint64_t p = ((uint64_t)k.at(2) << 32) | (uint64_t)k.at(1);
            //std::cout << p << std::endl;
        }
        else if( k[0] == 3){
            //std::cout << "acqType" << std::endl;
            //std::cout << k[1] << std::endl;
        }
        else if( k[0] == 4){
            //std::cout << "acqTypeAllowed" << std::endl;
            //std::cout << k[1] << std::endl;
        }
        else if( k[0] == 5){
            //std::cout << "coincidenceInUse" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            //std::cout << s << std::endl;
        }
        else if( k[0] == 6 ){
            //std::cout << "counts" << std::endl;
            //std::cout << k[1] << std::endl;
        }
        else if( k[0] == 7 ){
            //std::cout << "countsRecords" << std::endl;
            long p = ((uint64_t)k.at(2) << 32) | (uint64_t)k.at(1);
            //std::cout << p << std::endl;
        }
        else if( k[0] == 8 ){
            //std::cout << "countsRecordsWithZeroEvents" << std::endl;
            long p = ((uint64_t)k.at(2) << 32) | (uint64_t)k.at(1);
            //std::cout << p << std::endl;
        }
        else if( k[0] == 9 ){
            //std::cout << "countsTrigs" << std::endl;
            long p = ((uint64_t)k.at(2) << 32) | (uint64_t)k.at(1);
            //std::cout << p << std::endl;
        }
        else if( k[0] == 10){
            //std::cout << "cycleName" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            //std::cout << s << std::endl;
        }
        else if( k[0] == 11 ){
            //std::cout << "cycleStamp" << std::endl;
            long p = ((uint64_t)k.at(2) << 32) | (uint64_t)k.at(1);
            //std::cout << p << std::endl;
        }
        else if( k[0] == 12){
            //std::cout << "equipmentName" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            //std::cout << s << std::endl;
        }
        else if( k[0] == 13){
            //std::cout << "eventSelectionAcq" << std::endl;
            //std::cout << k[1] << std::endl;
        }
        else if( k[0] == 14 ){
            //std::cout << "eventsData" << std::endl;

            std::vector<uint32_t> eventsData;
            for(int i = 1; i < k.size(); i++){
                eventsData.push_back(k.at(i));
            }
            
            /*
            for(int i = 0; i < 5; i++){
                std::cout << (long)eventsData[i*10+0] << " " << (long)eventsData[i*10+1] << " " << eventsData[i*10+2] << " "<< eventsData[i*10+3] << " : "<< std::bitset<32>(eventsData[i*10+4]) << " " <<std::bitset<32>(eventsData[i*10+5]) << " " <<
                 std::bitset<32>(eventsData[i*10+6]) << " " << std::bitset<32>(eventsData[i*10+7]) << " " << std::bitset<32>(eventsData[i*10+8]) << " "<< std::bitset<32>(eventsData[i*10+9]) << std::endl;
            }
            */
            for(int i = 0; i < eventsData.size()/10; i++){
                std::cout << eventsData[i*10+0] << " " << eventsData[i*10+1]  << " "<< std::bitset<32>((int)eventsData[i*10+4]) << std::bitset<32>((int)eventsData[i*10+5]) << 
                 std::bitset<32>((int)eventsData[i*10+6]) << std::bitset<32>((int)eventsData[i*10+7]) << std::bitset<32>((int)eventsData[i*10+8]) << std::bitset<32>((int)eventsData[i*10+9]) << std::endl;
            }
            //for(int i = 1; i < k.size(); i+=10){
            //    std::cout << k.at(i) << ","; 
            //}
            std::cout << std::endl;
            
            
        }
        else if( k[0] == 15 ){
            //std::cout << "mean" << std::endl;
            uint64_t meanSNew = ((uint64_t)k[2] << 32) | (uint64_t)k[1];
            double meanNew = unpack754_64(meanSNew);
            //std::cout << meanNew << std::endl;

        }
        else if( k[0] == 16){
            //std::cout << "message" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            //std::cout << s << std::endl;
        }
        else if( k[0] == 17){
            //std::cout << "profile" << std::endl;
            double profile[196*2];
            for(int i = 1; i < k.size(); i+=2){
                uint64_t p = ((uint64_t)k.at(i+1) << 32) | (uint64_t)k.at(i);
                profile[(i+1)/2-1] = unpack754_64(p);
                //std::cout << (double)unpack754_64(p) << ",";
            }
        }
        else if( k[0] == 20){
            //std::cout << "profileStandAlone" << std::endl;
            double profileStandAlone[196];
            for(int i = 1; i < k.size(); i+=2){
                uint64_t p = ((uint64_t)k.at(i+1) << 32) | (uint64_t)k.at(i);
                profileStandAlone[(i+1)/2-1] = unpack754_64(p);
                //std::cout << (double)unpack754_64(p) << ",";
            }
        }
        else if( k[0] == 21){
            //std::cout << "timeFirstEvent" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            //std::cout << s << std::endl;
        }
        else if( k[0] == 22){
            //std::cout << "timeFirstTrigger" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            //std::cout << s << std::endl;
        }
        else if( k[0] == 23){
            //std::cout << "timeLastEvent" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            //std::cout << s << std::endl;
        }
        else if( k[0] == 24){
            //std::cout << "timeLastTrigger" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            //std::cout << s << std::endl;
        }
        else if( k[0] == 25){
            //std::cout << "trigger" << std::endl;
            //std::cout << k[1] << std::endl;
        }
        else if( k[0] == 26){
            //std::cout << "triggerOffsetAcq" << std::endl;
            //std::cout << (int)k[1] << std::endl;
        }
        else if( k[0] == 27){
            //std::cout << "triggerSelectionAcq" << std::endl;
            //std::cout << k[1] << std::endl;
        }

    }

    return 0;
}
