
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "Dip.h"
#include "DipSubscription.h"
#include "DIPVersion.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>


using namespace std;

//How long the test will run in ms. Default is 1 minute.
unsigned int lifeTime;


/**
* Simple demo client that subscribes to data coming from servers.
* Subscribe to "dip/test/api/dip-server_<from 0 to noPubs>"
*/

class Client {

private:
	// hold reference to subscription objects
	DipSubscription **sub;

	// DIP object
	DipFactory *dip;

/**
* handler for connect/disconnect/data reception events
* Nested class
* */
class GeneralDataListener:public DipSubscriptionListener{
private:

	// allow us to access subscription objects
	Client * client;

//	bool flip;

public:
	GeneralDataListener(Client *c):client(c){};

	/**
	* handle changes to subscribed to publications
	* Simply prints the contents of the received data.
	* @param subscription - the subsciption to the publications thats changed.
	* @param message - object containing publication data
	* */
	void handleMessage(DipSubscription *subscription, DipData &message){

		cout<<"Received data from "<<subscription->getTopicName()<<endl;
		cout<<"value :"<<message.extractDouble("field_7")<<endl;
		}


	/**
	* called when a publication subscribed to is available.
	* @param arg0 - the subsctiption who's publication is available.
	* */
	void connected(DipSubscription *arg0) {
		cout << "\nPublication source  " << arg0->getTopicName()<< " available\n";
	}


	/**
	* called when a publication subscribed to is unavailable.
	* @param arg0 - the subsctiption who's publication is unavailable.
	* @param arg1 - string providing more information about why the publication is unavailable.
	* */
	void disconnected(DipSubscription *arg0, char *arg1) {
		//printf("\nPublication source %s unavailable\n", arg0->getTopicName());
		}

	void handleException(DipSubscription* subscription, DipException& ex){
		//printf("Subs %s has error %s\n", subscription->getTopicName(), ex.what());
		}

	};

	//A handle on the DIP Data recipient.
	GeneralDataListener *handler;

public:

	/**
	* set up the subscriptions to dip/test/pub1 dip/test/pub2
	* */
	Client(const int argc, const char ** argv)
	{
		  lifeTime = 60000; //Client will die after 1 minute.

		  int numberOfPubs = 5;
          dip = Dip::create("dip-client");
		  handler = new GeneralDataListener(this);
		  //Creating an array of DipSubscriptions.
		  sub = new DipSubscription*[numberOfPubs];
		  dip->setDNSNode("localhost");
         
		  
		  for(int i = 0 ; i < numberOfPubs ; i++){
		      ostringstream oss;
		       oss << "dip/test/api/dip-server" << "_" << i;
		       sub[i] = dip->createDipSubscription(oss.str().c_str(), handler);
		  }
				  
			
	 }

	  ~Client(){
		  dip->destroyDipSubscription(sub[0]);
	  delete handler;
	  delete dip;
	  cout<<"Client's done."<<endl;

	}

	};


	/**
	* Start the client
	* */
	int main(const int argc, const char ** argv){
	    
        cout<<" Using "<<DipVersion::getDipVersion() <<endl;
        
		Client * theClient = new Client(argc,argv);

#ifdef WIN32
			Sleep(lifeTime);
#else
			sleep(lifeTime/1000);
#endif
		cout<<"Client's lifetime has expired, leaving... "<<endl;

		delete theClient;
		return(0);
		}

