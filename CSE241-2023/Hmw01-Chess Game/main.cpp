#include <iostream>
#include "board.h"
using namespace std;
using TalhaChess::Board;

int main()
{
    Board myBoard;
    cout << "Welcome to the Chess Game!" << endl;
    cout << myBoard << endl;

    while(1)
    {
        cout << "Please Enter your move:";
        cin >> myBoard;
        cout << myBoard << endl;
        myBoard.isItCheck();
        if(myBoard.isItMate())
        {
            cout << "Check Mate\nGame finished"<< endl;
            cout << "Score for White:" << myBoard.score4W() << endl;
            cout << "Score for Black:" << myBoard.score4B() << endl;
            break;
        } 
    }

    return 0;
}