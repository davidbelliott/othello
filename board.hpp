#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.hpp"
using namespace std;

class Board {

public:
    char data[64];

    bool occupied(int x, int y);
    bool get(char side, int x, int y);
    void set(char side, int x, int y);

    Board();
    Board(char data_in[64]);
    ~Board();

    bool onBoard(int x, int y);
    bool isDone();
    bool hasMoves(char side);
    bool checkMove(Move *m, char side);
    void doMove(Move *m, char side);
    int count(char side);
    void setBoard(char data[]);
    bool operator<(Board& other);
};

#endif
