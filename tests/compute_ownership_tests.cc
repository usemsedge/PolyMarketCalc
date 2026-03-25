#include "marketcalc_test_common.h"

#include <gtest/gtest.h>

TEST(MarketCalcTest, computeOwnership_AllCitiesGrow) {
  vector<vector<TileState>> map = makeNoSpecialMap();
  vector<Coord> cityCenters = makeNoSpecialCityCenters();
  vector<int> actionOrder = {0, 1, 2, 0, 1, 2};

  vector<vector<TileState>> expectedResult = {
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, EMPTY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };

  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}

TEST(MarketCalcTest, computeOwnership_AllCitiesGrowAlternateOrder) {
  vector<vector<TileState>> map = makeNoSpecialMap();
  vector<Coord> cityCenters = makeNoSpecialCityCenters();
  vector<int> actionOrder = {0, 1, 2, 2, 1, 0};

  vector<vector<TileState>> expectedResult = {
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };

  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}

TEST(MarketCalcTest, computeOwnership_NotAllCitiesGrow) {
  vector<vector<TileState>> map = makeNoSpecialMap();
  vector<Coord> cityCenters = makeNoSpecialCityCenters();
  vector<int> actionOrder = {0, 1, 2, 2, 1};

  vector<vector<TileState>> expectedResult = {
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };

  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}

TEST(MarketCalcTest, computeOwnership_GrowBeforeCapture) {
  vector<vector<TileState>> map = makeNoSpecialMap();
  vector<Coord> cityCenters = makeNoSpecialCityCenters();
  vector<int> actionOrder = {2, 2, 1, 0};

  vector<vector<TileState>> expectedResult = {
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(2, EMPTY), T(2, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(2, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };

  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}

TEST(MarketCalcTest, computeOwnership_ObstaclesBlockCapture) {
  vector<vector<TileState>> map = {
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, CITY), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, OBSTACLE), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, OBSTACLE), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, CITY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, CITY), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, OBSTACLE), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };
  vector<Coord> cityCenters = makeNoSpecialCityCenters();
  vector<int> actionOrder = {2, 1, 0};

  vector<vector<TileState>> expectedResult = {
    {T(0, EMPTY), T(0, EMPTY), T(0, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(0, CITY), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(0, EMPTY), T(-1, OBSTACLE), T(-1, OBSTACLE), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(-1, OBSTACLE), T(-1, OBSTACLE), T(1, EMPTY), T(1, CITY), T(1, EMPTY)},
    {T(2, EMPTY), T(2, CITY), T(-1, OBSTACLE), T(1, EMPTY), T(1, EMPTY), T(1, EMPTY)},
    {T(2, EMPTY), T(-1, OBSTACLE), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
    {T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY), T(-1, EMPTY)},
  };

  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}

TEST(MarketCalcTest, computeOwnership_FullScenario) {
  vector<vector<TileState>> map = {
    {T(-1, EMPTY), T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, EMPTY), T(-1, OBSTACLE), T(-1, RESOURCE)},
    {T(-1, RESOURCE), T(-1, CITY), T(-1, EMPTY), T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, EMPTY)},
    {T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, EMPTY), T(-1, RESOURCE), T(-1, EMPTY), T(-1, OBSTACLE)},
    {T(-1, EMPTY), T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, EMPTY), T(-1, CITY), T(-1, OBSTACLE)},
    {T(-1, RESOURCE), T(-1, CITY), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, RESOURCE), T(-1, EMPTY)},
    {T(-1, OBSTACLE), T(-1, EMPTY), T(-1, RESOURCE), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, RESOURCE)},
    {T(-1, EMPTY), T(-1, RESOURCE), T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, OBSTACLE), T(-1, EMPTY)},
    {T(-1, RESOURCE), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, OBSTACLE)},
  };
  vector<Coord> cityCenters = makeNoSpecialCityCenters();
  vector<int> actionOrder = {2, 1, 0, 1};

  vector<vector<TileState>> expectedResult = {
    {T(0, EMPTY), T(-1, OBSTACLE), T(0, RESOURCE), T(-1, EMPTY), T(-1, OBSTACLE), T(-1, RESOURCE)},
    {T(0, RESOURCE), T(0, CITY), T(0, EMPTY), T(-1, OBSTACLE), T(1, RESOURCE), T(1, EMPTY)},
    {T(-1, OBSTACLE), T(0, RESOURCE), T(0, EMPTY), T(1, RESOURCE), T(1, EMPTY), T(-1, OBSTACLE)},
    {T(2, EMPTY), T(-1, OBSTACLE), T(2, RESOURCE), T(1, EMPTY), T(1, CITY), T(-1, OBSTACLE)},
    {T(2, RESOURCE), T(2, CITY), T(-1, OBSTACLE), T(1, EMPTY), T(1, RESOURCE), T(1, EMPTY)},
    {T(-1, OBSTACLE), T(2, EMPTY), T(2, RESOURCE), T(-1, OBSTACLE), T(1, EMPTY), T(1, RESOURCE)},
    {T(-1, EMPTY), T(-1, RESOURCE), T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, OBSTACLE), T(-1, EMPTY)},
    {T(-1, RESOURCE), T(-1, OBSTACLE), T(-1, EMPTY), T(-1, OBSTACLE), T(-1, RESOURCE), T(-1, OBSTACLE)},
  };

  computeOwnership(map, cityCenters, actionOrder);
  EXPECT_EQ(map, expectedResult);
}
