#include "marketcalc_test_common.h"

#include <gtest/gtest.h>


TEST(calculateMarketTotal, RealState) {
  BacktrackState state = makeRealState();

  int expected = 1;
  int actual = calculateMarketTotal(state);

  EXPECT_EQ(expected, actual);
}

TEST(calculateMarketTotal, 2CityBestPlacementInitially) {
  /*
  OFFFFO
  OCWWCO
  OFMMFO
  sexy double level 8 market
  */
  vector<vector<TileState>> map = {
    {T(0, EMPTY), T(0, RESOURCE), T(0, RESOURCE), T(0, RESOURCE), T(0, RESOURCE), T(0, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, BUILDING), T(0, BUILDING), T(0, CITY), T(0, EMPTY)},
    {T(0, EMPTY), T(0, RESOURCE), T(0, MARKET), T(0, MARKET), T(0, RESOURCE), T(0, EMPTY)},
  };
  vector<Coord> cityCenters = {
    Coord{1, 1},
    Coord{1, 4},
  };
  unordered_map<int, vector<Coord>> tilesOwnedByCity;
  tilesOwnedByCity[0] = {
    Coord{0, 0}, Coord{0, 1}, Coord{0, 2},
    Coord{1, 0}, Coord{1, 1}, Coord{1, 2},
    Coord{2, 0}, Coord{2, 1}, Coord{2, 2},
  };
  tilesOwnedByCity[1] = {
    Coord{0, 3}, Coord{0, 4}, Coord{0, 5},
    Coord{1, 3}, Coord{1, 4}, Coord{1, 5},
    Coord{2, 3}, Coord{2, 4}, Coord{2, 5},
  };
  unordered_map<int, Coord> curBuildingsInCity = {
    {0, Coord{1, 2}},
    {1, Coord{1, 3}},
  };
  unordered_set<Coord> curBuildingsSet = {
    Coord{1, 2},
    Coord{1, 3},
  };
  unordered_map<int, Coord> curMarketsInCity = {
    {0, Coord{2, 2}},
    {1, Coord{2, 3}},
  };
  unordered_set<Coord> curMarketsSet = {
    Coord{2, 2},
    Coord{2, 3},
  };
  BacktrackState state = {
    map,
    cityCenters,
    tilesOwnedByCity,

    curBuildingsInCity,
    curBuildingsSet,
    curMarketsInCity,
    curMarketsSet,
  };

  int expected = 16;
  int actual = calculateMarketTotal(state);
  EXPECT_EQ(expected, actual);
}


TEST(calculateMarketTotal, 2CityGoodPlacementFromBacktracking) {
  /*
  OOFFFO
  FC^WCO
  OWMFMO
  decent level 5 + level 4 market
  however, the initial map was empty, the buildings and markets were added during backtracking
  */
  vector<vector<TileState>> map = {
    {T(0, EMPTY), T(0, EMPTY), T(0, RESOURCE), T(0, RESOURCE), T(0, RESOURCE), T(0, EMPTY)},
    {T(0, RESOURCE), T(0, CITY), T(0, OBSTACLE), T(0, EMPTY), T(0, CITY), T(0, EMPTY)},
    {T(0, EMPTY), T(0, RESOURCE), T(0, EMPTY), T(0, RESOURCE), T(0, EMPTY), T(0, EMPTY)},
  };
  vector<Coord> cityCenters = {
    Coord{1, 1},
    Coord{1, 4},
  };
  unordered_map<int, vector<Coord>> tilesOwnedByCity;
  tilesOwnedByCity[0] = {
    Coord{0, 0}, Coord{0, 1}, Coord{0, 2},
    Coord{1, 0}, Coord{1, 1}, Coord{1, 2},
    Coord{2, 0}, Coord{2, 1}, Coord{2, 2},
  };
  tilesOwnedByCity[1] = {
    Coord{0, 3}, Coord{0, 4}, Coord{0, 5},
    Coord{1, 3}, Coord{1, 4}, Coord{1, 5},
    Coord{2, 3}, Coord{2, 4}, Coord{2, 5},
  };
  unordered_map<int, Coord> curBuildingsInCity = {
    {0, Coord{2, 1}},
    {1, Coord{1, 3}},
  };
  unordered_set<Coord> curBuildingsSet = {
    Coord{2, 1},
    Coord{1, 3},
  };
  unordered_map<int, Coord> curMarketsInCity = {
    {0, Coord{2, 2}},
    {1, Coord{2, 3}},
  };
  unordered_set<Coord> curMarketsSet = {
    Coord{2, 2},
    Coord{2, 4},
  };
  BacktrackState state = {
    map,
    cityCenters,
    tilesOwnedByCity,

    curBuildingsInCity,
    curBuildingsSet,
    curMarketsInCity,
    curMarketsSet,
  };

  int expected = 9;
  int actual = calculateMarketTotal(state);
  EXPECT_EQ(expected, actual);
}



TEST(calculateMarketTotal, CapMarketLevel) {
  /*
  OFMFFO
  OCWWCO
  OFFFFO
  Singular lvl 10 market gets capped at MAX_MARKET_LEVEL (typically 8)
  */
  vector<vector<TileState>> map = {
    {T(0, EMPTY), T(0, RESOURCE), T(0, RESOURCE), T(0, RESOURCE), T(0, RESOURCE), T(0, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, RESOURCE), T(0, RESOURCE), T(0, CITY), T(0, EMPTY)},
    {T(0, EMPTY), T(0, RESOURCE), T(0, RESOURCE), T(0, RESOURCE), T(0, RESOURCE), T(0, EMPTY)},
  };
  vector<Coord> cityCenters = {
    Coord{1, 1},
    Coord{1, 4},
  };
  unordered_map<int, vector<Coord>> tilesOwnedByCity;
  tilesOwnedByCity[0] = {
    Coord{0, 0}, Coord{0, 1}, Coord{0, 2},
    Coord{1, 0}, Coord{1, 1}, Coord{1, 2},
    Coord{2, 0}, Coord{2, 1}, Coord{2, 2},
  };
  tilesOwnedByCity[1] = {
    Coord{0, 3}, Coord{0, 4}, Coord{0, 5},
    Coord{1, 3}, Coord{1, 4}, Coord{1, 5},
    Coord{2, 3}, Coord{2, 4}, Coord{2, 5},
  };
  unordered_map<int, Coord> curBuildingsInCity = {
    {0, Coord{1, 1}},
    {1, Coord{1, 3}},
  };
  unordered_set<Coord> curBuildingsSet = {
    Coord{1, 2},
    Coord{1, 3},
  };
  unordered_map<int, Coord> curMarketsInCity = {
    {0, Coord{0, 2}},
  };
  unordered_set<Coord> curMarketsSet = {
    Coord{0, 2},
  };
  BacktrackState state = {
    map,
    cityCenters,
    tilesOwnedByCity,

    curBuildingsInCity,
    curBuildingsSet,
    curMarketsInCity,
    curMarketsSet,
  };

  int expected = std::min(10, MAX_MARKET_LEVEL);
  int actual = calculateMarketTotal(state);
  EXPECT_EQ(expected, actual);
}