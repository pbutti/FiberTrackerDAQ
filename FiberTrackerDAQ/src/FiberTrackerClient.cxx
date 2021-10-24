#include "FiberTrackerClient.h"
#include "FiberTrackerDataListener.h"

void FiberTrackerClient::Subscribe(){
  sub[0] = dip->createDipSubscription("dip/acc/EAST/XBZT9/DETECTORS/XBPF/T09.BXBPF041", handler);
  sub[1] = dip->createDipSubscription("dip/acc/EAST/XBZT9/DETECTORS/XBPF/T09.BXBPF042", handler);
  sub[2] = dip->createDipSubscription("dip/acc/EAST/XBZT9/DETECTORS/XBPF/T09.BXBPF050", handler);
  sub[3] = dip->createDipSubscription("dip/acc/EAST/XBZT9/DETECTORS/XBPF/T09.BXBPF051", handler);
}

void FiberTrackerClient::Unsubscribe(){
  dip->destroyDipSubscription(sub[0]);
  dip->destroyDipSubscription(sub[1]);
  dip->destroyDipSubscription(sub[2]);
  dip->destroyDipSubscription(sub[3]);
}

void FiberTrackerClient::setRunNumber(std::string runNumber){
	handler->setRunNumber(runNumber);
}

void FiberTrackerClient::setFileNameBase(std::string fileNameBase){
	handler->setFileNameBase(fileNameBase);
}

void FiberTrackerClient::setOutputPath(std::string outputPath){
	handler->setOutputPath(outputPath);
}

FiberTrackerClient::FiberTrackerClient(std::string runNumber, std::string fileNameBase, std::string outputPath){
  //Subscribe to several publications (the 4 trackers eventually)
  int numberOfPubs = 4;
  dip = Dip::create("dip-client");
  handler = new FiberTrackerDataListener(this);
  handler->setRunNumber(runNumber);
  handler->setFileNameBase(fileNameBase);
  handler->setOutputPath(outputPath);
  sub = new DipSubscription*[numberOfPubs];
  dip->setDNSNode("dipnsgpn1,dipnsgpn2");
}

FiberTrackerClient::~FiberTrackerClient(){
  delete handler;
  delete dip;
}
