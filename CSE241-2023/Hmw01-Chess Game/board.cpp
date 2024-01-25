#include <iostream>
#include <vector>
#include <string.h>
#include <cstdlib>
#include "board.h"
#include "pieces.h"
using namespace std;

namespace
{
    bool isUpper(char a) 
    {
        //if type of piece is upper, it is white
        bool status = false;

        if(a >= 65 && a <= 90)
        {
            status = true;
        }

        return status; 
    }

    bool isLower(char a)
    {
        //if type of piece is lower, it is black.
        bool status = false;

        if(a >= 97 && a <= 122)
        {
            status = true;
        }

        return status; 
    }

    bool isLegal4Pawn(char** board, char fromChr, int fromDgt, char toChr, int toDgt)
    {
       bool legality = false;
       
       //to convert appropriate index to use in vector of chessBoard.
       int fromChrIndex = fromChr - 97;
       int fromDgtIndex = fromDgt - 1;
       int toChrIndex = toChr - 97;//'a'
       int toDgtIndex = toDgt - 1;
       char Type = board[fromChrIndex][fromDgtIndex];

       if(Type == 'P')
        {
            if(board[toChrIndex][toDgtIndex] == '.')
            {
                if(toChr == fromChr)
                {
                    if(toDgt = fromDgt + 2 && fromDgt == 2)
                    {
                        legality = true;
                    }
                    else if(toDgt == fromDgt + 1)
                    {
                        legality = true;
                    }
                }
            }
            else if(toChr == fromChr + 1 && toDgt == fromDgt + 1)
            {
                if(isLower(board[toChrIndex][toDgtIndex]))
                    legality = true;
            }
            else if(toChr == fromChr - 1 && toDgt == fromDgt + 1)
            {
                if(isLower(board[toChrIndex][toDgtIndex]))
                    legality = true;
            }
        }
        else if(Type == 'p')
        {
            if(board[toChrIndex][toDgtIndex] == '.')
            {
                if(toChr == fromChr)
                {
                    if(toDgt == fromDgt - 2 && fromDgt == 7)
                    {
                        legality = true;
                    }
                    else if(toDgt == fromDgt - 1)
                    {
                        legality = true;
                    }
                }
            }
            else if(toChr == fromChr - 1 && toDgt == fromDgt - 1)
            {
                if(isUpper(board[toChrIndex][toDgtIndex]))
                    legality = true;
            }
            else if(toChr == fromChr + 1 && toDgt == fromDgt - 1)
            {
                if(isUpper(board[toChrIndex][toDgtIndex]))
                    legality = true;
            }
        }

        return legality;
    }

    bool isLegal4Rook(char** board, char fromChr, int fromDgt, char toChr, int toDgt)
    {
        bool legality = false;
        
        //to convert appropriate index to use in vector of chessBoard.
        int fromChrIndex = fromChr - 97;
        int fromDgtIndex = fromDgt - 1;
        int toChrIndex = toChr - 97;
        int toDgtIndex = toDgt - 1;
        char Type = board[fromChrIndex][fromDgtIndex];

        if(toDgtIndex > fromDgtIndex)
        {
            fromDgtIndex += 1;
        }
        else if(toDgtIndex < fromDgtIndex)
        {
            fromDgtIndex -= 1;
        }
        else if(toChrIndex > fromChrIndex)
        {
            fromChrIndex += 1;
        }
        else if(toChrIndex < fromChrIndex)
        {
            fromChrIndex -= 1;
        }
        while(true)
        {
            if(fromChrIndex == toChrIndex && fromDgtIndex == toDgtIndex)
            {
                if( isUpper(Type) && isLower(board[toChrIndex][toDgtIndex]) || isLower(Type) && isUpper(board[toChrIndex][toDgtIndex]))
                {
                    legality = true;
                    break;
                }
                else if(board[toChrIndex][toDgtIndex] == '.')
                {
                    legality = true;
                    break;
                }
                else
                {
                    break;
                }
            }
            else if(board[fromChrIndex][fromDgtIndex] == '.')
            {
                if(toDgtIndex > fromDgtIndex)
                {
                    fromDgtIndex += 1;
                }
                else if(toDgtIndex < fromDgtIndex)
                {
                    fromDgtIndex -= 1;
                }
                else if(toChrIndex > fromChrIndex)
                {
                    fromChrIndex += 1;
                }
                else if(toChrIndex < fromChrIndex)
                {
                    fromChrIndex -= 1;
                }
            }
            else
            {
                break;
            }
        }

        return legality;
    }

    bool isLegal4Knight(char** board, char fromChr, int fromDgt, char toChr, int toDgt)
    {
        bool legality = false;
        
        //to convert appropriate index to use in vector of chessBoard.
        int fromChrIndex = fromChr - 97;
        int fromDgtIndex = fromDgt - 1;
        int toChrIndex = toChr - 97;
        int toDgtIndex = toDgt - 1;
        char fromType = board[fromChrIndex][fromDgtIndex];
        char toType = board[toChrIndex][toDgtIndex];

        if(board[toChrIndex][toDgtIndex] == '.')
        {
            legality = true;
        }
        else if(isUpper(fromType) && isLower(toType) || isLower(fromType) && isUpper(toType))
        {
            legality = true;
        }

        return legality;
    }

    bool isLegal4Bishop(char** board, char fromChr, int fromDgt, char toChr, int toDgt)
    {
        bool legality = false;
        
        //to convert appropriate index to use in vector of chessBoard.
        int fromChrIndex = fromChr - 97;
        int fromDgtIndex = fromDgt - 1;
        int toChrIndex = toChr - 97;
        int toDgtIndex = toDgt - 1;
        char Type = board[fromChrIndex][fromDgtIndex];

        if(toDgtIndex > fromDgtIndex)
        {
            if(toChrIndex > fromChrIndex)
            {
                fromChrIndex += 1;
                fromDgtIndex += 1;
            }
            else if(toChrIndex < fromChrIndex)
            {
                fromChrIndex -= 1;
                fromDgtIndex += 1;
            }
        }
        else if(toDgtIndex < fromDgtIndex)
        {
            if(toChrIndex > fromChrIndex)
            {
                fromChrIndex += 1;
                fromDgtIndex -= 1;
            }
            else if(toChrIndex < fromChrIndex)
            {
                fromChrIndex -= 1;
                fromDgtIndex -= 1;
            }
        }
        while(true)
        {
            if(fromChrIndex == toChrIndex && fromDgtIndex == toDgtIndex)
            {
                if( isUpper(Type) && isLower(board[toChrIndex][toDgtIndex]) || isLower(Type) && isUpper(board[toChrIndex][toDgtIndex]))
                {
                    legality = true;
                    break;
                }
                else if(board[toChrIndex][toDgtIndex] == '.')
                {
                    legality = true;
                    break;
                }
                else
                {
                    break;
                }
            }
            else if(board[fromChrIndex][fromDgtIndex] == '.')
            {
                if(toDgtIndex > fromDgtIndex)
                {
                    if(toChrIndex > fromChrIndex)
                    {
                        fromChrIndex += 1;
                        fromDgtIndex += 1;
                    }
                    else if(toChrIndex < fromChrIndex)
                    {
                        fromChrIndex -= 1;
                        fromDgtIndex += 1;
                    }
                }
                else if(toDgtIndex < fromDgtIndex)
                {
                    if(toChrIndex > fromChrIndex)
                    {
                        fromChrIndex += 1;
                        fromDgtIndex -= 1;
                    }
                    else if(toChrIndex < fromChrIndex)
                    {
                        fromChrIndex -= 1;
                        fromDgtIndex -= 1;
                    }
                }
            }
            else
            {
                break;
            }
        }

        return legality;
    }

    bool isLegal4Queen(char** board, char fromChr, int fromDgt, char toChr, int toDgt)
    {
        bool legality = false;

        if(isLegal4Rook(board, fromChr, fromDgt, toChr, toDgt) || 
            isLegal4Bishop(board, fromChr, fromDgt, toChr, toDgt))
        {
            legality = true;
        }

        return legality;
    }

    bool isLegal4King(char** board, char fromChr, int fromDgt, char toChr, int toDgt)
    {
        bool legality = false;
        
        //to convert appropriate index to use in vector of chessBoard.
        int fromChrIndex = fromChr - 97;
        int fromDgtIndex = fromDgt - 1;
        int toChrIndex = toChr - 97;
        int toDgtIndex = toDgt - 1;
        char fromType = board[fromChrIndex][fromDgtIndex];
        char toType = board[toChrIndex][toDgtIndex];

        if(board[toChrIndex][toDgtIndex] == '.')
        {
            legality = true;
        }
        else if(isUpper(fromType) && isLower(toType) || isLower(fromType) && isUpper(toType))
        {
            legality = true;
        }

        return legality;
    }

    bool isLegal4Board(char** board, char fromChr, int fromDgt, char toChr, int toDgt)
    {
        //checking if it is legal according to the state of chess board.

        bool legality = false;
        
        //to convert appropriate index to use in vector of chessBoard.
        int fromChrIndex = fromChr - 97;
        int fromDgtIndex = fromDgt - 1;
        char pieceType = board[fromChrIndex][fromDgtIndex];

        switch (pieceType)
        {
        case 'P':
        case 'p':
            legality = isLegal4Pawn(board,fromChr,fromDgt,toChr,toDgt);
            break;
        case 'R':
        case 'r':
            legality = isLegal4Rook(board,fromChr,fromDgt,toChr,toDgt);
            break;
        case 'N':
        case 'n':
            legality = isLegal4Knight(board,fromChr,fromDgt,toChr,toDgt);
            break;
        case 'B':
        case 'b':
            legality = isLegal4Bishop(board,fromChr,fromDgt,toChr,toDgt);
            break;
        case 'q':
        case 'Q':
            legality = isLegal4Queen(board,fromChr,fromDgt,toChr,toDgt);
            break;
        case 'K':
        case 'k':
            legality = isLegal4King(board,fromChr,fromDgt,toChr,toDgt);
            break;
        default:
            cerr << "ERROR002";
            break;
        }

        return legality;
    }
}

namespace TalhaChess
{
    Board::Board()
    {
        chessBoard.resize(8);
        for(int k=0; k<8; k++)
        {
            chessBoard[k].resize(8, Pieces('.'));
        }
        
        for(int i=0; i<8; i++)
        {
            chessBoard[i][1] = Pieces('P');
        }
        chessBoard[0][0] = Pieces('R');
        chessBoard[1][0] = Pieces('N');
        chessBoard[2][0] = Pieces('B');
        chessBoard[3][0] = Pieces('Q');
        chessBoard[4][0] = Pieces('K');
        chessBoard[5][0] = Pieces('B');
        chessBoard[6][0] = Pieces('N');
        chessBoard[7][0] = Pieces('R');

        for(int i=0; i<8; i++)
        {
            chessBoard[i][6] = Pieces('p');
        }
        chessBoard[0][7] = Pieces('r');
        chessBoard[1][7] = Pieces('n');
        chessBoard[2][7] = Pieces('b');
        chessBoard[3][7] = Pieces('q');
        chessBoard[4][7] = Pieces('k');
        chessBoard[5][7] = Pieces('b');
        chessBoard[6][7] = Pieces('n');
        chessBoard[7][7] = Pieces('r');

        color = 'w';
    }

    bool Board::move(char fromChr, int fromDgt, char toChr, int toDgt)
    {
        //if that function will be failed, return 0.
        
        bool legality;
        char **board = getBoard();

        //to convert appropriate index to use in vector of chessBoard.
        int fromChrIndex = fromChr - 97;
        int fromDgtIndex = fromDgt - 1;
        int toChrIndex = toChr - 97;//'a'
        int toDgtIndex = toDgt - 1;
        
        legality = isLegal4Board(board, fromChr, fromDgt, toChr, toDgt) && 
                    chessBoard[fromChrIndex][fromDgtIndex].isLegal4Piece(fromChr, fromDgt, toChr, toDgt);
        if(legality)
        {
            if(board[fromChrIndex][fromDgtIndex] == 'K' || board[fromChrIndex][fromDgtIndex] == 'k')
            {
                if(isPieceGoingToThreatened(fromChr, fromDgt, toChr, toDgt))
                {
                    cout << "King is forbidden to move there!" << endl;
                    legality = false;
                }
            }
        }

        if(legality)
        {
            char empty = '.';
            chessBoard[toChrIndex][toDgtIndex].setPieceType(chessBoard[fromChrIndex][fromDgtIndex].getPieceType());
            chessBoard[fromChrIndex][fromDgtIndex].setPieceType(empty);
        }

        deleteBoard(board);

        return legality;
    }

    bool Board::isItMate() const
    {
        bool status4K = true, status4k = true;
        int ChrIndex, IntIndex;
        char **board = getBoard();

        char pstChr4K; 
        int pstInt4K;
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(chessBoard[i][j].getPieceType() == 'K')
                {
                    pstChr4K = i + 97;
                    pstInt4K = j + 1;
                    ChrIndex = i;
                    IntIndex = j;
                }
            }
        }
        if(isPieceBeingThreatened(pstChr4K, pstInt4K))
        {
            for(int i=-1; i <= 1; i++)
            {
                for(int j=-1; j <= 1; j++)
                {
                    if((ChrIndex + i >= 0) && (ChrIndex + i <= 7) && (IntIndex + j >=0) && (IntIndex + j) <= 7)
                    {
                        if(!(i==0 && j==0) && (board[ChrIndex + i][IntIndex + j] == '.' || isLower(board[ChrIndex + i][IntIndex +j])))
                        {
                            if(!isGameFinished4King(pstChr4K, pstInt4K, pstChr4K + i, pstInt4K + j))
                            {
                                status4K = false;
                            }                
                        }
                    }
                }
            }
        }
        else
        {
            status4K = false;
        }

        char pstChr4k; 
        int pstInt4k;
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(chessBoard[i][j].getPieceType() == 'k')
                {
                    pstChr4k = i + 97;
                    pstInt4k = j + 1;
                    ChrIndex = i;
                    IntIndex = j;
                }
            }
        }
        if(isPieceBeingThreatened(pstChr4k, pstInt4k))
        {
            for(int i=-1; i <= 1; i++)
            {
                for(int j=-1; j <= 1; j++)
                {
                    if((ChrIndex + i >= 0) && (ChrIndex + i <= 7) && (IntIndex + j >=0) && (IntIndex + j) <= 7)
                    {
                        if(!(i==0 && j==0) && (board[ChrIndex + i][IntIndex + j] == '.' || isUpper(board[ChrIndex + i][IntIndex + j])))
                        {
                            if(!isGameFinished4King(pstChr4k, pstInt4k, pstChr4k + i, pstInt4k + j))
                            {
                                status4k = false;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            status4k = false;
        }

        deleteBoard(board);
        return status4K || status4k;
    }

    bool Board::isItCheck() const
    {
        bool status4K = false;
        char pstChr4K;
        int pstInt4K;
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(chessBoard[i][j].getPieceType() == 'K')
                {
                    pstChr4K = i + 97;
                    pstInt4K = j + 1;
                }
            }
        }
        if(isPieceBeingThreatened(pstChr4K, pstInt4K))
        {
            cout << "Check for white!";
            status4K = true;
        }

        bool status4k = false;
        char pstChr4k;
        int pstInt4k;
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(chessBoard[i][j].getPieceType() == 'k')
                {
                    pstChr4k = i + 97;
                    pstInt4k = j + 1;
                }
            }
        }
        if(isPieceBeingThreatened(pstChr4k, pstInt4k))
        {
            cout << "Check for black!";
            status4k = true;
        }

        return status4K || status4k;
    }

    double Board::score4W() const
    {
        double score = 39.0;
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(isLower(chessBoard[i][j].getPieceType()) && chessBoard[i][j].getPieceType() != '.')
                {
                    switch (chessBoard[i][j].getPieceType())
                    {
                    case 'p':
                        score -= 1;
                        break;
                    case 'n':
                        score -= 3;
                        break;
                    case 'b':
                        score -= 3;
                        break;
                    case 'r':
                        score -= 5;
                        break;
                    case 'q':
                        score -= 9;
                        break;
                    case 'k':
                        break;
                    default:
                        cerr << "What is the piece?" << endl;
                        break;
                    }
                }
                else if(isUpper(chessBoard[i][j].getPieceType()) && chessBoard[i][j].getPieceType() != '.')
                {
                    switch (chessBoard[i][j].getPieceType())
                    {
                    case 'P':
                        if(isPieceBeingThreatened(i+97,j+1))
                        {
                            score -= 0.5;
                        }
                        break;
                    case 'N':
                        if(isPieceBeingThreatened(i+97,j+1))
                        {
                            score -= 1.5;
                        }
                        break;
                    case 'B':
                        if(isPieceBeingThreatened(i+97,j+1))
                        {
                            score -= 1.5;
                        }
                        break;
                    case 'R':
                        if(isPieceBeingThreatened(i+97,j+1))
                        {
                            score -= 2.5;
                        }
                        break;
                    case 'Q':
                        if(isPieceBeingThreatened(i+97,j+1))
                        {
                            score -= 4.5;
                        }
                        break;
                    case 'K':
                        break;
                    default:
                        cerr << "What is the piece?" << endl;
                        break;
                    }
                }
            }
        }
        
        return score;
    }

    double Board::score4B() const
    {
        double score = 39.0;
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(isUpper(chessBoard[i][j].getPieceType()) && chessBoard[i][j].getPieceType() != '.')
                {
                    switch (chessBoard[i][j].getPieceType())
                    {
                    case 'P':
                        score -= 1;
                        break;
                    case 'N':
                        score -= 3;
                        break;
                    case 'B':
                        score -= 3;
                        break;
                    case 'R':
                        score -= 5;
                        break;
                    case 'Q':
                        score -= 9;
                        break;
                    case 'K':
                        break;
                    default:
                        cerr << "What is the piece?" << endl;
                        break;
                    }
                }
                else if(isLower(chessBoard[i][j].getPieceType()) && chessBoard[i][j].getPieceType() != '.')
                {
                    switch (chessBoard[i][j].getPieceType())
                    {
                    case 'p':
                        if(isPieceBeingThreatened(i+97,j+1))
                        {
                            score -= 0.5;
                        }
                        break;
                    case 'n':
                        if(isPieceBeingThreatened(i+97,j+1))
                        {
                            score -= 1.5;
                        }
                        break;
                    case 'b':
                        if(isPieceBeingThreatened(i+97,j+1))
                        {
                            score -= 1.5;
                        }
                        break;
                    case 'r':
                        if(isPieceBeingThreatened(i+97,j+1))
                        {
                            score -= 2.5;
                        }
                        break;
                    case 'q':
                        if(isPieceBeingThreatened(i+97,j+1))
                        {
                            score -= 4.5;
                        }
                        break;
                    case 'k':
                        break;
                    default:
                        cerr << "What is the piece?" << endl;
                        break;
                    }
                }
            }
        }
        
        return score;
    }

    ostream& operator<<(ostream& output, const Board& obj)
    {
        for(int i=0; i<8; i++)
        {
            output << 8-i << " | ";
            for(int j=0; j<8; j++)
            {
                output << obj.chessBoard[j][7-i].getPieceType() << "  ";
            }
            output << endl;
        }

        output << "    -----------------------" << endl;
        output << "    a  b  c  d  e  f  g  h " << endl;
        if(obj.color == 'w') 
        {
            output << "[White's Turn]";
        }
        else if(obj.color == 'b')
        {
            output << "[Black's Turn]";
        }

        return output;
    }
    
    istream& operator>>(istream& input, Board& obj)
    {
        bool inputLegality = true;
        char chr1, chr2;
        int num1, num2;
        
        string ourInput;
        getline(input, ourInput);

        if(ourInput == "suggest" || ourInput == "Suggest")
        {
            cout << "here" << endl;
            if(!obj.giveSuggest(chr1,num1,chr2,num2))
            {
                cerr << "ERROR04" << endl;
            }
            cout << "Suggestion is " << chr1 << num1 << chr2 << num2 << endl;
        }
        else//checking appropriate input
        {
            chr1 = ourInput[0];
            if(chr1 < 97 || chr1 > 104)
            {
                cout << "Wrong input for first character, try again!\n";
                inputLegality = false;
            }
            num1 = static_cast<int>(ourInput[1]) - static_cast<int>('0');
            if(num1 < 1 || num1 > 8)
            {
                cout << "Wrong input for first number, try again!\n";
                inputLegality = false;
            }
            chr2 = ourInput[2];
            if(chr2 < 97 || chr2 > 104)
            {
                cout << "Wrong input for second character, try again!\n";
                inputLegality = false;
            }
            num2 = static_cast<int>(ourInput[3]) - static_cast<int>('0');
            if(num2 < 1 || num2 > 8)
            {
                cerr << "Wrong input for second number, try again!\n";
                inputLegality = false;
            }
        }

        if(inputLegality)
        {
            //checking whose turn it is
            if(obj.color == 'w' && isUpper(obj.chessBoard[chr1 - 97][num1 - 1].getPieceType()))
            {
                obj.color = 'b';
                if(! obj.move(chr1, num1, chr2, num2))
                {
                    cout << "It is not legal for piece which you chose!\n";
                    obj.color = 'w';
                }
            }
            else if(obj.color == 'b' && isLower(obj.chessBoard[chr1 - 97][num1 - 1].getPieceType()))
            {
                obj.color = 'w';
                if(! obj.move(chr1, num1, chr2, num2))
                {
                    cout << "It is not legal for piece which you chose!\n";
                    obj.color = 'b';
                }
            }
            else
            {
                cout << "Not your turn!\n";
            }
        }

        return input;
    }

    char** Board::getBoard() const
    {
        //Get board as char array.
        char **arr = nullptr;
        bool succed = true;
        while(succed)
        {
            succed = false;
            arr = new (nothrow) char*[8];
            for(int i=0; i< 8; i++)
            {
                arr[i] = new (nothrow) char[8];
            }

            for(int i=0; i<8; i++)
            {
                if(arr[i])
                {
                }
                else
                {
                    cout << "Memory allocation failed. Wait just a second" << endl;
                    succed = true;
                    break;
                }
            }
            
            if(arr)
            {}
            else
            {
                cout << "Memory allocation failed. Wait just a second" << endl;
                succed = true;
            }
        }

        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                arr[i][j] = chessBoard[i][j].getPieceType();
            }
        }
        
        return arr;
    }
    
    void Board::deleteBoard(char** board) const
    {
        for(int i = 0; i < 8; i++) 
        {
            delete[] board[i];
        }
        delete[] board;
    }

    bool Board::isPieceGoingToThreatened(char fromChr, int fromDgt, char toChr, int toDgt) const
    {   
        //If the piece ,which is going to move, will be under threat , then return true.

        bool legality = false;
        char **board = getBoard();

        //to convert appropriate index to use in vector of chessBoard.
        int fromChrIndex = fromChr - 97;
        int fromDgtIndex = fromDgt - 1;
        int toChrIndex = toChr - 97;//'a'
        int toDgtIndex = toDgt - 1;
        
        bool isLegalToMove =isLegal4Board(board, fromChr, fromDgt, toChr, toDgt) && 
                    chessBoard[fromChrIndex][fromDgtIndex].isLegal4Piece(fromChr, fromDgt, toChr, toDgt); 
        if(!isLegalToMove)
        {
            cerr << "Invalid input for isPieceGoingToThreatened() function " << endl;
            exit(-1);
        }

        bool isItLegalToMove;
        board[toChrIndex][toDgtIndex] = board[fromChrIndex][fromDgtIndex];

        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(chessBoard[i][j].getPieceType() != '.')
                {
                    isItLegalToMove = chessBoard[i][j].isLegal4Piece(i+97,j+1,toChr,toDgt) && isLegal4Board(board,i+97,j+1,toChr,toDgt);
                    if(isItLegalToMove)
                    {
                        legality = true;
                    }
                }
            }
        }

        deleteBoard(board);
        return legality;
    }

    bool Board::isGameFinished4King(char fromChr, int fromDgt, char toChr, int toDgt) const
    {
        //if there is no chance to escape for King, then return true.
        bool legality = false;
        char **board = getBoard();

        //to convert appropriate index to use in vector of chessBoard.
        int fromChrIndex = fromChr - 97;
        int fromDgtIndex = fromDgt - 1;
        int toChrIndex = toChr - 97; 
        int toDgtIndex = toDgt - 1;
        
        bool isLegalToMove =isLegal4Board(board, fromChr, fromDgt, toChr, toDgt) && 
                    chessBoard[fromChrIndex][fromDgtIndex].isLegal4Piece(fromChr, fromDgt, toChr, toDgt); 
        if(!isLegalToMove)
        {
            cerr << "Invalid input for isPieceGoingToThreatened() function " << endl;
            exit(-1);
        }

        bool isItLegalToMove;
        board[toChrIndex][toDgtIndex] = board[fromChrIndex][fromDgtIndex];

        int numOfThreats = 0;
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(chessBoard[i][j].getPieceType() != '.')
                {
                    isItLegalToMove = chessBoard[i][j].isLegal4Piece(i+97,j+1,toChr,toDgt) && isLegal4Board(board,i+97,j+1,toChr,toDgt);
                    if(isItLegalToMove)
                    {
                        if(!isPieceBeingThreatened(i+97, j+ 1))
                        {
                            numOfThreats++;
                            legality = true;
                        }
                    }
                }
            }
        }
        
        if(numOfThreats > 1)
        {
            legality = false;
        }
        
        deleteBoard(board);
        return legality;
    }

    bool Board::isPieceBeingThreatened(char currChr, int currDgt) const
    {
        //If the piece being threatened, then return true. 
        
        bool legality = false;
        char **board = getBoard();

        //to convert appropriate index to use in vector of chessBoard.
        int currChrIndex = currChr - 97;
        int currDgtIndex = currDgt - 1;
        
        bool isItLegalToMove;
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(chessBoard[i][j].getPieceType() != '.')
                {
                    isItLegalToMove = chessBoard[i][j].isLegal4Piece(i+97,j+1,currChr,currDgt) && isLegal4Board(board,i+97,j+1,currChr,currDgt);
                    if(isItLegalToMove)
                    {
                        deleteBoard(board);
                        return legality = true;
                    }
                }
            }
        }

        deleteBoard(board);
        return legality;
    }

    bool Board::giveSuggest(char& fromChr, int& fromDgt, char& toChr, int& toDgt) const
    {
        //if that function will be failed, return 0.
        
        bool succeed = false;
        char** board = getBoard();

        bool isItLegalToMove;
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(chessBoard[i][j].getPieceType() != '.')
                {                    
                    if((color == 'b' && isLower(chessBoard[i][j].getPieceType()))
                    || (color == 'w' && isUpper(chessBoard[i][j].getPieceType())))
                    {
                        for(int k=0; k<8; k++)
                        {
                            for(int t=0; t<8; t++)
                            {
                                isItLegalToMove = chessBoard[i][j].isLegal4Piece(i+97,j+1,k+97,t+1) && isLegal4Board(board,i+97,j+1,k+97,t+1);
                                if(isItLegalToMove && !isPieceGoingToThreatened(i+97, j+1, k+97, t+1))
                                {
                                    fromChr = i + 97;
                                    fromDgt = j + 1;
                                    toChr = k + 97;
                                    toDgt = t + 1;
                                    succeed = true;
                                    deleteBoard(board);
                                    return succeed;
                                }
                            }
                        }
                    }
                }
            }
        }

        deleteBoard(board);
        return succeed;
    }
}