#include<stdio.h>
#include "defs.h"

int Sq120ToSq64[BRD_SQ_NUM];
 int Sq64ToSq120[64];
 U64 setMask[64];
U64 clearMask[64];

void InitBitMasks(){
    int index=0;
    for(index=0;index<64;index++){
        setMask[index]=0ULL;
        clearMask[index]=0ULL;
    }
    for(index=0;index<64;index++){
        setMask[index]=setMask[index]|(1ULL<<index);
        clearMask[index]=~setMask[index];
    }
}
void Initsq120to64(){
    int index=0;
    int file=FILE_A;
    int rank=RANK_1;
    int sq= A1;
    int sq64=0;
    for(index=0;index<BRD_SQ_NUM;index++){
        Sq120ToSq64[index]=65;
    }
    for(index=0;index<64;index++){
        Sq64ToSq120[index]=120;
    }
    for(rank=RANK_1;rank<=RANK_8;rank++){
        for(file=FILE_A;file<=FILE_H;file++){
            sq=FR2SQ(file,rank);
            Sq64ToSq120[sq64]=sq;
            Sq120ToSq64[sq]=sq64;
            sq64++;
        }
    }
}

void  AllInit() {
    Initsq120to64();
    InitBitMasks();

}