#include "../includes/aicu.h"

int	win_strategy(int last_heap)
{
	int	move;

	if (last_heap % 4 == 0 && last_heap >= 3)
		move = 3;
	else if (last_heap % 4 == 3 && last_heap >= 2)
		move = 2;
	else
		move = 1;
	return (move);
}

int	lose_strategy(int last_heap)
{
	int	move;

	if (last_heap % 4 == 3)
		move = 3;
	else if (last_heap % 4 == 2)
		move = 2;
	else
		move = 1;
	return (move);
}

int get_ai_move(t_board *board, int *last_turn, int *strategy)
{
    int	move;
    int	last_heap = board->heaps[board->num_heaps - 1];

	if (strategy[board->num_heaps - 1])
		move = win_strategy(last_heap);
	else
		move = lose_strategy(last_heap);

	*last_turn = 1;
    return (move);
}
