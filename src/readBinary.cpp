#include <fstream>
#include <iostream>
#include <vector>
#include <bitset>

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
        std::cout << "Length: " << (int)*iter << std::endl;
        std::vector<uint32_t> subvector;
        for(int i = 1; i <= (int)*iter; i++){ subvector.push_back(*(iter+i));}
        for(const auto& k: subvector){ std::cout << std::bitset<32>(k) << std::endl;} 
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
    infile.open("out41.dat", std::ios::binary | std::ios::in);
    std::vector<std::vector<uint32_t>> result = readVectors(infile);

    infile.close();

    return 0;
}
