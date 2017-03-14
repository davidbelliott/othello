#pragma once

//#include "opening_book.hpp"
#include "board.hpp"
#include "othello_node.hpp"
#include "common.hpp"
#include <iostream>
#include <vector>

class Player {
private:
    Board* board;
    //std::map<Board, OthelloNode, BoardCmp> transpositions;
    int erm;            // Estimated remaining moves (for use in timing)
    char player_side;
    static const int weights[8][8];

public:
    static const int default_depth;
    static const int max_depth;

    Player(char side_in);
    ~Player();

    void set_board(Board* board);
    int get_weight(Board* board, char move_side, int i, int j);
    int heuristic(Board* board, char move_side);
    void get_possible_moves(Board* board, char side, std::vector<Move>* moves);
    int negamax(Board* board, int depth, char move_side, int a, int b, Move** m=nullptr);
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};
