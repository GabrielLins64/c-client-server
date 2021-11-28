# Client / Server Project

PROJECT_NAME	=	client
MAIN_NAME		= client

C_SOURCE		= $(wildcard ./src/*.c)
H_SOURCE		= $(wildcard ./src/*.h)

OBJ					= $(subst .c,.o,$(subst src,objects,$(C_SOURCE)))

CC					= gcc
CC_FLAGS		= -c			\
						  -W			\
						  -ansi		\
						  -pedantic

# Command to clean
RM = rm -rf

#############################
## Compilation and linking ##
#############################

all: objFolder $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJ)
		@ echo 'Building binary using GCC linker: $@'
		$(CC) $^ -o $@
		@ echo 'Finished building binary: $@'
		@ echo ' '

./objects/%.o: ./src/%.c ./src/%.h
		@ echo 'Building target using GCC compiler: $<'
		$(CC) $< $(CC_FLAGS) -o $@
		@ echo ' '

./objects/$(MAIN_NAME).o: ./src/$(MAIN_NAME).c $(H_SOURCE)
		@ echo 'Building target using GCC compiler: $<'
		$(CC) $< $(CC_FLAGS) -o $@
		@ echo ' '

objFolder:
		@ mkdir -p objects

clean:
		@ $(RM) ./objects/*.o $(PROJECT_NAME) *~
		@ rmdir objects

.PHONY: all clean
