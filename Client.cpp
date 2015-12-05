#include <iostream>
#include <time.h>
#include <stdlib.h>
//#include "shared.h"
#include "socket.h"
#include "socketserver.h"


using namespace Communication;
using namespace std;

int main(void) {
	try{
		Socket mySocket("127.0.0.1", 2000);
		mySocket.Open();
		string input = "";

		while(input != "exit"){
			cout<< "Enter: " <<endl;
		/*	FlexWait waiter(2, &cinWatcher, &mySocket);
			Blockable * result = waiter.Wait();
			if (result == &mySocket){
				ByteArray tmp;
				if (mySocket.Read(tmp) <= 0){
					cout << "disconnection" << endl;
					cout << "socket closed unexpectedly" << endl;
					break;
				}
				else{
					input = tmp.ToString();
					cout << "Received Top: " << input  << endl;
					continue;
				}
			}
			else{*/
			cin >> input;
			ByteArray myByteArray(input);
			int written = mySocket.Write(myByteArray);
			if (written != myByteArray.v.size()){
				cout << "Wrote: " << written << endl;
				cout << "socket closed unexpectedly" << endl;
				break;

			}
			else{
				cout << "Sent: " << input << endl;
			}

			if (mySocket.Read(myByteArray) <=0){
				cout << "socket closed unexpectedly" << endl;
				break;
			}

			//mySocket.Read(myByteArray);
			std::cout << "Received : " << myByteArray.ToString() << endl;
			//}
		}
		cout << "Sleep" << endl;
		mySocket.Close();
		sleep(1);
	}
	catch(string s){
		cout << "error" << s << endl;
	}
	catch(...){
		cout << "unexpected exception" << endl;
	}


}
