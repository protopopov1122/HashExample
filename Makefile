CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic
TEST_FACTOR=1.5

ifeq ($(FNV), ON)
	FNVDEFS=-DUSE_FNV
else
	FNVDEFS=
endif

ifeq ($(DYNAMIC), ON)
	DEFS=$(FNVDEFS) -DUSE_DYNAMIC
else
	DEFS=$(FNVDEFS)
endif

all: hash

test: hash
	./test.sh ./hash $(TEST_FACTOR)

hash: src/*.cpp
	$(CXX) $(CXXFLAGS) $(DEFS) src/*.cpp -o hash

.PHONY: all test
