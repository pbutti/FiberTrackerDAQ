#ifndef FIBERTRACKERDATALISTENER_H
#define FIBERTRACKERDATALISTENER_H

#include <string>
#include "Dip.h"
#include "DipSubscription.h"

class FiberTrackerClient;

class FiberTrackerDataListener:public DipSubscriptionListener{
 private:
  //To access subscriptions
  FiberTrackerClient* client;
  std::string runNumber;
  std::string fileNameBase;
  std::string outputPath;
  
 public:
  
 FiberTrackerDataListener(FiberTrackerClient* c):client(c){};
  
  void setRunNumber(std::string s){runNumber = s;};
  void setFileNameBase(std::string s){fileNameBase = s;};
  void setOutputPath(std::string s){outputPath = s;};

  void handleMessage(DipSubscription *subscription, DipData &message);

  void connected(DipSubscription *arg0) {
    std::cout << "\nPublication source  " << arg0->getTopicName()<< " available\n";
  }
  
  void disconnected(DipSubscription *arg0, char *arg1){}
  
  void handleException(DipSubscription* subscription, DipException& ex){
    printf("Subs %s has error %s\n", subscription->getTopicName(), ex.what());
  }
 
};


#endif
