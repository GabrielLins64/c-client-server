# Client / Server Project

PROJECT_NAME	= server.out
MAIN_NAME		= server.out

OBJ_DIR			= objects
SRC_DIR			= src
INC_DIR			= include

C_SOURCE		= $(wildcard ./$(SRC_DIR)/*.c)
H_SOURCE		= $(wildcard ./$(INC_DIR)/*.h)

OBJ				= $(subst .c,.o,$(subst src,$(OBJ_DIR),$(C_SOURCE)))

CC				= gcc
CC_FLAGS		= -c	\
				-W		\
				-ansi	\
				-pedantic

# Command to clean
RM = rm -rf

#############################
## Compilation and linking ##
#############################

all: objFolder $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJ)
		@ echo ' '
		@ echo 'Building binary "$@" using GCC linker...'
		$(CC) $^ -o $@
		@ echo 'Binary "$@" successfully built!'

$(OBJ_DIR)/%.o: ./$(SRC_DIR)/%.c
		@ echo -n 'Building target using GCC compiler: $<... '
		@ $(CC) $(CC_FLAGS) -I $(INC_DIR) $< -o $@
		@ echo 'Success!'

objFolder:
		@ mkdir -p objects

clean:
		@ $(RM) ./objects/*.o $(PROJECT_NAME) *~
		@ rmdir objects

.PHONY: all clean
