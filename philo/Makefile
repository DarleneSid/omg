NAME	=	philo

DIR_SRCS		=	srcs

DIR_SRCS_B		=	bonus

DIR_OBJS		=	objs

SRCS_NAMES		=	philo.c parsing.c process.c forks.c

OBJS_NAMES		=	${SRCS_NAMES:.c=.o}

DEPS			=	${SRCS_NAMES:.c=.d}

SRCS			=	$(addprefix $(DIR_SRCS)/,$(SRCS_NAMES))

OBJS			=	$(addprefix $(DIR_OBJS)/,$(OBJS_NAMES))

INC				=	-Iincludes

CC				=	cc

CDFLAGS 		=	-MMD -MP

CFLAGS			=	-Wall -Werror -Wextra  -pthread

all:	${NAME}

bonus:	${BONUS}

$(DIR_OBJS):
	mkdir -p $(DIR_OBJS)

$(OBJS) : $(DIR_OBJS)/%.o : $(DIR_SRCS)/%.c
	$(CC) -g3 $(CFLAGS) $(CDFLAGS) $(INC) -c $< -o $@ 

$(NAME): $(DIR_OBJS) $(OBJS) 
	$(CC) -g3 ${INC} $(OBJS) $(LIB) -o $(NAME) -lpthread
	@echo "\033[31;5mphilo\033[0m"

clean:
	rm -rf ${DIR_OBJS}

fclean:	clean
	rm -rf ${LIBFT}
	rm -rf ${NAME}
	rm -rf ${BONUS}

re:	fclean all

-include $(DEPS)

.PHONY:	all clean fclean re
