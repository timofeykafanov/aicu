#include "../includes/aicu.h"

void	display_board(t_board *board)
{
	int	dif;
	int	biggest = 0;

	for	(int i = 0; i < board->num_heaps; i++)
    {
		for (int k = 0; k < board->num_heaps; k++)
		{
			if (biggest < board->heaps[k])
				biggest = board->heaps[k];
		}
		dif = biggest - board->heaps[i];
		for (int x = 0; x < dif; x++)
			ft_printf(" ");
        for (int j = 0; j < board->heaps[i]; j++)
		{
            ft_printf("| ");
		}
        ft_printf("\n");
    }
}

int get_move(int *last_turn, t_board *board)
{
    char	*line;
	char	*trimmed;
    int		move = 0;

    ft_printf("Please choose between 1 and 3 items: ");
	line = get_next_line(1);
	// free(line);
	// line = NULL;
	if (!line)
		return (ft_printf("Error reading input\n"), 0);
    while (line)
	{
		if (ft_strlen(line) > 2)
		{
			trimmed = ft_strtrim(line, "\n");
			ft_printf(trimmed);
			free(trimmed);
			ft_printf(" - Invalid choice\n");
			ft_printf("Please choose between 1 and 3 items: ");
			free(line);
			line = get_next_line(1);
			continue ;
		}
        move = ft_atoi(line);
        free(line);
		if (move < 1 || move > 3 || board->heaps[board->num_heaps - 1] < move)
		{
			ft_printf("%d - Invalid choice\n", move);
			ft_printf("Please choose between 1 and 3 items: ", 1);
		}
		else
		    break ;
		line = get_next_line(1);
    }
	*last_turn = 0;
    return (move);
}

int main(int argc, char **argv)
{
	int			last_turn = 0;
	int			move;
	int			*strategy = NULL;
    t_board		*board;

    t_err err = prepare_game(argc, argv, &board, &strategy);
    if (err != SUCCESS)
    	return (free(strategy), err);

    while (1)
    {
		if (!last_turn)
		{
        	display_board(board);
			move = get_ai_move(board, &last_turn, strategy);
			ft_printf("AI took %d\n", move);
        	if (update_board(board, move) == ERROR)
				return (free(strategy), ERROR);
		}
		else if (last_turn)
		{
        	display_board(board);
			move = get_move(&last_turn, board);
			if (update_board(board, move) == ERROR)
				return (free(strategy), free_board(board), ERROR);
		}
		if (board->num_heaps == 0)
            	break ;
    }

	if (last_turn)
		ft_printf("You are the winner! Congratulations!\n");
	else if (!last_turn)
		ft_printf("You lose!\n");

	free_board(board);
	free(strategy);

    return (SUCCESS);
}
