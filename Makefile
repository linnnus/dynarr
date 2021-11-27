### CONFIG ###

LIB=dynarr

# PROGRAMS
CC=gcc
RM=rm -rf
MKDIR=mkdir -p
ASTYLE=astyle

# DIRECTORIES
OBJ_DIR=.obj
SRC_DIR=src

# FLAGS
CFLAGS=-g -std=c11 -Werror -Wall -Wextra -I$(INCLUDE_DIR)
LDFLAGS=

### PHONY ###

.PHONY: test clean format dirs

test: $(OBJ_DIR)/$(LIB)_test
	$(OBJ_DIR)/$(LIB)_test

clean:
	$(RM) $(OBJ_DIR)/$(LIB)_test

format:
	$(ASTYLE) \
		--style=linux \
		--indent=tab \
		--suffix=none \
		--max-code-length=80 \
		$(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*.h)

### RULES ###

dirs: ; @$(MKDIR) $(OBJ_DIR)

$(OBJ_DIR)/$(LIB)_test: $(SRC_DIR)/$(LIB)_test.c $(SRC_DIR)/$(LIB).h
	$(CC) $(CFLAGS) -c $< -o $@
