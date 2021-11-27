### CONFIG ###

# PROGRAMS
CC=gcc
RM=rm -rf
MKDIR=mkdir -p
ASTYLE=astyle

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
NAME=dynarr_test
TEST_ARGS=

### PHONY ###

.PHONY: all test debug clean stb format dirs

all: $(OBJ_DIR)/$(NAME)

test: all
	./$(OBJ_DIR)/$(NAME) $(TEST_ARGS)

debug: all
	@echo "not implemented yet"

format:
	$(ASTYLE) --style=linux --indent=tab --suffix=none \
		--max-code-length=80 \
		$(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*.h)

# TODO: remove "// vim:" comments
stb:
	cat $(SRC_DIR)/dynarr.h > $(OBJ_DIR)/dynarr.h
	printf '#ifdef DYNARR_IMPLEMENTATION\n\n' >> $(OBJ_DIR)/dynarr.h
	tail -n+3 $(SRC_DIR)/dynarr.c >> $(OBJ_DIR)/dynarr.h
	printf '\n#endif' >> $(OBJ_DIR)/dynarr.h


clean:
	$(RM) $(OBJS) $(DEPS) $(OBJ_DIR)/$(NAME)

### RULES ###

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o, $(SRCS))
DEPS=$(patsubst $(SRC_DIR)/%.c,$(DEP_DIR)/%.d, $(SRCS))

dirs: ; @$(MKDIR) $(OBJ_DIR) $(DEP_DIR)

$(OBJ_DIR)/$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(DEPFLAGS) $(LDFLAGS) -o $(OBJ_DIR)/$(NAME) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEP_DIR)/%.d | dirs
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# include dependency files without failing on missing ones
$(DEPS):
include $(wildcard $(DEPS))

# vim: ft=make noet tw=80
