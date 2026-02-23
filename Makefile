NAME = philo

CC = cc

SRC_DIR = srcs
OBJ_DIR = Objects
INCLUDE_DIR = includes

SOURCES = $(SRC_DIR)/main.c \
		  $(SRC_DIR)/parsing.c \
		  $(SRC_DIR)/threads.c \
		  $(SRC_DIR)/init.c \
		  $(SRC_DIR)/routine.c \
		  $(SRC_DIR)/ft_usleep.c \
		  $(SRC_DIR)/init_destroy_fork.c \
		  $(SRC_DIR)/init_mutex.c \
		  $(SRC_DIR)/init_free_malloc.c \
		  $(SRC_DIR)/eat_sleep_think.c \

CFLAG = -Wall -Werror -Wextra

OBJ_FILES = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS_FILES = $(OBJ_FILES:%.o=%.d)

all: $(NAME)

-include $(DEPS_FILES)

$(NAME): $(OBJ_FILES)
	@$(CC) $(CFLAG) $(OBJ_FILES) -pthread -o $(NAME)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR) 
	$(CC) $(CFLAG) -I $(INCLUDE_DIR) -MMD -MP -o $@ -c $<

clean:
	rm -rf $(OBJ_DIR)

fclean:clean
	rm -rf $(NAME) 

re : fclean
	$(MAKE) all

.PHONY: fclean clean all re
