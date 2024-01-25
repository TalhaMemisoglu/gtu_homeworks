#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include "pieces.h"
using namespace std;

namespace TalhaChess
{
    class Board
    {
    public:
        Board();
        bool move(char fromChr, int fromDgt, char toChr, int toDgt);
        bool isItCheck() const;
        bool isItMate() const;
        double score4W() const;//get score for white.
        double score4B() const;//get score for black.
        friend ostream& operator<<(ostream& output, const Board& obj);//print board
        friend istream& operator>>(istream& input, Board& obj);//input from user as english notation
    private:
        vector<vector<Pieces>> chessBoard;
        char color;
        char** getBoard() const;
        void deleteBoard(char** board) const;
        bool isPieceGoingToThreatened(char fromChr, int fromDgt, char toChr, int toDgt) const;
        bool isPieceBeingThreatened(char currChr, int currDgt) const;
        bool isGameFinished4King(char fromChr, int fromDgt, char toChr, int toDgt) const;
        bool giveSuggest(char& fromChr, int& fromDgt, char& toChr, int& toDgt) const;
    };
}

#endif //BOARD_H