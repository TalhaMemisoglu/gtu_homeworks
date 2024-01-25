#ifndef PIECES_H
#define PIECES_H

namespace TalhaChess
{
    class Pieces
    {
    public:
        Pieces();
        Pieces(char type);
        void setPieceType(char type);
        char getPieceType() const;
        
        //Checking if it is legal for move rules of pieces
        bool isLegal4Piece(char fromChr, int fromDgt, char toChr, int toDgt) const;
    private:
        char pieceType;
    };
}

#endif //PIECES_H