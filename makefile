# 项目名称
PROJECT_NAME = main

# 编译器
CC = g++

# 编译器标志
CFLAGS = -std=c++17 -Wall -Wextra -pedantic -finstrument-functions

# 链接器标志
LDFLAGS = 

# 源文件
SOURCES = main.cpp lexical_parser.cpp syntax_parser.cpp

# 头文件目录
INCLUDE_DIRS = -Iinclude

# 可执行文件目录
BIN_DIR = bin

# 目标文件
OBJECTS = $(SOURCES:.cpp=.o)

# 可执行文件
EXECUTABLE = $(PROJECT_NAME)

.PHONY: all clean run

# 默认目标：编译可执行文件
all: $(BIN_DIR)/$(EXECUTABLE)

# 编译可执行文件
$(BIN_DIR)/$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

# 编译目标文件
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# 运行可执行文件
run: $(BIN_DIR)/$(EXECUTABLE)
	$(BIN_DIR)/$(EXECUTABLE)

# 清理构建文件
clean:
	rm -rf $(OBJECTS) $(BIN_DIR)/$(EXECUTABLE)

# 创建可执行文件目录
$(BIN_DIR):
	mkdir -p $@

# 调试目标
debug: $(BIN_DIR)/$(EXECUTABLE)
	gdb --args $(BIN_DIR)/$(EXECUTABLE)

