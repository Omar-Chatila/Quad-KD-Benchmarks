CC := g++
CFLAGS := -O3 --std=c++23
TESTS := Tests.cpp QuadTreeTest.cpp KDTreeTests.cpp UtilTest.cpp
SOURCES := ../src/KDTreeEfficient.cpp ../src/SortKDTree.cpp ../src/QuadTree.cpp ../src/PointRegionQuadTree.cpp
HEADERS := ../include/Util.h
TARGET := tests

all: $(TARGET)

$(TARGET): $(TESTS) $(SOURCES) $(HEADERS)
	$(CC) $(TESTS) $(SOURCES) -I../include $(CFLAGS) -o $(TARGET)

.PHONY: clean

clean:
	rm -f $(TARGET)