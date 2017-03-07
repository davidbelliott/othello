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
    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player()
{
    delete board;
}

double Player::minimax(Board* board, int depth, bool maximizing)
{
    double score = 0.0;
    if(depth == 0)
    {
        score = heuristic(board, side);
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

int Player::parity_heuristic(Board* board, Side move_side)
{
    int ret = board->count(move_side) - board->count(OTHER_SIDE(move_side));
    return ret;
}

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
                ret++;
            else if(board->get(OTHER_SIDE(move_side), i, j))
                ret--;
        }
    }

    for(int i = 1; i < 7; i++)
    {
        for(int j = 1; j < 7; j+= 6)
        {
            if(board->get(move_side, i, j))
                ret--;
            else if(board->get(OTHER_SIDE(move_side), i, j))
                ret++;
        }
    }

    return ret;
}

double Player::heuristic(Board* board, Side move_side)
{
    return 1.0 * parity_heuristic(board, move_side) + 2.0 * edge_heuristic(board, move_side);
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

    double best = minimax(board, 4, true);

    std::cerr << "best: " << best << std::endl;

    //if(ret)
        //board->doMove(ret, side);

    return nullptr;
}
