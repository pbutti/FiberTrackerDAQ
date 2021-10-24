#include "FiberTrackerClient.h"
#include <string>
#include <iostream>
#include <unistd.h>

int main(const int argc, const char ** argv){
    if(argc != 4){
        std::cout << "ERROR: Invalid arguments. Should be: ./clientBinary runNumber fileNameBase outputPath" << std::endl;
	return 1;
    }

    std::string runNumber(argv[1]);
    std::string fileNameBase(argv[2]);
    std::string outputPath(argv[3]);

    std::cout <<  outputPath + fileNameBase + runNumber + "_41.bin" << std::endl;

    std::cout << "Starting DIP Client" << std::endl;
    //Listens for data until destroyed
    FiberTrackerClient* client = new FiberTrackerClient(runNumber, fileNameBase, outputPath);

    client->Subscribe();

    //Sleep this program doesn't terminate
    while(true){
        sleep(1);
    }

    client->Unsubscribe();

    delete client;

    return 0;
}
