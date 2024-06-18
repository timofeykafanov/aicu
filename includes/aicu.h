#ifndef AICU_H
# define AICU_H

# include "../libft/libft.h"

#define DEFAULT_HEAP_AMOUNT 16

typedef enum e_err {
    SUCCESS = 0,
    ERROR = 1,
    ERROR_MALLOC = 2,
    ERROR_INVALID_MAP = 3,
    ERROR_USAGE = 4,
}    t_err;

# define STDIN 0

# include <stdlib.h>
# include <fcntl.h>

typedef struct s_board
{
    int *heaps;
    int num_heaps;
    int _heap_size;
}   t_board;

// ai.c
int get_ai_move(t_board *board, int *last_turn, int *strategy);

// error.c
int error(t_err error);

// parse_file.c
t_err parse_file(int fd, t_board *board);

// prepare_game.c
t_err prepare_game(int argc, char **argv, t_board **board, int **strategy);

// board.c
t_err        add_heap(t_board *board, int heap);
void         free_board(t_board *board);
t_board      *init_board();
t_err	     update_board(t_board *board, int items_removed);

#endif
