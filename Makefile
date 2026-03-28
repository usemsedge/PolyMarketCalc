CXX = g++
GTEST_PREFIX = /opt/homebrew/opt/googletest
GTEST_INC = $(GTEST_PREFIX)/include
GTEST_LIB = $(GTEST_PREFIX)/lib
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g -I$(GTEST_INC) -I. -Itests

TARGET = marketcalc_tests
TEST_SRC = $(wildcard tests/*_tests.cc)
SRC = $(TEST_SRC) tests/test_main.cc marketcalc.cc

all: compile

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) -L$(GTEST_LIB) -lgtest -lgtest_main -pthread

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f marketcalc.js marketcalc.wasm

run: compile
	node server.js

compile:
	emcc marketcalc.cc -o marketcalc.js \
		-s EXPORTED_FUNCTIONS='["_malloc","_free","_main_wasm"]' \
		-s EXPORTED_RUNTIME_METHODS='["HEAP32"]' \
		-s MODULARIZE \
		-s EXPORT_ES6 \
		-s ENVIRONMENT='web'