#include "othello_node.hpp"

OthelloNode::OthelloNode(Board* board_in, Side side_in)
    : board(board_in),
      side(side_in),
      children()
{
}

OthelloNode::~OthelloNode()
{
    for(auto it = children.begin(); it != children.end(); ++it)
        delete *it;
    delete board;
}

double OthelloNode::populate_children(int depth, Side side, bool maximizing)
{
    if(depth == 0)
        weight = heuristic(side);
    else
    {
        weight = maximizing ? std::numeric_limits<double>::min() : std::numeric_limits<double>::max();
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                Move move(i, j);
                if(board->checkMove(&move, side))
                {
                    OthelloNode* child = new OthelloNode(board->copy(), OTHER_SIDE(side));
                    child->board->doMove(&move, side);
                    if(maximizing)
                        weight = std::max(weight, child->populate_children(depth - 1, false));
                    else
                        weight = std::min(weight, child->populate_children(depth - 1, true));
                    children.push_back(child);
                }
            }
        }
    }
    return weight;
}

int OthelloNode::parity_heuristic(Side move_side)
{
    int ret = board->count(move_side) - board->count(OTHER_SIDE(move_side));
    return ret;
}

int OthelloNode::edge_heuristic(Side move_side)
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

double OthelloNode::heuristic(Side move_side)
{
    return 1.0 * parity_heuristic(board, move_side) + 2.0 * edge_heuristic(board, move_side);
}
