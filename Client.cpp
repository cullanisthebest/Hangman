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
		int counter = 0;
		ByteArray myByteArray;

		while(input != "exit"){
			if (counter == 0){
				//cout<< "Enter yes to play game: " <<endl;
				mySocket.Read(myByteArray);
				std::cout << myByteArray.ToString() << endl;
				mySocket.Read(myByteArray);
				std::cout << myByteArray.ToString() << endl;
				counter++;

				if (myByteArray.ToString() == "******Entering Game As Player 2. Player 1 Turn******\n"){
					mySocket.Read(myByteArray);
					std::cout << myByteArray.ToString() << endl;
					//mySocket.Read(myByteArray);
					//std::cout << myByteArray.ToString() << endl;
				}

			}


			cin >> input;
			myByteArray = ByteArray(input);
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
			std::cout << myByteArray.ToString() << endl;
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
