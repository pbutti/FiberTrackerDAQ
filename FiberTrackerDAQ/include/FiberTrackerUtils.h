#ifndef FIBERTRACKERUTILS_H
#define FIBERTRACKERUTILS_H


#include <unistd.h>
#include <vector>
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
        //std::cout << "Length: " << (int)*iter << std::endl;
        std::vector<uint32_t> subvector;
        for(int i = 1; i <= (int)*iter; i++){ subvector.push_back(*(iter+i));}
        //for(const auto& k: subvector){ std::cout << k << std::endl;}
        //copy(iter+1,iter+(int)*iter+1, subvector);
        std::advance(iter, *iter+1);
        sub.push_back(subvector);

        if(iter == end){
            break;
        }
    }

    return sub;
}

#endif
