#include "othello_node.hpp"
#include <limits>

OthelloNode::OthelloNode(Board* board_in, Side move_side_in)
    : board(board_in),
      move_side(move_side_in),
      children()
{
}

OthelloNode::~OthelloNode()
{
    delete board;
}
