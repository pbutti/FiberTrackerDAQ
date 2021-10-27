#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "Dip.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "DIPVersion.h"

#define _GLIBCXX_USE_CXX11_ABI 0

using namespace std;

/**
* Simple demo server that publishes DIP data.
* */

class ErrHandler:public DipPublicationErrorHandler{
	void handleException(DipPublication* publication, DipException& ex){
		printf("Error because %s", ex.what());
		}
	};

int main(const int argc, const char** argv) {
   string pubNameRoot = "dip/test/api/dip-server";
   
   cout<<"Using "<<DipVersion::getDipVersion() <<endl;

	unsigned int noPubs = 5;
	unsigned int noFields = 4;
	unsigned int sizefields = 10;
	unsigned int updateRate = 1000; //(ms)
	unsigned int lifeTime = 60000; //(ms) So the Server by default runs 1 minute.

	string    someString = "test";
	DipBool	  someBool   = true;
	DipByte	  someByte   = 1;
	DipShort  someShort  = 1;
	DipInt	  someInt    = 1;
	DipLong	  someLong   = 1073741824l;
	DipFloat  someFloat  = 1.0f;
	DipDouble someDouble = 1.0f;

	string *  stringArray = new string[sizefields];
	DipBool *  boolArray = new DipBool[sizefields];
	DipByte *  byteArray = new DipByte[sizefields];
	DipShort * shortArray = new DipShort[sizefields];
	DipInt *   intArray = new DipInt[sizefields];
	DipLong *  longArray = new DipLong[sizefields];
	DipFloat * floatArray = new DipFloat[sizefields];
	DipDouble *doubleArray = new DipDouble[sizefields];

	for (unsigned int i = 0 ;i < sizefields ; i++) {
		stringArray[i] = '\0';
		boolArray[i] = true;
		byteArray[i] = 0;
		shortArray[i] = 0;
		intArray[i] = 0;
		longArray[i] = 0;
		floatArray[i] = 0.0f;
		doubleArray[i] = 0.0f;
		}

	DipPublication **pub = new DipPublication*[noPubs]();
	DipData **pubData   = new DipData*[noPubs];
	DipTimestamp aTimeStamp;

	//Creating the server.
	string serverName = "Server";
	DipFactory *dip = Dip::create(serverName.c_str());
    dip->setDNSNode("localhost");


	//The following call forces the selection for the DNS of your choice rather than the default (hardcoded in DIP),
	// or the one you have specified via the DIPNS environment variable.

	//Publication names
	ErrHandler errorHandler;
	for (unsigned int i=0; i < noPubs; i++){
		ostringstream oss;
		oss << pubNameRoot << "_" << i;
		pub[i] = dip->createDipPublication(oss.str().c_str(), &errorHandler);
		pubData[i] = dip->createDipData();
		cout<<serverName<<" has just created a publication to :"<<oss.str()<<endl;
		}

	//Publication fields
	string fieldName = "field_";
	vector<string> fieldNames;
	for (unsigned int i = 0; i < (noFields*16); i++) {
		ostringstream oss;
		oss << fieldName << i;
		fieldNames.push_back(oss.str());
		}

	//Just a flag for traces.
	bool flip=true;

	//Iterations = lifetime / updateRate.
	unsigned int iterations = lifeTime / updateRate;
	while(iterations--){
		try{
			for (unsigned int indexPub = 0; indexPub < noPubs; indexPub++) {		
				int i = 0;
				unsigned int indexField = 0;
				while(indexField < noFields) { //For each publication.
					pubData[indexPub]->insert(someString,fieldNames[i++].c_str());
					pubData[indexPub]->insert(someBool,fieldNames[i++].c_str());
					pubData[indexPub]->insert(someByte,fieldNames[i++].c_str());
					pubData[indexPub]->insert(someShort,fieldNames[i++].c_str());
					pubData[indexPub]->insert(someInt,fieldNames[i++].c_str());
					pubData[indexPub]->insert(someLong,fieldNames[i++].c_str());
					pubData[indexPub]->insert(someFloat,fieldNames[i++].c_str());
					pubData[indexPub]->insert(someDouble,fieldNames[i++].c_str());
					pubData[indexPub]->insert(stringArray, sizefields ,fieldNames[i++].c_str());
					pubData[indexPub]->insert(boolArray, sizefields ,fieldNames[i++].c_str());
					pubData[indexPub]->insert(byteArray, sizefields ,fieldNames[i++].c_str());
					pubData[indexPub]->insert(shortArray, sizefields ,fieldNames[i++].c_str());
					pubData[indexPub]->insert(intArray, sizefields ,fieldNames[i++].c_str());
					pubData[indexPub]->insert(longArray, sizefields ,fieldNames[i++].c_str());
					pubData[indexPub]->insert(floatArray, sizefields ,fieldNames[i++].c_str());
					pubData[indexPub]->insert(doubleArray, sizefields ,fieldNames[i++].c_str());
					indexField++;
					}	
				DipTimestamp time;
				if (flip==true ? cout<<"/": cout<<"\\") flip = !flip;
				cout<< "Posting data to pub "<<pub[indexPub]->getTopicName() << endl;
				pub[indexPub]->send(*pubData[indexPub], time);
				}

			//Update data...
			someBool == true ? someBool = false: someBool =true;	
			someByte == 10 ? someByte =0 : someByte++;
			someShort == 100 ? someShort = 0 : someShort++;
			someInt == 1000000 ? someInt = 0:someInt++;
			someLong == 1247483648l ? someLong = 0:someLong++;
			someFloat >= 1000000.0f ? someFloat = 0.0f : someFloat++;
			someDouble >= 1000000.0f ? someDouble = 0.0f : someDouble++;
			ostringstream oss;
			oss<<"Value <"<<someInt<<"> is of \"int\" type.";
			someString = oss.str();
			for (unsigned int i = 0 ;i < sizefields ; i++) {
				boolArray[i] == true ? boolArray[i] = false: boolArray[i] =true;
				byteArray[i] == 10 ? byteArray[i] =0 : byteArray[i]++;
				shortArray[i] == 100 ? shortArray[i] = 0 : shortArray[i]++;
				intArray[i] == 1000000 ? intArray[i] = 0 : intArray[i]++;
				longArray[i] == 1247483648l ? longArray[i] = 0 : longArray[i]++;
				floatArray[i] >= 1000000.0f ? floatArray[i] = 0.0f : floatArray[i]++;
				doubleArray[i] >= 1000000.0f ? doubleArray[i] = 0.0f : doubleArray[i]++;
				stringArray[i] = someString;
				}

#ifdef WIN32
			Sleep(updateRate);
#else
			sleep(updateRate/1000);
#endif
			} catch (DipException e){
				printf("Failed to send data\n");
			}

		}

	// shutdown
	cout<<"Server's lifetime has expired, leaving... "<<endl;
	for (unsigned int i=0; i < noPubs; i++){
		cout<<"Server is cleaning pending Publications."<<endl;
		dip->destroyDipPublication(pub[i]);
	}
	delete []pubData;
	cout<<"Server's done."<<endl;
	return 0;
}
