#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "defs.h"

void UpdateListsMaterial(S_BOARD *pos) {
    memset(pos->bigPce, 0, sizeof(pos->bigPce));
    memset(pos->minPce, 0, sizeof(pos->minPce));
    memset(pos->majPce, 0, sizeof(pos->majPce));
    memset(pos->material, 0, sizeof(pos->material));
    memset(pos->pceNum, 0, sizeof(pos->pceNum));
    pos->pawns[WHITE] = pos->pawns[BLACK] = pos->pawns[BOTH] = 0ULL;
    
    for (int index = 0; index < BRD_SQ_NUM; index++) {
        int sq = index;
        int piece = pos->pieces[index];
        if (piece != OFFBOARD && piece != EMPTY) {
            int color = PieceCol[piece];
            if (PieceBig[piece]) pos->bigPce[color]++;
            if (PieceMin[piece]) pos->minPce[color]++;
            if (PieceMaj[piece]) pos->majPce[color]++;
            pos->material[color] += PieceVal[piece];
            pos->pList[piece][pos->pceNum[piece]++] = sq;
            if (piece == wK || piece == bK) pos->KingSq[color] = sq;
            if (piece == wP) {
                SETBIT(pos->pawns[WHITE], Sq120ToSq64[sq]);
                SETBIT(pos->pawns[BOTH], Sq120ToSq64[sq]);
            } else if (piece == bP) {
                SETBIT(pos->pawns[BLACK], Sq120ToSq64[sq]);
                SETBIT(pos->pawns[BOTH], Sq120ToSq64[sq]);
            }
        }
    }
}

void ResetBoard(S_BOARD *pos) {
    memset(pos, 0, sizeof(S_BOARD));
    for (int index = 0; index < BRD_SQ_NUM; index++) pos->pieces[index] = OFFBOARD;
    for (int index = 0; index < 64; index++) pos->pieces[SQ120(index)] = EMPTY;
    pos->KingSq[WHITE] = pos->KingSq[BLACK] = NO_SQ;
    pos->side = BOTH;
    pos->enPas = NO_SQ;
    pos->castlePerm = 0;
}

int ParseFen(char *fen, S_BOARD *pos) {
    if (!fen || !pos) return -1;
    ResetBoard(pos);
    int rank = RANK_8, file = FILE_A, piece, count, sq64, sq120;
    while (rank >= RANK_1 && *fen) {
        count = 1;
        switch (*fen) {
            case 'p': piece = bP; break;
            case 'r': piece = bR; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'k': piece = bK; break;
            case 'q': piece = bQ; break;
            case 'P': piece = wP; break;
            case 'R': piece = wR; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'K': piece = wK; break;
            case 'Q': piece = wQ; break;
            case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
                piece = EMPTY;
                count = *fen - '0';
                break;
            case '/': case ' ': rank--; file = FILE_A; fen++; continue;
            default: return -1;
        }
        for (int i = 0; i < count; i++) {
            sq64 = rank * 8 + file;
            sq120 = SQ120(sq64);
            if (piece != EMPTY) pos->pieces[sq120] = piece;
            file++;
        }
        fen++;
    }
    pos->side = (*fen == 'w') ? WHITE : BLACK;
    fen += 2;
    while (*fen != ' ' && *fen) {
        switch (*fen) {
            case 'K': pos->castlePerm |= wKcA; break;
            case 'Q': pos->castlePerm |= wQcA; break;
            case 'k': pos->castlePerm |= bKcA; break;
            case 'q': pos->castlePerm |= bQcA; break;
        }
        fen++;
    }
    fen++;
    if (*fen != '-') {
        file = fen[0] - 'a';
        rank = fen[1] - '1';
        pos->enPas = FR2SQ(file, rank);
    }
    pos->posKey = GeneratePosKey(pos);
    UpdateListsMaterial(pos);
    return 0;
}

int CheckBoard(const S_BOARD *pos) {
    int t_pceNum[13] = {0}, t_bigPce[2] = {0}, t_majPce[2] = {0}, t_minPce[2] = {0}, t_material[2] = {0};
    U64 t_pawns[3] = {pos->pawns[WHITE], pos->pawns[BLACK], pos->pawns[BOTH]};
    for (int sq64 = 0; sq64 < 64; ++sq64) {
        int sq120 = SQ120(sq64);
        int piece = pos->pieces[sq120];
        if (piece != EMPTY) {
            t_pceNum[piece]++;
            int color = PieceCol[piece];
            if (PieceBig[piece]) t_bigPce[color]++;
            if (PieceMin[piece]) t_minPce[color]++;
            if (PieceMaj[piece]) t_majPce[color]++;
            t_material[color] += PieceVal[piece];
        }
    }
    for (int piece = wP; piece <= bK; ++piece) ASSERT(t_pceNum[piece] == pos->pceNum[piece]);
    ASSERT(GeneratePosKey(pos) == pos->posKey);
    return TRUE;
}

void PrintBoard(const S_BOARD *pos) {
    printf("\nGame Board:\n\n");
    for (int rank = RANK_8; rank >= RANK_1; rank--) {
        printf("%d  ", rank + 1);
        for (int file = FILE_A; file <= FILE_H; file++) {
            printf("%3c", PceChar[pos->pieces[FR2SQ(file, rank)]]);
        }
        printf("\n");
    }
    printf("\n   a  b  c  d  e  f  g  h\n");
    printf("side: %c\n", SideChar[pos->side]);
    printf("enPas: %d\n", pos->enPas);
    printf("castle: %c%c%c%c\n", pos->castlePerm & wKcA ? 'K' : '-', pos->castlePerm & wQcA ? 'Q' : '-', pos->castlePerm & bKcA ? 'k' : '-', pos->castlePerm & bQcA ? 'q' : '-');
    printf("posKey: %llX\n", pos->posKey);
}
