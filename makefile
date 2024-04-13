CXX = g++
CXXFLAGS = -std=c++11 -Wall

LP: lexical_parser.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: cleanLP
cleanLP:
	rm -f LP

.PHONY: runLP
runLP: LP
	./LP
