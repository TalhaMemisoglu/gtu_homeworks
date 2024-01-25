#include <iostream>
#include <vector>
#include "board.h"
#include "pieces.h"

namespace
{
    bool isLegal4Pawn(char Type, char fromChr, int fromDgt, char toChr, int toDgt)
    {
        bool legality = false;
        if(Type == 'P')
        {
            if(fromDgt == 2 && fromChr == toChr && toDgt == 4)
            {
                
                legality = true;
            }
            else if(fromChr == toChr && toDgt == fromDgt+1)
            {
                legality = true;
            }
            else if(toChr == fromChr + 1 && toDgt == fromDgt + 1)
            {
                legality = true;
            }
            else if(toChr == fromChr - 1 && toDgt == fromDgt + 1)
            {
                legality = true;
            }
        }
        else if(Type == 'p')
        {
            if(fromDgt == 7 && fromChr == toChr && toDgt == 5 )
            {
                legality = true;
            }
            else if(fromChr == toChr && toDgt == fromDgt-1)
            {
                legality = true;
            }
            else if(toChr == fromChr - 1 && toDgt == fromDgt - 1)
            {
                legality = true;
            }
            else if(toChr == fromChr + 1 && toDgt == fromDgt - 1)
            {
                legality = true;
            }
        }

        return legality;
    }

    bool isLegal4Rook(char fromChr, int fromDgt, char toChr, int toDgt)
    {
        bool legality = false;

        if(fromChr == toChr)
        {
            legality = true;
        }
        else if(fromDgt == toDgt)
        {
            legality = true;
        }

        return legality;
    }

    bool isLegal4Knight(char fromChr, int fromDgt, char toChr, int toDgt)
    {
        bool legality = false;

        if(toChr == fromChr + 2 && toDgt == fromDgt + 1)
        {
            legality = true;
        }
        else if(toChr == fromChr + 1 && toDgt == fromDgt + 2)
        {
            legality = true;
        }
        else if(toChr == fromChr - 1 && toDgt == fromDgt + 2)
        {
            legality = true;
        }
        else if(toChr == fromChr - 2 && toDgt == fromDgt + 1)
        {
            legality = true;
        }
        else if(toChr == fromChr - 2 && toDgt == fromDgt - 1)
        {
            legality = true;
        }
        else if(toChr == fromChr - 1 && toDgt == fromDgt - 2)
        {
            legality = true;
        }
        else if(toChr == fromChr + 1 && toDgt == fromDgt - 2)
        {
            legality = true;
        }
        else if(toChr == fromChr + 2 && toDgt == fromDgt - 1)
        {
            legality = true;
        }
        
        return legality;
    }

    bool isLegal4Bishop(char fromChr, int fromDgt, char toChr, int toDgt)
    {
        int legality = false;

        for(int i = 1; (fromChr+i)<='h'; i++)
        {
            if((fromChr+i)==toChr && (fromDgt+i)==toDgt)
            {
                legality = true;
            }
            else if((fromChr+i)==toChr && (fromDgt-i)==toDgt)
            {
                legality = true;
            }
        }

        for(int i = 1; (fromChr-i) >='a'; i++)
        {
            if((fromChr-i)==toChr && (fromDgt+i)==toDgt)
            {
                legality = true;
            }
            else if((fromChr-i)==toChr && (fromDgt-i)==toDgt)
            {
                legality = true;
            }
        }

        return legality;
    }

    bool isLegal4Queen(char fromChr, int fromDgt, char toChr, int toDgt)
    {
        bool legality = false;

        if(isLegal4Bishop(fromChr,fromDgt,toChr,toDgt) || isLegal4Rook(fromChr,fromDgt,toChr,toDgt))
        {
            legality = true;
        }

        return legality;
    }

    bool isLegal4King(char fromChr, int fromDgt, char toChr, int toDgt)
    {
        bool legality = false;

        if(toChr == fromChr + 1 && toDgt == fromDgt)
        {
            legality = true;
        }
        else if(toChr == fromChr + 1 && toDgt == fromDgt + 1)
        {
            legality = true;
        }
        else if(toChr == fromChr && toDgt == fromDgt + 1)
        {
            legality = true;
        }
        else if(toChr == fromChr - 1 && toDgt == fromDgt)
        {
            legality = true;
        }
        else if(toChr == fromChr - 1 && toDgt == fromDgt + 1)
        {
            legality = true;
        }
        else if(toChr == fromChr - 1 && toDgt == fromDgt - 1)
        {
            legality = true;
        }
        else if(toChr == fromChr && toDgt == fromDgt - 1)
        {
            legality = true;
        }
        else if(toChr == fromChr + 1 && toDgt == fromDgt - 1)
        {
            legality = true;
        }

        return legality;
    }
}


namespace TalhaChess
{
    Pieces::Pieces() : pieceType('.')
    {/*Intentionally left blank*/}
    
    Pieces::Pieces(char type) : pieceType(type)
    {/*Intentionally left blank*/}

    void Pieces::setPieceType(char type)
    {
        pieceType = type;
    }

    char Pieces::getPieceType() const
    {
        return pieceType;
    }

    bool Pieces::isLegal4Piece(char fromChr, int fromDgt, char toChr, int toDgt) const
    {
        //Checking if it is legal for move rules of pieces
        bool legality = false;

        switch (pieceType)
        {
        case 'P':
        case 'p':
            legality = isLegal4Pawn(pieceType,fromChr,fromDgt,toChr,toDgt);
            break;
        case 'R':
        case 'r':
            legality = isLegal4Rook(fromChr,fromDgt,toChr,toDgt);
            break;
        case 'N':
        case 'n':
            legality = isLegal4Knight(fromChr,fromDgt,toChr,toDgt);
            break;
        case 'B':
        case 'b':
            legality = isLegal4Bishop(fromChr,fromDgt,toChr,toDgt);
            break;
        case 'q':
        case 'Q':
            legality = isLegal4Queen(fromChr,fromDgt,toChr,toDgt);
            break;
        case 'K':
        case 'k':
            legality = isLegal4King(fromChr,fromDgt,toChr,toDgt);
            break;
        case '.':
            legality = false;
            break;
        default:
            cerr << "ERROR001";
            break;
        }

        return legality;
    }
}