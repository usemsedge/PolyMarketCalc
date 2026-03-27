#include "marketcalc_test_common.h"

#include <gtest/gtest.h>


// Exclusively for testing backtracking


/*
Creates a very simple map with 1 city 0 buildings 0 markets
  |____
  | ∧ |
  | 0⊞|
  |  ■|
Expected result (one of two possible, market level 2):  
  |____
  | ∧ |
  | 0⊞|
  |⊕*■| 
*/
inline BacktrackState makeTinyState0Building0Market() {
  static vector<vector<TileState>> map;
  static vector<Coord> cityCenters;
  static unordered_map<int, vector<Coord>> tilesOwnedByCity;

  map = {
    {T(1, EMPTY), T(1, OBSTACLE), T(1, EMPTY)},
    {T(1, EMPTY), T(1, CITY), T(1, USED_RESOURCE)},
    {T(1, EMPTY), T(1, EMPTY), T(1, RESOURCE)},
  };

  cityCenters = {
    Coord{1, 1},
  };

  tilesOwnedByCity[0] = {
    Coord{0, 0}, Coord{0, 1}, Coord{0, 2},
    Coord{1, 0}, Coord{1, 1}, Coord{1, 2},
    Coord{2, 0}, Coord{2, 1}, Coord{2, 2},
  };

  unordered_set<Coord> curBuildingsSet = {};

  unordered_map<int, Coord> curBuildingsInCity = {};

  unordered_set<Coord> curMarketsSet = {};

  unordered_map<int, Coord> curMarketsInCity = {};

  return BacktrackState{
    map,
    cityCenters,
    tilesOwnedByCity,

    curBuildingsInCity,
    curBuildingsSet,
    curMarketsInCity,
    curMarketsSet,
  };
}



/*
Creates a very simple map with 1 city 1 buildings 0 markets
  |____
  | ∧ |
  | 0⊞|
  |  *|
Expected result (only one possible, market level 1):  
  |____
  | ∧ |
  | 0⊞|
  | ⊕*|
*/
inline BacktrackState makeTinyState1Building0Market() {
  static vector<vector<TileState>> map;
  static vector<Coord> cityCenters;
  static unordered_map<int, vector<Coord>> tilesOwnedByCity;

  map = {
    {T(1, EMPTY), T(1, OBSTACLE), T(1, EMPTY)},
    {T(1, EMPTY), T(1, CITY), T(1, USED_RESOURCE)},
    {T(1, EMPTY), T(1, EMPTY), T(1, BUILDING)},
  };

  cityCenters = {
    Coord{1, 1},
  };

  tilesOwnedByCity[0] = {
    Coord{0, 0}, Coord{0, 1}, Coord{0, 2},
    Coord{1, 0}, Coord{1, 1}, Coord{1, 2},
    Coord{2, 0}, Coord{2, 1}, Coord{2, 2},
  };

  unordered_set<Coord> curBuildingsSet = {
    Coord{2, 2},
  };

  unordered_map<int, Coord> curBuildingsInCity = {
    {1, Coord{2, 2}},
  };

  unordered_set<Coord> curMarketsSet = {};

  unordered_map<int, Coord> curMarketsInCity = {};

  return BacktrackState{
    map,
    cityCenters,
    tilesOwnedByCity,

    curBuildingsInCity,
    curBuildingsSet,
    curMarketsInCity,
    curMarketsSet,
  };
}


/*
Creates a very simple map with 1 city 1 buildings 1 market
  |____
  | ∧ |
  | 0⊞|
  | ⊕*|
Expected result (do nothing)
  |____
  | ∧ |
  | 0⊞|
  | ⊕*| 
*/
inline BacktrackState makeTinyState1Building1Market() {
  static vector<vector<TileState>> map;
  static vector<Coord> cityCenters;
  static unordered_map<int, vector<Coord>> tilesOwnedByCity;

  map = {
    {T(1, EMPTY), T(1, OBSTACLE), T(1, EMPTY)},
    {T(1, EMPTY), T(1, CITY), T(1, USED_RESOURCE)},
    {T(1, EMPTY), T(1, MARKET), T(1, BUILDING)},
  };

  cityCenters = {
    Coord{1, 1},
  };

  tilesOwnedByCity[0] = {
    Coord{0, 0}, Coord{0, 1}, Coord{0, 2},
    Coord{1, 0}, Coord{1, 1}, Coord{1, 2},
    Coord{2, 0}, Coord{2, 1}, Coord{2, 2},
  };

  unordered_set<Coord> curBuildingsSet = {
    Coord{2, 2},
  };

  unordered_map<int, Coord> curBuildingsInCity = {
    {1, Coord{2, 2}},
  };

  unordered_set<Coord> curMarketsSet = {
    Coord{2, 1},
  };

  unordered_map<int, Coord> curMarketsInCity = {
    {1, Coord{2, 1}},
  };

  return BacktrackState{
    map,
    cityCenters,
    tilesOwnedByCity,

    curBuildingsInCity,
    curBuildingsSet,
    curMarketsInCity,
    curMarketsSet,
  };
}


TEST(backtrackPlacements, Tiny1Building1Market_BaseCase_DoNothing) {
  BacktrackState state = makeTinyState1Building1Market();
  vector<vector<TileState>> expectedMap = state.map;

  // Directly testing the base case
  BacktrackResult resultState = backtrackPlacements(state, 1, false);

  EXPECT_EQ(expectedMap, resultState.bestLayout);
  EXPECT_EQ(1, resultState.bestMarketTotal);
}

TEST(backtrackPlacements, Tiny1Building0Market_1Deep_PlaceMarket) {
  BacktrackState state = makeTinyState1Building0Market();
  vector<vector<TileState>> expectedMap = state.map;
  expectedMap[2][1] = T(1, MARKET); // Should place market here

  //Testing the case where all the building recursion is done
  BacktrackResult resultState = backtrackPlacements(state, 0, false);

  EXPECT_EQ(expectedMap, resultState.bestLayout);
  EXPECT_EQ(1, resultState.bestMarketTotal);
}

TEST(backtrackPlacements, Tiny0Building0Market_2Deep_PlaceMarket) {
  BacktrackState state = makeTinyState0Building0Market();
  vector<vector<TileState>> expectedMap = state.map;
  expectedMap[2][1] = T(1, BUILDING); // Should place building here

  vector<vector<TileState>> expectedMap1 = expectedMap, expectedMap2 = expectedMap;
  expectedMap1[2][0] = T(1, MARKET); // Choice 1: place market here
  expectedMap2[1][0] = T(1, MARKET); // Choice 2: place market here


  // Testing full recursion through both cases
  BacktrackResult resultState = backtrackPlacements(state, 0, true);

  EXPECT_EQ(2, resultState.bestMarketTotal);
  EXPECT_TRUE(expectedMap1 == resultState.bestLayout || expectedMap2 == resultState.bestLayout);
}