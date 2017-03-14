Caltech CS2 Assignment 9: Othello

See [assignment9_part1.html](http://htmlpreview.github.io/?https://github.com/caltechcs2/othello/blob/master/assignment9_part1.html) and [assignment9_part2.html](http://htmlpreview.github.io/?https://github.com/caltechcs2/othello/blob/master/assignment9_part2.html)

## Contributions of team members

This was a solo team.

## Improvements made to the AI

I began by implementing a simple heuristic that just totalled all of the squares occupied by the target player multiplied by weights for each square (which favored corners, etc). This heuristic proved more effective than a combination of more complex heuristics, e.g. stability, parity, and corners captured. I modified the usual minimax algorithm, using negamax for simplicity. I also implemented alpha-beta pruning and iterative deepening. The iterative deepening roughly estimates the number of moves remaining in the game, allocates a set block of time for iteration, and then begins iterating. After each iteration, it estimates the time cost of another iteration. If the cost would exceed the time allotment, it does not continue. This allows it to iterate as deep as possible, usually 10-14 moves in advance.

I began implementing an opening book, and rewrote the provided board code to allow mapping of boards to moves and simplify comparison between boards and serialization of boards. I'm still working on the opening book, and not sure if it will be ready for the competition.
