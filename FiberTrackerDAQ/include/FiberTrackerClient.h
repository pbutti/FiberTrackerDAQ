#ifndef FIBERTRACKERCLIENT_H
#define FIBERTRACKERCLIENT_H

#include "Dip.h"
#include "DipSubscription.h"
#include <string>

class FiberTrackerDataListener;

class FiberTrackerClient {
 private:
  DipSubscription **sub;
  DipFactory *dip;
  
  FiberTrackerDataListener* handler;

public:
  void Subscribe();
  void Unsubscribe();
  FiberTrackerClient(std::string runNumber, std::string fileNameBase, std::string outputPath);
  ~FiberTrackerClient();

  void setRunNumber(std::string runNumber);
  void setFileNameBase(std::string fileNameBase);
  void setOutputPath(std::string outputPath);
  
};

#endif
