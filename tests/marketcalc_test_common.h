#pragma once

#include "marketcalc.h"
#include "chrono"

#include <vector>

using std::vector;

BacktrackState defaultState = {NULL, 0, 0, NULL, 0, NULL, NULL, NULL};

/*
Caller responsible for freeing returned map
*/
int32_t* makeNoSpecialMap() {
  int32_t size = 8 * 6;
  int32_t* map = (int32_t*)malloc(size * sizeof(TileState));
  if (!map) {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
  }
  int32_t temp[] = {
    -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    -1, EMPTY, -1, CITY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, CITY, -1, EMPTY,
    -1, EMPTY, -1, CITY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1,EMPTY,
    -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY,
  };

  for (int i = 0; i < size * sizeof(TileState) / sizeof(int32_t); i++) {
    map[i] = temp[i];
  }
  return map;
}

/*
Caller responsible for freeing returned array
*/
int32_t* makeNoSpecialCityCenters() {
  int32_t size = 3;
  int32_t* cityCenters = (int32_t*)malloc(size * sizeof(Coord));
  if (!cityCenters) {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
  }
  int32_t temp[] = {
    1, 1,
    3, 4,
    4, 1,
  };

  for (int i = 0; i < size * sizeof(Coord) / sizeof(int32_t); i++) {
    cityCenters[i] = temp[i];
  }
  return cityCenters;

}

/*
Caller responsi1ble for freeing returned map
 // Create a simple map and state for testing 
  // Contains a mix of everything
  // 3 cities, 2 buildings, 1 market
  // Market total 1

  |_________
  |  ∧   |
  | 0*  ■|
  | ⊞⊞   |
  | ⊞ ∧1 |
  | 2⊕   |
  | ⊞*   |
  |      |
  |      |
   Legend:
   ∧ = OBSTACLE
   0, 1, 2 = CITY
   ⊞ = USED_RESOURCE
   ■ = RESOURCE
   ⊕ = MARKET
   * = BUILDING
   (empty) = EMPTY
  */
BacktrackState makeRealState() {
  int32_t rows = 8;
  int32_t cols = 6;
  int32_t numCities = 3;
  int32_t size = rows * cols;
  int32_t* map = (int32_t*)malloc(size * sizeof(TileState));
  if (!map) {
    fprintf(stderr, "Memory allocation failed\n");
    return defaultState;
  }
  int32_t temp[] = {
    0, EMPTY, 0, EMPTY, 0, OBSTACLE, 0, EMPTY, -1, EMPTY, -1, EMPTY,
    0, EMPTY, 0, CITY, 0, BUILDING, 0, EMPTY, 1, EMPTY, 1, RESOURCE,
    0, EMPTY, 0, USED_RESOURCE, 0, USED_RESOURCE, 1, EMPTY, 1, EMPTY, 1, EMPTY,
    2, EMPTY, 2, USED_RESOURCE, 2, EMPTY, 1, OBSTACLE, 1, CITY, 1, EMPTY,
    2, EMPTY, 2, CITY, 2, MARKET, 1, EMPTY, 1, EMPTY, 1, EMPTY,
    2, EMPTY, 2, USED_RESOURCE, 2, BUILDING, 1, EMPTY, 1, EMPTY, 1, EMPTY,
    2, EMPTY, 2, EMPTY, 2, EMPTY, 2, EMPTY, -1, EMPTY, -1, EMPTY,
    -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY,
  };

  int32_t* cityCenters = makeNoSpecialCityCenters();

  int32_t sizeTilesOwned = 3 * MAX_TILES_PER_CITY;
  int32_t* tilesOwnedByCity = (int32_t*)malloc(sizeTilesOwned * sizeof(Coord));
  if (!tilesOwnedByCity) {
    fprintf(stderr, "Memory allocation failed\n");
    return defaultState;
  }

  int32_t row0[] = {
    0, 0, 0, 1, 0, 2, 0, 3,
    1, 0, 1, 1, 1, 2, 1, 3,
    2, 0, 2, 1, 2, 2
  };
  for (int i = 0; i < 11 * 2; i++) {
    tilesOwnedByCity[i] = row0[i];
  }

  int32_t row1[] = {
          1, 4, 1, 5,
    2, 3, 2, 4, 2, 5,
    3, 3, 3, 4, 3, 5,
    4, 3, 4, 4, 4, 5,
    5, 3, 5, 4, 5, 5
  };
  for (int i = 0; i < 14 * 2; i++) {
    tilesOwnedByCity[MAX_TILES_PER_CITY * 2 + i] = row1[i];
  }

  int32_t row2[] = {
    3, 0, 3, 1, 3, 2,
    4, 0, 4, 1, 4, 2,
    5, 0, 5, 1, 5, 2,
    6, 0, 6, 1, 6, 2, 6, 3
  };
  for (int i = 0; i < 13 * 2; i++) {
    tilesOwnedByCity[2 * 2 * MAX_TILES_PER_CITY + i] = row2[i];
  }

  // 2 buildings and 1market
  Coord* curBuildingsInCity = (Coord*)malloc(numCities * sizeof(Coord));
  if (!curBuildingsInCity) {
    fprintf(stderr, "Memory allocation failed\n");
    return defaultState;
  }
  curBuildingsInCity[0] = Coord{1, 2};
  curBuildingsInCity[2] = Coord{5, 2};

  Coord* curMarketsInCity = (Coord*)malloc(numCities * sizeof(Coord));
  if (!curMarketsInCity) {
    fprintf(stderr, "Memory allocation failed\n");
    return defaultState;
  }
  curMarketsInCity[2] = Coord{4, 2};

  return BacktrackState{
    (const int32_t*) map,
    rows,
    cols,

    (const int32_t*)cityCenters,
    numCities,

    (const int32_t*)tilesOwnedByCity,

    (int32_t*)curBuildingsInCity,
    (int32_t*)curMarketsInCity,
  };
}

inline const vector<vector<int>> realStateAllowedMarketPlacements = {
  {0, 1, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
};
inline const vector<vector<int>> realStateAllowedBuildingPlacements = {
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0},
  {0, 0, 0, 1, 1, 1},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
};


