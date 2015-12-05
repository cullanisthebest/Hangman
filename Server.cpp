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
    Socket mySocket2;
 	//vector <ClientThread * > v;
    string message;
public:
    GameThread(Socket const  player1, Socket const  player2)
    : Thread(true), mySocket1(player1), mySocket2(player2)
    {
        ;
    }
    long ThreadMain(void){

        //while (1)
        cout<<"Now in a game thread"<<endl;
        ByteArray bytes;
        mySocket1.Write(ByteArray("******Entering Game As Player 1. Your Turn******\n"));
        mySocket2.Write(ByteArray("******Entering Game As Player 2. Player 1 Turn******\n"));


        while (true){
            mySocket1.Write(ByteArray("******Your turn******\n"));

            cout<<"Player 1's turn" <<endl;

            mySocket1.Read(bytes);

            mySocket1.Write(ByteArray("Player 1 ENTERED : "));
            mySocket1.Write(bytes);
            mySocket1.Write(ByteArray("\n"));
            mySocket2.Write(ByteArray("******Your turn******\n"));

            cout<<"Player 2's turn" <<endl;

            mySocket2.Read(bytes);
            mySocket1.Write(ByteArray("Player 2 ENTERED : "));
            mySocket1.Write(bytes);
            mySocket1.Write(ByteArray("\n"));


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
         FlexWait waiter(2, &mySocketServer, &cinWatcher);
         Blockable * result = waiter.Wait();
         if (result == &cinWatcher){
            cout<< "I AM SERVER, type exit to close me" <<endl;
            string input;
            cin >> input;
            if (input == "exit"){
                cout << "exiting!!" << endl;
                for (int i = 0; i < vThreads.size(); i++){
                    delete vThreads[i];
                }
                break;
            }
            else{
                continue;
            }
        }
        else if (result == &mySocketServer) {
            Socket mySocket1 = mySocketServer.Accept();
            mySocket1.Write(ByteArray("Player 1 Accepted!\n"));
            Socket mySocket2 = mySocketServer.Accept();
            mySocket2.Write(ByteArray("Player 2 Accepted!\n"));
            vThreads.push_back(new GameThread(mySocket1,mySocket2));

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
cout << "server is going to bed" << endl;
sleep(1);
return 0;
}
