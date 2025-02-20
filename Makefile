CC= cc
FLAGS= -Wall -Wextra -Werror -fsanitize=address
RM= rm -f
SRC= mandatory/ft_split.c mandatory/pipex.c mandatory/pipex_utils.c mandatory/utils.c mandatory/utils_libft.c
# Bonus Source Files
BSRC= bonus/ft_split_bonus.c \
      bonus/pipex_bonus.c \
      bonus/pipex_utils_bonus.c \
      bonus/utils_bonus.c \
      bonus/utils_libft_bonus.c

# Object Files
OBJECT= $(SRC:.c=.o)
BOBJECT= $(BSRC:.c=.o)
# Executable Name
NAME= pipex

all: $(NAME)

$(NAME): $(OBJECT)
	$(CC) $(FLAGS) $(OBJECT) -o $(NAME)

bonus/%.o: bonus/%.c include/pipex_bonus.h
	$(CC) $(FLAGS) -c $< -o $@

bonus: $(BOBJECT)
	$(CC) $(FLAGS) $(BOBJECT) -o $(NAME)

mandatory/%.o: mandatory/%.c include/pipex.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECT) $(BOBJECT)

fclean:
	$(RM) $(OBJECT) $(BOBJECT)

re: fclean all