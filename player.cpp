#include "player.hpp"

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
    if(board->checkMove(opponentsMove, OTHER_SIDE(side)))
        board->doMove(opponentsMove, OTHER_SIDE(side));

    Move move(0, 0);
    bool did_move = false;
    for(int i = 0; i < 8 && !did_move; i++)
    {
        for(int j = 0; j < 8 && !did_move; j++)
        {
            move.setX(i);
            move.setY(j);
            if(board->checkMove(&move, side))
            {
                board->doMove(&move, side);
                did_move = true;
            }
        }
    }

    return (did_move ? new Move(move) : nullptr);
}
