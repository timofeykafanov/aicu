#include <errno.h>
#include <unistd.h>

#include "../includes/aicu.h"
#include "../libft/libft.h"

t_err parse_file(int fd, t_board *board)
{
	char	*line;

	line = get_next_line(fd);
	if (!line)
		return (free_board(board), error(ERROR_MALLOC));

	if (line[0] == '\n')
		return (free(line), error(ERROR_INVALID_MAP));

    while (line)
    {
		for (int i = 0; line[i]; ++i)
		{
			if (ft_isdigit(line[i]) || line[i] == '\n')
    			continue;

			return (free(line), error(ERROR_INVALID_MAP));
    	}

		if (line[0] == '\n')
		{
			free(line);
			break ;
		}
		if (ft_strlen(line) > 6)
			return (free(line), error(ERROR_INVALID_MAP));
		int heap = ft_atoi(line);
		if (heap > 10000 || heap < 1)
			return (free(line), error(ERROR_INVALID_MAP));

		if (add_heap(board, heap) != SUCCESS)
			return (free(line), error(ERROR_MALLOC));

		free(line);
		line = get_next_line(fd);
		// free(line);
		// line = NULL;
		if (!line)
			return (free_board(board), error(ERROR_MALLOC));
	}

	return (SUCCESS);
}
