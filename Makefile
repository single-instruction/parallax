# TODO:// Use CMake instead of Make ?? Modular build file organization.
CXX = clang++
LLVM_CONFIG = /home/bhaskar/dev/llvm-project/build/bin/llvm-config
CXXFLAGS = -std=c++17 -g -O3 $(shell $(LLVM_CONFIG) --cxxflags) -I$(INCLUDE_DIR)
LDFLAGS = $(shell $(LLVM_CONFIG) --ldflags --system-libs --libs core) -lfl

# Directory variables
INCLUDE_DIR = include
SRC_DIR = src/compiler
BUILD_DIR = build
DOC_DIR = doc

# Source files
SRCS = $(SRC_DIR)/parallax.cpp $(SRC_DIR)/AST.cpp
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Flex and Bison files
FLEX_SRC = $(SRC_DIR)/lexer.l
BISON_SRC = $(SRC_DIR)/parser.y

# Flex and Bison generated files
FLEX_OUT = $(BUILD_DIR)/lexer.cpp
BISON_OUT_CPP = $(BUILD_DIR)/parser.cpp
BISON_OUT_HPP = $(BUILD_DIR)/parser.hpp

# Target executable
TARGET = $(BUILD_DIR)/plx

.PHONY: all clean doc

all: $(TARGET)

$(TARGET): $(OBJS) $(FLEX_OUT) $(BISON_OUT_C)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Flex target
flex: $(FLEX_OUT)

$(FLEX_OUT): $(FLEX_SRC) | $(BUILD_DIR)
	flex --c++ -o $@ $<

# Bison target
bison: $(BISON_OUT_CPP)

$(BISON_OUT_CPP) $(BISON_OUT_HPP): $(BISON_SRC) | $(BUILD_DIR)
	bison -d -o $(BISON_OUT_CPP) $<

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(DOC_DIR)

# Header dependencies
$(BUILD_DIR)/parallax.o: $(INCLUDE_DIR)/AST.h $(INCLUDE_DIR)/CodeGenContext.h $(BISON_OUT_H)
$(BUILD_DIR)/AST.o: $(INCLUDE_DIR)/AST.h $(INCLUDE_DIR)/CodeGenContext.h
$(FLEX_OUT): $(BISON_OUT_H)

# Doxygen
doc:
	doxygen Doxyfile

# Clean and generate documentation
fulldoc: clean doc
