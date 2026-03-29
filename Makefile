CC = gcc
CXX = g++
GTEST_PREFIX = /opt/homebrew/opt/googletest
GTEST_INC = $(GTEST_PREFIX)/include
GTEST_LIB = $(GTEST_PREFIX)/lib
CFLAGS = -std=c11 -Wall -Wextra -O2 -g -I.
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g -I$(GTEST_INC) -I.

TARGET = marketcalc_tests
TEST_SRC = $(wildcard tests/*_tests.cc)
SRC = $(TEST_SRC) tests/test_main.cc marketcalc.c

all: compile

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) -L$(GTEST_LIB) -lgtest -lgtest_main -pthread

marketcalc.o: marketcalc.c
	$(CC) $(CFLAGS) -c marketcalc.c -o marketcalc.o

compile: marketcalc.o
	emcc marketcalc.o -o marketcalc.js \
		-s EXPORTED_FUNCTIONS='["_malloc","_free","_main_wasm"]' \
		-s EXPORTED_RUNTIME_METHODS='["HEAP32"]' \
		-s MODULARIZE \
		-s EXPORT_ES6 \
		-s ENVIRONMENT='web'

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f marketcalc.js marketcalc.wasm marketcalc.o