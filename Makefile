### CONFIG ###

# PROGRAMS
CC=gcc
RM=rm -rf
MKDIR=mkdir -p
ASTYLE=astyle
UPPERCASE=tr '[:lower:]' '[:upper:]'

# DIRECTORIES
OBJ_DIR=.obj
SRC_DIR=src
DEP_DIR=$(OBJ_DIR)/.dep
INCLUDE_DIR=include

# FLAGS
DEPFLAGS=-MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d
CFLAGS=-g -std=c11 -Werror -Wall -Wextra -I$(INCLUDE_DIR)
LDFLAGS=

# PROGRAM
LIB_NAME=dynarr
TEST_NAME=$(LIB_NAME)_test
TEST_ARGS=

### PHONY ###

.PHONY: all test debug clean stb format dirs

all: $(OBJ_DIR)/$(TEST_NAME)

test: all
	./$(OBJ_DIR)/$(TEST_NAME) $(TEST_ARGS)

debug: all
	@echo "not implemented yet"

format:
	$(ASTYLE) --style=linux --indent=tab --suffix=none \
		--max-code-length=80 \
		$(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*.h)

LIB_NAME_UPPERCASE:=$(shell echo $(LIB_NAME) | $(UPPERCASE))

# TODO: remove "// vim:" comments
stb:
	cat $(SRC_DIR)/$(LIB_NAME).h > $(OBJ_DIR)/$(LIB_NAME).h
	printf '#ifdef $(LIB_NAME_UPPERCASE)_IMPLEMENTATION\n\n' \
		>> $(OBJ_DIR)/$(LIB_NAME).h
	tail -n+3 $(SRC_DIR)/$(LIB_NAME).c >> $(OBJ_DIR)/$(LIB_NAME).h
	printf '\n#endif' >> $(OBJ_DIR)/$(LIB_NAME).h


clean:
	$(RM) $(OBJS) $(DEPS) $(OBJ_DIR)/$(TEST_NAME)

### RULES ###

SRCS=$(SRC_DIR)/$(LIB_NAME).c $(SRC_DIR)/$(LIB_NAME)_test.c
OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o, $(SRCS))
DEPS=$(patsubst $(SRC_DIR)/%.c,$(DEP_DIR)/%.d, $(SRCS))

dirs: ; @$(MKDIR) $(OBJ_DIR) $(DEP_DIR)

$(OBJ_DIR)/$(TEST_NAME): $(OBJS)
	$(CC) $(CFLAGS) $(DEPFLAGS) $(LDFLAGS) -o $(OBJ_DIR)/$(TEST_NAME) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEP_DIR)/%.d | dirs
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# include dependency files without failing on missing ones
$(DEPS):
include $(wildcard $(DEPS))

# vim: ft=make noet tw=80
