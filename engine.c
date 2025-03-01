#include <stdio.h>
#include "defs.h"
int main()
{
    AllInit();
    U64 playBitBoard = 0ULL;
    printf("\nStarting Board:\n");
    PrintBitBoard(playBitBoard);
    playBitBoard |= (1ULL << SQ64(D2));
    printf("\nBoard after adding a piece: in D2\n");
    PrintBitBoard(playBitBoard);
    playBitBoard |= (1ULL << SQ64(G2));
    printf("\nBoard after adding a piece: in G2\n");
    PrintBitBoard(playBitBoard);

    return 0;
}