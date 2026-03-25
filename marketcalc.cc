#include "pair_hash.h"
#include "marketcalc.h"


#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <climits>
#include <cmath>
#include <functional>
#include <utility>
#include <unordered_set>


using std::vector;
using std::pair;
using std::unordered_map;
using std::unordered_set;




bool inBounds(const vector<vector<TileState>>& map, Coord coord) {
    int height = map.size();
    int width = map[0].size();
    return coord.row >= 0 && coord.row < height && coord.col >= 0 && coord.col < width;
}

// Find all city centers from the map
// Guarenteed to be the same order every time (top to bottom, left to right)
// so that city IDs are consistent when using border growth order
vector<Coord> findCityCenters(const vector<vector<TileState>>& map) {
    vector<Coord> centers;
    int height = map.size();
    int width = map[0].size();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (map[y][x].type == CITY) {
                centers.push_back(Coord{y, x});
            }
        }
    }
    return centers;
}

void computeOwnership(vector<vector<TileState>>& map, 
                      const vector<Coord>& cityCenters,
                      const vector<int>& actionOrder) {
    // cityId -> whether captured
    unordered_map<int, int> timesCitySeen;

    // Simulate border growths in order to get proper ownership map
    for (int orderIdx = 0; orderIdx < (int)actionOrder.size(); orderIdx++) {
        int cityId = actionOrder[orderIdx];
        int cx = cityCenters[cityId].col;
        int cy = cityCenters[cityId].row;
        
        // City has not been captured yet
        // Capture it and compute 3x3
        if (timesCitySeen[cityId] == 0) {
            timesCitySeen[cityId] = 1;

          // Assign city tile itself
          map[cy][cx].owner = cityId;
          
          // Assign 3x3 ownership
          for (int i = 0; i < BASE_TILE_COUNT; i++) {
              int nx = cx + dx[i];
              int ny = cy + dy[i];
              Coord coord = {ny, nx};
              if (inBounds(map, coord) && map[ny][nx].type != OBSTACLE) {
                  // Only assign if unowned
                  // Do not overwrite previous border growths
                  if (map[ny][nx].owner == -1) {
                      map[ny][nx].owner = cityId;
                  }
              }
          }
        }

        else if (timesCitySeen[cityId] == 1) {
          timesCitySeen[cityId] = 2;
          // Assign 5x5 ownership
          for (int i = 0; i < EXPANDED_TILE_COUNT; i++) {
              int nx = cx + dx5[i];
              int ny = cy + dy5[i];
              Coord coord = {ny, nx};
              if (inBounds(map, coord) && map[ny][nx].type != OBSTACLE) {
                  // Only assign if unowned
                  // Do not overwrite previous border growths
                  if (map[ny][nx].owner == -1) {
                      map[ny][nx].owner = cityId;
                  }
              }
          }
        }
        
        else {
          throw std::invalid_argument("City ID " + std::to_string(cityId) + " appears more than twice in actionOrder");
        }
    }

}

/*
Defined in marketcalc.h
*/
bool canPlaceMarket(const BacktrackState& state, Coord coord) {
  int row = coord.row;
  int col = coord.col;
  
  // Can only place within city borders
  if (state.map[row][col].owner == -1) {
    return false;
  }

  // Can only place on empty or resource tile
  if (state.map[row][col].type != EMPTY && state.map[row][col].type != RESOURCE) {
    return false;
  }

  // Market exists in city already
  if (state.curMarketsInCity.find(state.map[row][col].owner) != state.curMarketsInCity.end()) {
    return false;
  }

  // Already occupied by something we just placed
  // SANITY CHECK, DONT NEED since we already checked the map
  if (state.curBuildingsSet.find(coord) != state.curBuildingsSet.end() || 
      state.curMarketsSet.find(coord) != state.curMarketsSet.end()) {
    return false;
  }

  // Check adjacency to at least one building tile
  for (int i = 0; i < BASE_TILE_COUNT; i++) {
    int nx = row + dx[i];
    int ny = col + dy[i];
    Coord adjacentCoord = {nx, ny};
    if (inBounds(state.map, adjacentCoord) &&
       state.curBuildingsSet.find(adjacentCoord) != state.curBuildingsSet.end()) {
      return true;
    }
  }
  // fix?
  return false;
}

/*
Defined in marketcalc.h
*/
bool canPlaceBuilding(const BacktrackState& state, Coord coord) {
  int row = coord.row;
  int col = coord.col;

  // Can only place within city borders
  if (state.map[row][col].owner == -1) {
    return false;
  }

  // Can only place on empty or resource tile
  if (state.map[row][col].type != EMPTY && state.map[row][col].type != RESOURCE) {
    return false;
  }

  // Building exists in city already
  if (state.curBuildingsInCity.find(state.map[row][col].owner) != state.curBuildingsInCity.end()) {
    return false;
  }

  // Already occupied by something we just placed
  // SANITY CHECK, DONT NEED since we already checked the map
  if (state.curBuildingsSet.find(coord) != state.curBuildingsSet.end() || 
      state.curMarketsSet.find(coord) != state.curMarketsSet.end()) {
    return false;
  }

  // Check adjacency to at least one resource tile
  // which is not covered
  for (int i = 0; i < BASE_TILE_COUNT; i++) {
    int nx = col + dx[i];
    int ny = row + dy[i];
    Coord adjacentCoord = {nx, ny};
    if (inBounds(state.map, adjacentCoord) &&
       (state.map[ny][nx].type == RESOURCE || 
       state.map[ny][nx].type == USED_RESOURCE) && 
       state.curBuildingsSet.find(adjacentCoord) == state.curBuildingsSet.end() &&
       state.curMarketsSet.find(adjacentCoord) == state.curMarketsSet.end()) {

      return true;
    }
  }
  // fix?
  return false;
}

#if 0

MarketResult calculateBestMarketsGivenGrowthOrder(const vector<vector<int>>& map, const vector<int>& growthOrder) {
    // Build up initial state
    // Create owner map
    int height = map.size();
    int width = map[0].size();
    vector<pair<int,int>> cityCenters = findCityCenters(map);
    int numCities = cityCenters.size();

    // same size as map
    vector<vector<int>> owner(height, vector<int>(width, -1));

    // Simulate border growths in order to get proper ownership map
    owner = computeOwnership(map, cityCenters, growthOrder);

    // Get placeable tiles for each city
    // so we can iterate through them all
    unordered_map<int, vector<pair<int,int>>> tilesOwnedByCity;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int cityId = owner[y][x];
            if (cityId != -1 && (map[y][x] == EMPTY || map[y][x] == RESOURCE)) {
                tilesOwnedByCity[cityId].push_back({x, y});
            }
        }
    }

    // Algorithm: Backtracking to try all building/market placements
    BacktrackState state = {
        map,
        cityCenters,
        tilesOwnedByCity,
        height,
        width,
        numCities,
        vector<pair<int,int>>(numCities, {-1, -1}), // curBuildings
        unordered_set<pair<int,int>>(),
        vector<pair<int,int>>(numCities, {-1, -1}), // curMarkets
        unordered_set<pair<int,int>>(),
        0, 
        map, // best layout starts off as original map
        0,
    };

    while (true) {
      // TODO: Implement backtracking logic to place buildings and markets
      break;
    }

    //TODO: replace
    return {state.bestMarketTotal, state.bestLayout};
}

/**
 * Given a BacktrackState and coordinates, check if we can place a building
 * Rules: 
 * building must be adjacent to at least 1 resource
 * can only be placed on empty or resource tile
 * 
 * This function DOES NOT CHECK only 1 building per city
 */
bool canPlaceBuilding(const BacktrackState& state, int x, int y) {
  // Can only place on empty or resource tile
  if (state.map[y][x] != EMPTY && state.map[y][x] != RESOURCE) {
    return false; 
  }

  if (state.curBuildingsSet.find({x, y}) != state.curBuildingsSet.end() || 
      state.curMarketsSet.find({x, y}) != state.curMarketsSet.end()) {
    return false; // Already occupied by another building or market
  }

  // Check adjacency to at least one resource tile
  for (int i = 0; i < BASE_TILE_COUNT; i++) {
    int nx = x + dx[i];
    int ny = y + dy[i];
    if (inBounds(state.map, nx, ny) &&
       state.map[ny][nx] == RESOURCE && 
       state.curBuildingsSet.find({nx, ny}) == state.curBuildingsSet.end() &&
       state.curMarketsSet.find({nx, ny}) == state.curMarketsSet.end()) {
      return true;
    }
  }
  return false;
}


/**
 * Given a BacktrackState, calculate the total market level
 * based on current building and market placements.
 */
int calculateMarketTotal(const BacktrackState& state) {
  // Place all buildings and markets on a temporary map
  vector<vector<int>> tempMap = state.map;
  for (int c = 0; c < state.numCities; c++) {
    if (state.curBuildings[c].first != -1) {
      int bx = state.curBuildings[c].first;
      int by = state.curBuildings[c].second;
      tempMap[by][bx] = BUILDING;
      // Safety check: you may only place on empty tile or resource tile
      if (state.map[by][bx] != EMPTY && state.map[by][bx] != RESOURCE) {
        std::cerr << "Error: Attempted to place building on invalid tile." << std::endl;
        exit(1);
      }
    }
    if (state.curMarkets[c].first != -1) {
      int mx = state.curMarkets[c].first;
      int my = state.curMarkets[c].second;
      tempMap[my][mx] = MARKET;
      // Safety check: you may only place on empty tile or resource tile
      if (state.map[my][mx] != EMPTY && state.map[my][mx] != RESOURCE) {
        std::cerr << "Error: Attempted to place market on invalid tile." << std::endl;
        exit(1);
      }
    }
  }

  // First calculate building levels
  unordered_map<pair<int,int>, int> buildingLevels;
  for (int c = 0; c < state.numCities; c++) {
    if (state.curBuildings[c].first != -1) {
      int bx = state.curBuildings[c].first;
      int by = state.curBuildings[c].second;
      int level = 0;
      // Count adjacent resource tiles
      for (int i = 0; i < BASE_TILE_COUNT; i++) {
        int nx = bx + dx[i];
        int ny = by + dy[i];
        if (inBounds(state.map, nx, ny) &&
           (state.map[ny][nx] == RESOURCE) &&
           state.curBuildingsSet.find({nx, ny}) == state.curBuildingsSet.end() &&
           state.curMarketsSet.find({nx, ny}) == state.curMarketsSet.end()) {
          level++;
        }
      }
      buildingLevels[{bx, by}] = std::min(level, MAX_BUILDING_LEVEL); // cap
    }
  }
  
  // Next calculate market levels
  int totalMarketLevel = 0;
  for (int c = 0; c < state.numCities; c++) {
    if (state.curMarkets[c].first != -1) {
      int mx = state.curMarkets[c].first;
      int my = state.curMarkets[c].second;
      int marketLevel = 0;
      // Sum adjacent building levels
      for (int i = 0; i < BASE_TILE_COUNT; i++) {
        int nx = mx + dx[i];
        int ny = my + dy[i];
        if (inBounds(state.map, nx, ny)) {
          auto it = buildingLevels.find({nx, ny});
          if (it != buildingLevels.end()) {
            marketLevel += it->second;
          }
        }
      }
      marketLevel = std::min(marketLevel, MAX_MARKET_LEVEL); // cap
      totalMarketLevel += marketLevel;
    }
  }

  // Sum those and return
  return totalMarketLevel;
    
}

vector<vector<int>> buildLayoutFromState(const BacktrackState& state) {
    vector<vector<int>> layout = state.map;
    for (int c = 0; c < state.numCities; c++) {
        if (state.curBuildings[c].first != -1) {
            int bx = state.curBuildings[c].first;
            int by = state.curBuildings[c].second;
            layout[by][bx] = BUILDING;
        }
        if (state.curMarkets[c].first != -1) {
            int mx = state.curMarkets[c].first;
            int my = state.curMarkets[c].second;
            layout[my][mx] = MARKET;
        }
    }
    return layout;
}

// Place markets for each city using backtracking
BacktrackState placeMarkets(BacktrackState& state, int cityIdx) {
    if (cityIdx == state.numCities) {
        // All markets placed, evaluate current layout
        int currentMarketTotal = calculateMarketTotal(state);
        if (currentMarketTotal > state.bestMarketTotal) {
            state.bestMarketTotal = currentMarketTotal;
            state.bestLayout = buildLayoutFromState(state);
        }
        return state;
    }

    int cityId = cityIdx;
    const auto& placeableTiles = state.tilesOwnedByCity.at(cityId);

    // Attempt to place a market in each tile and backtrack from there
    for (const auto& tile : placeableTiles) {
        int x = tile.first;
        int y = tile.second;

        // Check if we can place market here
        if (canPlaceMarket(state, x, y)) {
            // Place market
            state.curMarkets[cityId] = {x, y};

            // Recurse to next city
            placeMarkets(state, cityIdx + 1);

            // Backtrack
            state.curMarkets[cityId] = {-1, -1};
        }
    }

    // Also try not placing a market for this city
    state.curMarkets[cityId] = {-1, -1};
    placeMarkets(state, cityIdx + 1);

    return state;
}

// Place buildings for each city using backtracking
BacktrackState placeBuildings(BacktrackState& state, int cityIdx) {
    if (cityIdx == state.numCities) {
        // All buildings placed, proceed to place markets
        return placeMarkets(state, 0);
    }

    int cityId = cityIdx;
    const auto& placeableTiles = state.tilesOwnedByCity.at(cityId);

    // Attempt to place a building in each tile and backtrack from there
    for (const auto& tile : placeableTiles) {
        int x = tile.first;
        int y = tile.second;

        // Check if we can place building here
        if (canPlaceBuilding(state, x, y)) {
            // Place building
            state.curBuildings[cityId] = {x, y};

            // Recurse to next city
            placeBuildings(state, cityIdx + 1);

            // Backtrack
            state.curBuildings[cityId] = {-1, -1};
        }
    }

    // Also try not placing a building for this city
    state.curBuildings[cityId] = {-1, -1};
    placeBuildings(state, cityIdx + 1);

    return state;
}

/*
Given a tile map, Calculate the best sawmill/windmill market spots
and return a 2D vector representing map layout.

Input: 2d vector representing the map
-- Valid values:
-- 0: empty tile
-- 1: city
-- 2: obstacle (mountain, water) where you cannot place anything
-- 3: resource tile (forest or farm)

Output: MarketResult struct
*/
MarketResult market_calc(const vector<vector<int>>& map) {
    MarketResult best_layout;

    vector<pair<int,int>> cityCenters = findCityCenters(map);
    int village_count = cityCenters.size();
    
    if (village_count == 0) {
        return best_layout;
    }
    
    int height = map.size();
    int width = map[0].size();
    
    // Find which cities are involved in any conflict (only these need to be permuted)
    // We will just permute all of them for simplicity
    // Users can simply use the tool multiple times
    // on visually separated clusters of cities
    vector<int> growthOrder;
    for (int c = 0; c < village_count; c++) {
        growthOrder.push_back(c);
    }
    
    // Theoretical maximum: each city can have at most level 8 market
    int theoreticalMax = village_count * 8;
    
    do {
        MarketResult result = calculateBestMarketsGivenGrowthOrder(map, growthOrder);
        
        if (result.totalMarketLevel > best_layout.totalMarketLevel) {
            best_layout = result;
            
            // Early termination if we hit theoretical maximum
            // (level 8 market in each city)
            if (best_layout.totalMarketLevel >= theoreticalMax) {
                return best_layout;
            }
        }
    } while (std::next_permutation(growthOrder.begin(), growthOrder.end()));

    return best_layout;
}


#endif
