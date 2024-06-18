#include <stdlib.h>

#include "../includes/aicu.h"

t_board *init_board()
{
    t_board	*board = ft_calloc(1, sizeof(t_board));
    if (!board)
        return (NULL);

    board->heaps = ft_calloc(DEFAULT_HEAP_AMOUNT, sizeof(int));
    if (!board->heaps)
    {
        free(board);
        return (NULL);
    }
    board->_heap_size = DEFAULT_HEAP_AMOUNT;

    return (board);
}

t_err add_heap(t_board *board, int heap)
{
	int	*new_heap;

	if(board->num_heaps == board->_heap_size)
	{
		new_heap = ft_calloc(board->_heap_size * 2, sizeof(int));
		if (new_heap == NULL)
			return (ERROR);

		board->_heap_size *= 2;
		for (int i = 0; i < board->num_heaps; ++i)
			new_heap[i] = board->heaps[i];

		free(board->heaps);
		board->heaps = new_heap;
	}

	board->heaps[board->num_heaps] = heap;
	board->num_heaps += 1;

	return (SUCCESS);
}


t_err	update_board(t_board *board, int items_removed)
{
	if (board->num_heaps == 0)
		return (ERROR);

    if (items_removed < 1 || items_removed > 3)
        return (ERROR);

    if (board->heaps[board->num_heaps - 1] - items_removed <= 0)
        board->num_heaps--;
    else
        board->heaps[board->num_heaps - 1] -= items_removed;

    return (SUCCESS);
}

void free_board(t_board *board)
{
	free(board->heaps);
	free(board);
}
