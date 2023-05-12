#Directory folder names
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

#Compiler, Compiler flags and Includes
CC       = gcc
CFLAGS   = -Wall -fopenmp -g

#File names (no extension)
GOL_FILE_NAME = game_of_life

###################### T A R G E T S###################################
$(GOL_FILE_NAME): $(BIN_DIR) $(GOL_FILE_NAME).o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(GOL_FILE_NAME) $(OBJ_DIR)/$(GOL_FILE_NAME).o
#######################################################################

##################### O B J E C T - F I L E S #########################
$(GOL_FILE_NAME).o: $(SRC_DIR)/$(GOL_FILE_NAME).c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $^ -o $(OBJ_DIR)/$@
#######################################################################

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) *.o perf.* 