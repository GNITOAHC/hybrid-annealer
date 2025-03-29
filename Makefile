.DEFAULT_GOAL := all # Set default target to all

CC = g++
CFLAGS = -Wall -O3 -std=c++20

TARGET = bin/main

# Define paths
SRC_DIR = src
BUILD_DIR = build

# Program sources and objects
SRCS = $(shell find $(SRC_DIR) -name '*.cc') # Automatically find all .cc files in src and its subdirectories
OBJS = $(SRCS:$(SRC_DIR)/%.cc=$(BUILD_DIR)/%.o) # Convert .cc files to .o files in build directory

# Automatically find all header directories in src and its subdirectories
HEADER_DIRS = $(shell find $(SRC_DIR) -type d -print)
INCLUDES = $(addprefix -I, $(HEADER_DIRS))

# Default target
all: $(TARGET)

# ===== Main target rule
$(TARGET): $(OBJS)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) -L$(BUILD_DIR)

# Generic rule for .cc to .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ $(DEFS)

clean:
	$(RM) -r $(BUILD_DIR) $(TARGET)

.PHONY: all clean
