CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic

ifeq ($(FNV), ON)
	DEFS=-DUSE_FNV
else
	DEFS=
endif

all: hash

test: hash
	./test.sh ./hash

hash: src/*.cpp
	$(CXX) $(CXXFLAGS) $(DEFS) src/*.cpp -o hash

.PHONY: all test
