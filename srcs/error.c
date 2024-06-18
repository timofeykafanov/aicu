#include <unistd.h>

#include "../includes/aicu.h"
#include "../libft/libft.h"

int error(t_err error)
{
	ft_putendl_fd("ERROR", STDERR_FILENO);

	return (error);
}
