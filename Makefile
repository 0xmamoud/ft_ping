NAME		= ft_ping

# Compiler and flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g3
INCLUDES	= -I./includes

# Directories
SRC_DIR		= src
OBJ_DIR		= obj
INC_DIR		= includes

SRCS		= main.c ping.c network.c icmp.c stats.c

# Object files
OBJS		= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

# Colors for output
GREEN		= \033[0;32m
RED			= \033[0;31m
RESET		= \033[0m

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN) $(NAME) created successfully$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@echo "$(RED) Clean complete$(RESET)"

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(RED) Clean complete$(RESET)"

re: fclean all

.PHONY: all clean fclean re
