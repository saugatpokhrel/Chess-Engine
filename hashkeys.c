#include "defs.h"

U64 GeneratePosKey(const S_BOARD *pos)
{
    int sq;
    U64 finalKey = 0;
    int piece;

    for (sq = 0; sq < BRD_SQ_NUM; sq++)
    {
        piece = pos->pieces[sq];

        if (piece != OFFBOARD && piece != EMPTY)
        {
            ASSERT(piece >= wP && piece <= bK);  
            finalKey ^= pieceKeys[piece][sq];      
        }
    }

    if (pos->side == WHITE)
    {
        finalKey ^= sideKey;
    }

    if (pos->enPas != NO_SQ)
    {
        ASSERT(pos->enPas >= 0 && pos->enPas < BRD_SQ_NUM);
        finalKey ^= pieceKeys[EMPTY][pos->enPas];
    }

    ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);
    finalKey ^= castleKeys[pos->castlePerm];

    return finalKey;
}
