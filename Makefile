################################################################################
######                             PROPERTIES                             ######
################################################################################

CC			= cc
RM			= rm -rf
CFLAGS		= -Wall -Wextra -Werror -MD -MP -g -I $(INCLUDES)
MAKEFLAGS	= -j$(nproc) --no-print-directory

NAME		= alum1
NAME_B		= alum1_bonus

SRCSDIR		= srcs
INCLUDES	= includes

SRCS		= \
			$(SRCSDIR)/aicu.c \
			$(SRCSDIR)/ai.c \
			$(SRCSDIR)/board.c \
			$(SRCSDIR)/error.c \
			$(SRCSDIR)/parse_file.c \
			$(SRCSDIR)/prepare_game.c \

SRCS_B		= \
			$(SRCSDIR)/aicu_bonus.c \
			$(SRCSDIR)/ai.c \
			$(SRCSDIR)/board.c \
			$(SRCSDIR)/error.c \
			$(SRCSDIR)/parse_file.c \
			$(SRCSDIR)/prepare_game.c \

OBJSDIR		= objs
OBJS		= $(addprefix $(OBJSDIR)/, $(SRCS:.c=.o))
DEPS		= $(addprefix $(OBJSDIR)/, $(SRCS:.c=.d))

OBJSDIR_B	= objs_bonus
OBJS_B		= $(addprefix $(OBJSDIR_B)/, $(SRCS_B:.c=.o))
DEPS_B		= $(addprefix $(OBJSDIR_B)/, $(SRCS_B:.c=.d))

################################################################################
######                               LIBFT                                ######
################################################################################

LIBDIR		= ./libft
LIBFT		= ${LIBDIR}/libft.a

################################################################################
######                               RULES                                ######
################################################################################

all		: $(NAME)

bonus	: $(NAME_B)

$(NAME)	: ${OBJS}
		$(MAKE) -C ${LIBDIR} all
		$(CC) ${CFLAGS} -o $@ $^ -L. ${LIBFT}

$(NAME_B)	: ${OBJS_B}
		$(MAKE) -C ${LIBDIR} all
		$(CC) ${CFLAGS} -o $@ $^ -L. ${LIBFT} raylib/lib/libraylib.a -lm

${OBJSDIR}/%.o	: %.c
		@mkdir -p $(dir $@)
		${CC} ${CFLAGS} -c $< -o $@

${OBJSDIR_B}/%.o	: %.c
		@mkdir -p $(dir $@)
		${CC} ${CFLAGS} -Iraylib/include -c $< -o $@

clean	:
		$(MAKE) -C ${LIBDIR} clean
		$(RM) $(OBJSDIR) $(OBJSDIR_B)

fclean	:
		$(MAKE) -C ${LIBDIR} fclean
		$(RM) $(OBJSDIR) $(NAME) $(OBJSDIR_B) $(NAME_B)

re		:
		$(MAKE) -C ${LIBDIR} fclean
		$(RM) $(OBJSDIR) $(NAME)
		$(MAKE) all

reb		:
		$(MAKE) -C ${LIBDIR} fclean
		$(RM) $(OBJSDIR_B) $(NAME_B)
		$(MAKE) bonus

-include $(DEPS)

.PHONY	: all clean fclean re bonus reb
