CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = infix2postfix
SRCS = infix2postfix.cpp

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(TARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET)
