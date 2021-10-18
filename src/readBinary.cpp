#include <fstream>
#include <iostream>
#include <vector>
#include <bitset>

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
        std::cout << "DIP: COULD NOT WRITE SPILL" << std::endl;
    }
}

void writeVectorVector(std::ofstream& outfile, std::vector<std::vector<uint32_t>> data){
    for(const auto& subvector: data){
        std::vector<uint32_t> v = {subvector.size()};
        v.insert(v.end(), subvector.begin(), subvector.end());
        std::cout << v.at(0) << std::endl;
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
        //std::cout << "Length: " << (int)*iter << std::endl;
        std::vector<uint32_t> subvector;
        for(int i = 1; i <= (int)*iter; i++){ subvector.push_back(*(iter+i));}
        //for(const auto& k: subvector){ std::cout << std::bitset<32>(k) << std::endl;} 
        //copy(iter+1,iter+(int)*iter+1, subvector);
        std::advance(iter, *iter+1);
        sub.push_back(subvector);
        
        if(iter == end){
            break;
        }
    }

    return sub;
}

int main(){
    std::ifstream infile;
    infile.open("data/XBPF_test_run_1_42.bin", std::ios::binary | std::ios::in);
    std::vector<std::vector<uint32_t>> result = readVectors(infile);

    infile.close();

    for(const auto& k: result){ 

        std::cout << "Header: " << k[0] << std::endl;
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
            std::cout << "coincidenceInUse" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            std::cout << s << std::endl;
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
            std::cout << "cycleName" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            std::cout << s << std::endl;
        }
        else if( k[0] == 11 ){
            std::cout << "cycleStamp" << std::endl;
            long p = ((uint64_t)k.at(2) << 32) | (uint64_t)k.at(1);
            std::cout << p << std::endl;
        }
        else if( k[0] == 12){
            std::cout << "equipmentName" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            std::cout << s << std::endl;
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
            for(int i = 0; i < 5; i++){
                std::cout << eventsData[i*10+0] << " " << eventsData[i*10+1]  << " "<< std::bitset<32>((int)eventsData[i*10+4]) << std::bitset<32>((int)eventsData[i*10+5]) << 
                 std::bitset<32>((int)eventsData[i*10+6]) << std::bitset<32>((int)eventsData[i*10+7]) << std::bitset<32>((int)eventsData[i*10+8]) << std::bitset<32>((int)eventsData[i*10+9]) << std::endl;
            }
            for(int i = 1; i < k.size(); i+=10){
                std::cout << k.at(i) << ","; 
            }
            std::cout << std::endl;
            
            
        }
        else if( k[0] == 15 ){
            //std::cout << "mean" << std::endl;
            uint64_t meanSNew = ((uint64_t)k[2] << 32) | (uint64_t)k[1];
            double meanNew = unpack754_64(meanSNew);
            //std::cout << meanNew << std::endl;

        }
        else if( k[0] == 16){
            std::cout << "message" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            std::cout << s << std::endl;
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
                std::cout << (double)unpack754_64(p) << ",";
            }
        }
        else if( k[0] == 21){
            std::cout << "timeFirstEvent" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            std::cout << s << std::endl;
        }
        else if( k[0] == 22){
            std::cout << "timeFirstTrigger" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            std::cout << s << std::endl;
        }
        else if( k[0] == 23){
            std::cout << "timeLastEvent" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            std::cout << s << std::endl;
        }
        else if( k[0] == 24){
            std::cout << "timeLastTrigger" << std::endl;
            char s[k.size()];
            for(int i = 1; i < k.size(); i++){
               s[i-1] = (char)k.at(i); 
            }
            s[k.size()-1] = 0;
            std::cout << s << std::endl;
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
