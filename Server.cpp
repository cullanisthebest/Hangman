#include "socket.h"
#include "socketserver.h"
#include "thread.h"
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <pthread.h>

using namespace std;
using namespace Communication;



class GameThread : public Thread
{

private:
    Socket mySocket1;
 //.	Socket mySocket2;
 	//vector <ClientThread * > v;
    string message;
public:
    GameThread(Socket const  player1)
    : Thread(true), mySocket1(player1)
    {
        ;
    }
    long ThreadMain(void){

        //while (1)
        cout<<"Now in a game thread"<<endl;
        ByteArray bytes;
        mySocket1.Write(ByteArray("******I am game 1******\n"));


        while (true){
            mySocket1.Write(ByteArray("******Your turn******\n"));
            cout<<"game 1 is waiting to read your message...." <<endl;
            mySocket1.Read(bytes);
            cout <<"your message is: "<< bytes.ToString() << endl;
        }
        cout << "game 1 died" << endl;

    }
};




int main(int argc, char **argv) {

    cout << "Initial server begins"  << endl;
    SocketServer mySocketServer(2000);
    vector<GameThread *> vThreads;
   // Socket mySocket1(0);
    for(;;){
       try{
           cout<< "Before Flexwait" <<endl;

           FlexWait waiter(2, &mySocketServer, &cinWatcher);
           cout << "chilling before the wait" <<endl;
           Blockable * result = waiter.Wait();
           cout<<"chilling after the wait" <<endl;
           if (result == &cinWatcher){
            cout<< "I AM SERVER, type exit to close me" <<endl;
            string input;
            cin >> input;
            if (input == "exit"){
                cout << "exiting!!" << endl;
                for (int i = 0; i < vThreads.size(); i++){
                    delete vThreads[i];
                }
                cout<< "Server shall break" <<endl;
                break;
            }
            else{
                cout<<"Server shall continue" <<endl;
                continue;
            }
        }
        else if (result == &mySocketServer) {
            cout <<"Received connection on this socketserver and accepting!" << endl;
           Socket mySocket1 = mySocketServer.Accept();

            mySocket1.Write(ByteArray("I am now creating a game thread!\n"));

        //  Socket mySocket2 = mySocketServer.Accept();*/
            vThreads.push_back(new GameThread(mySocket1));

            cout << "making game " << endl;
            continue;
            //break;
        }



    }
    catch(TerminationException err){
      cout << err << endl;
  }
  catch(string err){
      cout << err << endl;
  }
}
cout << "server is oging to bed" << endl;
sleep(1);
return 0;
}
