#include "../includes/aicu.h"
#include <fcntl.h>

t_err prepare_game(int argc, char **argv, t_board **board, int **strategy)
{

    *board = init_board();
	if (!*board)
		return (ERROR);

	if (argc == 1)
    {
    	int err = parse_file(STDIN_FILENO, *board);
    	if (err != SUCCESS)
    		return (err);
    }
	else if (argc == 2)
	{
		int	fd = open(argv[1], O_RDONLY);
		int err = parse_file(fd, *board);
    	if (err != SUCCESS)
		{
			return (err);
		}
		close(fd);
	}
	else
	{
		ft_printf("Usage: %s [map file path]\n", argv[0]);
		return (ERROR_USAGE);
	}
	*strategy = (int *)malloc(sizeof(int) * ((*board)->num_heaps));
	// free(*strategy);
	// *strategy = NULL;
	if (!*strategy)
		return (free_board(*board), ERROR);
	for (int i = 0; i < (*board)->num_heaps; i++)
	{
		if (i == 0)
			(*strategy)[i] = 1;
		else
		{
			if ((*strategy)[i - 1] == 1 && (*board)->heaps[i - 1] % 4 == 1)
				(*strategy)[i] = 0;
			else if ((*strategy)[i - 1] == 1)
				(*strategy)[i] = 1;
			if ((*strategy)[i - 1] == 0 && (*board)->heaps[i - 1] % 4 == 0)
				(*strategy)[i] = 1;
			else
				(*strategy)[i] = 0;
		}
	}
	return (SUCCESS);
}
