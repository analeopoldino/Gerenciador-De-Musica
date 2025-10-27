# Executável principal
TARGET = music_manager

# Executável de testes
TEST_TARGET = run_tests

# Compiladores
CXX = g++
CC = gcc

# Diretórios
SRC_DIR = src
INCLUDE_DIR = include
SQLITE_DIR = library-sqlite3
BUILD_DIR = build
TEST_DIR = tests

# Arquivos fonte principais
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
MAIN_FILE = main.cpp
SQLITE_SRC = $(shell find $(SQLITE_DIR) -name 'sqlite3.c')
HDRS = $(wildcard $(INCLUDE_DIR)/*.h) $(wildcard $(INCLUDE_DIR)/*.hpp)
SQLITE_HDRS = $(shell find $(SQLITE_DIR) -name '*.h')

# Arquivos de teste
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)

# Objetos normais
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))
OBJS += $(BUILD_DIR)/main.o
OBJS += $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SQLITE_SRC)))

# Objetos de teste (sem main.o)
TEST_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.test.o, $(SRC_FILES))
TEST_OBJS += $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.test.o, $(TEST_SRCS))
TEST_OBJS += $(patsubst %.c, $(BUILD_DIR)/%.test.o, $(notdir $(SQLITE_SRC)))

# Flags
INCLUDES = -I$(INCLUDE_DIR) -I$(SQLITE_DIR)
CXXFLAGS = -std=c++17 -Wall -Wextra $(INCLUDES)
CFLAGS = -Wall $(INCLUDES)
LDFLAGS = -ldl -lpthread
GTEST_FLAGS = -lgtest_main -lgtest -pthread

# Regras
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDRS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: $(MAIN_FILE) $(HDRS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SQLITE_SRC))): $(SQLITE_SRC) $(SQLITE_HDRS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILD_DIR)/%.test.o: $(SRC_DIR)/%.cpp $(HDRS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.test.o: $(TEST_DIR)/%.cpp $(HDRS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(patsubst %.c, $(BUILD_DIR)/%.test.o, $(notdir $(SQLITE_SRC))): $(SQLITE_SRC) $(SQLITE_HDRS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(GTEST_FLAGS)

clean:
	@rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET)

.PHONY: all clean test