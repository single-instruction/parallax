# Flex tool
FLEX = flex
FLEX_SRC = src/compiler/language.l
FLEX_OUT = src/compiler/lex.yy.c

# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wno-unused-function
LDFLAGS = -lfl

# Target executable
TARGET = parallax
SRC = src/compiler/lexer.cpp

# Build target
$(TARGET): $(FLEX_OUT) $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Flex step
$(FLEX_OUT): $(FLEX_SRC)
	$(FLEX) -o $(FLEX_OUT) $(FLEX_SRC)

# Run target
run: $(TARGET)
	./$(TARGET)

# Clean up generated files
clean:
	rm -f $(TARGET) $(FLEX_OUT)

.PHONY: clean run
