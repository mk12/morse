# Compiler
CC := clang
OFLAGS := -O3
CFLAGS := -std=c11 -Weverything -pedantic -Wno-gnu-binary-literal -Wno-vla
LDFLAGS := -lpthread

# Project
NAME := morse
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

# Files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
EXEC := $(BIN_DIR)/$(NAME)

# Dependencies
DEPFLAGS = -MT $@ -MMD -MP -MF $(OBJ_DIR)/$*.d

# Targets
.PHONY: all release debug prep clean

all: release

release: CFLAGS += $(OFLAGS) -DNDEBUG
release: LDFLAGS += $(OFLAGS)
release: prep $(EXEC)
release:
	@echo "Target release is up to date"

debug: CFLAGS += -g
debug: prep $(EXEC)
debug:
	@echo "Target debug is up to date"

prep:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

clean:
	rm -f $(OBJS) $(DEPS) $(EXEC)

$(EXEC): $(OBJS)
	@echo "\033[0;31mLinking executable $(NAME)\033[0m"
	@$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "\033[0;32mCC\033[0m $<"
	@$(CC) -c $< -o $@ $(CFLAGS) $(DEPFLAGS)

-include $(DEPS)
