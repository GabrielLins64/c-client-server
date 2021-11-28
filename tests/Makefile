# Client / Server Project

PROJECT_NAME	= tests.out
MAIN_NAME		= tests.out

OBJ_DIR			= objects
SRC_DIR			= src
INC_DIR			= include

C_SOURCE		= $(wildcard ./$(SRC_DIR)/*.cpp)
H_SOURCE		= $(wildcard ./$(INC_DIR)/*.hpp)

OBJ				= $(subst .cpp,.o,$(subst src,$(OBJ_DIR),$(C_SOURCE)))

CC				= g++
CC_FLAGS		= -c

# Command to clean
RM = rm -rf

#############################
## Compilation and linking ##
#############################

all: objFolder $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJ)
		@ echo ' '
		@ echo 'Building binary "$@" using G++ linker...'
		$(CC) $^ -o $@
		@ echo 'Binary "$@" successfully built!'

$(OBJ_DIR)/%.o: ./$(SRC_DIR)/%.cpp
		@ echo -n 'Building target using G++ compiler: $<... '
		@ $(CC) $(CC_FLAGS) -I $(INC_DIR) $< -o $@
		@ echo 'Success!'

objFolder:
		@ mkdir -p objects

clean:
		@ $(RM) ./objects/*.o $(PROJECT_NAME) *~
		@ rmdir objects

.PHONY: all clean
