#include <stdio.h>
#include "defs.h"
#include <stdlib.h>
int main()
{
    AllInit();
    U64 playBitBoard = 0ULL;
    int index = 0;
    for (index = 0; index < 64; index++)
    {
        printf("Index :%d \n\n", index);
        PrintBitBoard(clearMask[index]);
        printf("\n\n");
    }
    return 0;
}