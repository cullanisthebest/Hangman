#include "socket.h"
#include "socketserver.h"
#include "thread.h"
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <ctime>
#include <pthread.h>
#include <stdio.h>
#include <cstring>
#include <vector>


using namespace std;
using namespace Communication;



class GameThread : public Thread
{

private:
    Socket mySocket1;
    Socket mySocket2;

public:
    string toString(std::vector<char> v){
        string result = "";
        for (int i = 0; i < v.size(); i++){
            result += v[i];
            result += " ";
        }
        return result;
    }

    void updateBoard(std::vector<char> *resultVector, std::vector<char> gameWordVector ,char userInput){
        for(int i=0; i<gameWordVector.size();i++){
            if(gameWordVector[i]==userInput){
                resultVector->at(i) = gameWordVector[i];
            }
        }
    }

    GameThread(Socket const  player1, Socket const  player2)
    : Thread(true), mySocket1(player1), mySocket2(player2)
    {
        ;
    }
    long ThreadMain(void){
        string words [] = {"APPLE","BANANA","PINEAPPLE","ORANGE","CARROT","CELERY","WATERMELON","PEACH","POMEGRANATE","DRAGONFRUIT","BAKCHOY"};

        srand(time(0));
        int gameWordIndex = rand() % 10 + 0; 
        cout << gameWordIndex <<endl;

        std::vector<char> gameWordVector;
        std::vector<char> resultVector;

        string gameWord = words[gameWordIndex];
        //cout << gameWord << "size:" << gameWord.size()  <<endl;


        for (int i = 0; i < gameWord.size(); i++){
            gameWordVector.push_back(gameWord.at(i)); 
            cout <<  gameWordVector[i];
        }
        cout << endl;
        for (int i = 0; i < gameWord.size(); i++){
            resultVector.push_back('_'); 
            cout <<  resultVector[i] << " ";
        } 
        cout << endl;

        string resultString = toString(resultVector);
        //cout<< resultString << endl;

        cout<<"Now in a game thread"<<endl;
        ByteArray bytes;
        string initialMessageP1 = "******Entering Game As Player 1. Your Turn******\n the game board is:" + resultString + "\n";

        mySocket1.Write(ByteArray(initialMessageP1));

        string initialMessageP2 = "******Entering Game As Player 2. Player 1's Turn******\n the game board is:" + resultString + "\n";
        mySocket2.Write(ByteArray(initialMessageP2));

        bool win = false;

        while (win != true){
            if(toString(resultVector)!=toString(gameWordVector)){
                cout<<"Player 1's turn" <<endl;

                mySocket1.Read(bytes);
                char letter = (bytes.ToString())[0];
                updateBoard(&resultVector, gameWordVector, letter);
                
                resultString = toString(resultVector);
                
/*
                if (toString(resultVector)==toString(gameWordVector)){
                    
                }*/
                string pOneInput = "Player 1 Entered: " + bytes.ToString() + "\n *****Your turn*****\n the game board is:" + resultString + "\n";
               
                mySocket2.Write(ByteArray(pOneInput));
                string currentGameBoard = "the game board is: " + resultString;
                mySocket1.Write(ByteArray(currentGameBoard));
            }

            if(toString(resultVector)==toString(gameWordVector)){
                mySocket1.Write(ByteArray("Player 1 wins! Game is now over"));
                mySocket2.Write(ByteArray("Player 1 wins! Game is now over"));
                mySocket1.Close();
                mySocket2.Close();
                win = true;
                break;
            }

            if(toString(resultVector)!=toString(gameWordVector)){
                cout<<"Player 2's turn" <<endl;

                mySocket2.Read(bytes);
                char letterP2 = (bytes.ToString())[0];
                updateBoard(&resultVector, gameWordVector, letterP2);
                resultString = toString(resultVector);
                string pTwoInput = "Player 2 Entered: " + bytes.ToString() + "\n *****Your turn*****\n the game board is:" + resultString + "\n";
                mySocket1.Write(ByteArray(pTwoInput));
                string currentGameBoard = "the game board is: " + resultString;
                mySocket2.Write(ByteArray(currentGameBoard));
            }

            if(toString(resultVector)==toString(gameWordVector)){
                mySocket1.Write(ByteArray("Player 2 wins! Game is now over"));
                mySocket2.Write(ByteArray("Player 2 wins! Game is now over"));
                mySocket1.Close();
                mySocket2.Close();
                win = true;
                break;
            }

        }
        cout << "game over" << endl;

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
