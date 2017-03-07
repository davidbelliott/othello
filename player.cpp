#include "player.hpp"
#include <map>
#include <limits>
#include <vector>

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side_in)
    : board(new Board()),
      side(side_in),
      testingMinimax(false)   // Will be set to true in test_minimax.cpp.
{
}

/*
 * Destructor for the player.
 */
Player::~Player()
{
    delete board;
}

void Player::set_board(Board* board_in)
{
    board = board_in;
}

double Player::minimax(Board* board, int depth, bool maximizing)
{
    double score = 0.0;
    if(depth == 0)
    {
        score = parity_heuristic(board, side);
        std::cerr << "Heuristic: " << score << std::endl;
    }
    else
    {
        score = maximizing ? -std::numeric_limits<double>::max() : std::numeric_limits<double>::max();
        Move move(0, 0);
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                move.set_x(i);
                move.set_y(j);
                Side moving_side = maximizing ? side : OTHER_SIDE(side);
                if(board->checkMove(&move, moving_side))
                {
                    Board* copy = board->copy();
                    copy->doMove(&move, moving_side);
                    double this_score = minimax(copy, depth - 1, !maximizing);
                    if(maximizing)
                        score = std::max(score, this_score);
                    else
                        score = std::min(score, this_score);
                    std::cerr << "Maximizing: " << maximizing << "\nScore:" << score << "\nThis score:" << this_score << std::endl << std::endl;
                    delete copy;
                }
            }
        }
    }
    return score;
}

/**
 * @brief Returns the difference between move_side's pieces and the other
 * side's pieces.
 */
int Player::parity_heuristic(Board* board, Side move_side)
{
    int ret = board->count(move_side) - board->count(OTHER_SIDE(move_side));
    return ret;
}

/**
 * @brief Return value is incremented by 1 for every move_side edge piece,
 * incremented by 2 for every move_side corner piece, decremented by 1 for
 * move_side pieces 1 away from the edge, and the opposite for the other side's
 * pieces.
 */
int Player::edge_heuristic(Board* board, Side move_side)
{
    int ret = 0;

    // Note: double-counting corners is intentional
    // Top and bottom edges
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j += 7)
        {
            if(board->get(move_side, i, j))
                ret+=1;
            else if(board->get(OTHER_SIDE(move_side), i, j))
                ret-=1;
        }
    }

    for(int i = 1; i < 7; i++)
    {
        for(int j = 1; j < 7; j += 6)
        {
            if(board->get(move_side, i, j))
                ret-=1;
            else if(board->get(OTHER_SIDE(move_side), i, j))
                ret+=1;
        }
    }


    return ret;
}

int Player::mobility_heuristic(Board* board, Side move_side)
{
    int n_moves = 0;
    int n_opponent_moves = 0;
    Move move(0, 0);
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            move.set_x(i);
            move.set_y(j);
            if(board->checkMove(&move, move_side))
                n_moves++;
            if(board->checkMove(&move, OTHER_SIDE(move_side)))
                n_opponent_moves++;
        }
    }
    return n_moves - n_opponent_moves;
}

/**
 * @brief Returns a weighted sum of all heuristics.
 */
double Player::heuristic(Board* board, Side move_side)
{
    return 1.0 * parity_heuristic(board, move_side); + 
        testingMinimax ? 0.0 : 
        ( 5.0 * edge_heuristic(board, move_side) +
          5.0 * mobility_heuristic(board, move_side) );
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft)
{
    board->doMove(opponentsMove, OTHER_SIDE(side));

    bool found_move = false;
    std::pair<double, Move> best_move(0.0, Move(0, 0));
    Move move(0, 0);
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            move.set_x(i);
            move.set_y(j);
            if(board->checkMove(&move, side))
            {
                double score = minimax(board, 1, true);
                if(!found_move || score > best_move.first)
                {
                    found_move = true;
                    best_move.first = score;
                    best_move.second = move;
                }
            }
        }
    }

    Move* ret = nullptr;
    if(found_move)
    {
        board->doMove(&best_move.second, side);
        ret = new Move(best_move.second);
    }

    return ret;
}
