#include "player.hpp"
#include <map>

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

// Returns greedy heuristic for this move
// (n_{this} - n_{opponent})
// Returns 0 if the move is invalid
int Player::greedy_heuristic(Move *move, Side move_side)
{
    int ret = 0;
    if(board->checkMove(move, move_side))
    {
        Board* copy = board->copy();
        copy->doMove(move, move_side);
        ret = copy->count(move_side) - copy->count(OTHER_SIDE(move_side));
    }
    return ret;
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

    std::map<int, Move> possible_moves;
    Move move(0, 0);
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            move.set_x(i);
            move.set_y(j);
            if(board->checkMove(&move, side))
            {
                std::cerr << "Possible move: (" << move.x << ", " << move.y << std::endl;
                possible_moves.insert(std::pair<int, Move>(greedy_heuristic(&move, side), move));
            }
        }
    }

    Move* ret = nullptr;
    if(!possible_moves.empty())
    {
        ret = new Move(possible_moves.rbegin()->second);
        std::cerr << "Chosen move: (" << ret->x << ", " << ret->y << std::endl;
    }

    return ret;
}
