#include "marketcalc_test_common.h"

extern "C" {
#include "marketcalc.h"
}

#include <gtest/gtest.h>

TEST(computeOwnership, AllCitiesGrow) {
  int32_t* map = makeNoSpecialMap();
  int32_t* cityCenters = makeNoSpecialCityCenters();
  int32_t actionOrder[] = {0, 1, 2, 0, 1, 2};

  int32_t expectedResult[] = {
    0, EMPTY, 0, EMPTY, 0, EMPTY, 0, EMPTY, -1, EMPTY, -1, EMPTY,
    0, EMPTY, 0, CITY, 0, EMPTY, 0, EMPTY,  1, EMPTY,  1, EMPTY,
    0, EMPTY, 0, EMPTY, 0, EMPTY,  1, EMPTY,  1, EMPTY,  1, EMPTY,
    2, EMPTY, 2, EMPTY, 2, EMPTY,  1, EMPTY,  1, CITY,  1, EMPTY,
    2, EMPTY, 2, CITY, 2, EMPTY,  1, EMPTY,  1, EMPTY,  1, EMPTY,
    2, EMPTY, 2, EMPTY, 2, EMPTY,  1, EMPTY,  1, EMPTY,  1, EMPTY,
    2, EMPTY, 2, EMPTY, 2, EMPTY,  2, EMPTY, -1, EMPTY, -1, EMPTY,
   -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY
  };

  computeOwnership(map, 8, 6,
    cityCenters, 3,
    actionOrder, 6);

  for (int i = 0; i < 8 * 6 * 2; i++) {
    EXPECT_EQ(map[i], expectedResult[i]);
  }

  free(map);
  free(cityCenters);
}

TEST(computeOwnership, AllCitiesGrowAlternateOrder) {
  int32_t* map = makeNoSpecialMap();
  int32_t* cityCenters = makeNoSpecialCityCenters();
  int32_t actionOrder[] = {0, 1, 2, 2, 1, 0};

  int32_t expectedResult[] = {
    0, EMPTY, 0, EMPTY, 0, EMPTY, 0, EMPTY, -1, EMPTY, -1, EMPTY,
    0, EMPTY, 0, CITY, 0, EMPTY, 1, EMPTY,  1, EMPTY,  1, EMPTY,
    0, EMPTY, 0, EMPTY, 0, EMPTY,  1, EMPTY,  1, EMPTY,  1, EMPTY,
    2, EMPTY, 2, EMPTY, 2, EMPTY,  1, EMPTY,  1, CITY,  1, EMPTY,
    2, EMPTY, 2, CITY, 2, EMPTY,  1, EMPTY,  1, EMPTY,  1, EMPTY,
    2, EMPTY, 2, EMPTY, 2, EMPTY,  2, EMPTY,  1, EMPTY,  1, EMPTY,
    2, EMPTY, 2, EMPTY, 2, EMPTY,  2, EMPTY, -1, EMPTY, -1, EMPTY,
   -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY
  };

  computeOwnership(map, 8, 6, cityCenters, 3, actionOrder, 6);

  for (int i = 0; i < 8 * 6 * 2; i++) {
    EXPECT_EQ(map[i], expectedResult[i]);
  }

  free(map);
  free(cityCenters);
}

TEST(computeOwnership, NotAllCitiesGrow) {
  int32_t* map = makeNoSpecialMap();
  int32_t* cityCenters = makeNoSpecialCityCenters();
  int32_t actionOrder[] = {0, 1, 2, 2, 1};

  int32_t expectedResult[] = {
    0, EMPTY, 0, EMPTY, 0, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    0, EMPTY, 0, CITY, 0, EMPTY, 1, EMPTY,  1, EMPTY,  1, EMPTY,
    0, EMPTY, 0, EMPTY, 0, EMPTY,  1, EMPTY,  1, EMPTY,  1, EMPTY,
    2, EMPTY, 2, EMPTY, 2, EMPTY,  1, EMPTY,  1, CITY,  1, EMPTY,
    2, EMPTY, 2, CITY, 2, EMPTY,  1, EMPTY,  1, EMPTY,  1, EMPTY,
    2, EMPTY, 2, EMPTY, 2, EMPTY,  2, EMPTY,  1, EMPTY,  1, EMPTY,
    2, EMPTY, 2, EMPTY, 2, EMPTY,  2, EMPTY, -1, EMPTY, -1, EMPTY,
   -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY
  };

  computeOwnership(map, 8, 6, cityCenters, 3, actionOrder, 5);

  for (int i = 0; i < 8 * 6 * 2; i++) {
    EXPECT_EQ(map[i], expectedResult[i]);
  }

  free(map);
  free(cityCenters);
}

TEST(computeOwnership, GrowBeforeCapture) {
  int32_t* map = makeNoSpecialMap();
  int32_t* cityCenters = makeNoSpecialCityCenters();
  int32_t actionOrder[] = {2, 2, 1, 0};

  int32_t expectedResult[] = {
    0, EMPTY, 0, EMPTY, 0, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    0, EMPTY, 0, CITY, 0, EMPTY, -1, EMPTY,  -1, EMPTY,  -1, EMPTY,
    2, EMPTY, 2, EMPTY, 2, EMPTY,  2, EMPTY,  1, EMPTY,  1, EMPTY,
    2, EMPTY, 2, EMPTY, 2, EMPTY,  2, EMPTY,  1, CITY,  1, EMPTY,
    2, EMPTY, 2, CITY, 2, EMPTY,  2, EMPTY,  1, EMPTY,  1, EMPTY,
    2, EMPTY, 2, EMPTY, 2, EMPTY,  2, EMPTY,  -1, EMPTY,  -1, EMPTY,
    2, EMPTY, 2, EMPTY, 2, EMPTY,  2, EMPTY, -1, EMPTY, -1, EMPTY,
   -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY
  };

  computeOwnership(map, 8, 6, cityCenters, 3, actionOrder, 4);

  for (int i = 0; i < 8 * 6 * 2; i++) {
    EXPECT_EQ(map[i], expectedResult[i]);
  }

  free(map);
  free(cityCenters);
}

TEST(computeOwnership, ObstaclesBlockCapture) {
  int32_t map[] = {
    -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    -1, EMPTY, -1, CITY, -1, OBSTACLE, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    -1, EMPTY, -1, OBSTACLE, -1, OBSTACLE, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    -1, EMPTY, -1, OBSTACLE, -1, OBSTACLE, -1, EMPTY, -1, CITY, -1, EMPTY,
    -1, EMPTY, -1, CITY, -1, OBSTACLE, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    -1, EMPTY, -1, OBSTACLE, -1, OBSTACLE, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY
  };
  int32_t cityCenters[] = {1, 1, 3, 4, 4, 1};
  int32_t actionOrder[] = {2, 1, 0};

  int32_t expectedResult[] = {
    0, EMPTY, 0, EMPTY, 0, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    0, EMPTY, 0, CITY, -1, OBSTACLE, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    0, EMPTY, -1, OBSTACLE, -1, OBSTACLE, 1, EMPTY, 1, EMPTY, 1, EMPTY,
    2, EMPTY, -1, OBSTACLE, -1, OBSTACLE, 1, EMPTY, 1, CITY, 1, EMPTY,
    2, EMPTY, 2, CITY, -1, OBSTACLE, 1, EMPTY, 1, EMPTY, 1, EMPTY,
    2, EMPTY, -1, OBSTACLE, -1, OBSTACLE, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY
  };

  computeOwnership(map, 8, 6, cityCenters, 3, actionOrder, 3);

  for (int i = 0; i < 8 * 6 * 2; i++) {
    EXPECT_EQ(map[i], expectedResult[i]);
  }
}

TEST(computeOwnership, FullScenario) {
  int32_t map[] = {
    -1, EMPTY, -1, OBSTACLE, -1, RESOURCE, -1, EMPTY, -1, OBSTACLE, -1, RESOURCE,
    -1, RESOURCE, -1, CITY, -1, EMPTY, -1, OBSTACLE, -1, RESOURCE, -1, EMPTY,
    -1, OBSTACLE, -1, RESOURCE, -1, EMPTY, -1, RESOURCE, -1, EMPTY, -1, OBSTACLE,
    -1, EMPTY, -1, OBSTACLE, -1, RESOURCE, -1, EMPTY, -1, CITY, -1, OBSTACLE,
    -1, RESOURCE, -1, CITY, -1, OBSTACLE, -1, EMPTY, -1, RESOURCE, -1, EMPTY,
    -1, OBSTACLE, -1, EMPTY, -1, RESOURCE, -1, OBSTACLE, -1, EMPTY, -1, RESOURCE,
    -1, EMPTY, -1, RESOURCE, -1, OBSTACLE, -1, RESOURCE, -1, OBSTACLE, -1, EMPTY,
    -1, RESOURCE, -1, OBSTACLE, -1, EMPTY, -1, OBSTACLE, -1, RESOURCE, -1, OBSTACLE
  };
  int32_t cityCenters[] = {1, 1, 3, 4, 4, 1};
  int32_t actionOrder[] = {2, 1, 0, 1};

  int32_t expectedResult[] = {
    0, EMPTY, -1, OBSTACLE, 0, RESOURCE, -1, EMPTY, -1, OBSTACLE, -1, RESOURCE,
    0, RESOURCE, 0, CITY, 0, EMPTY, -1, OBSTACLE, 1, RESOURCE, 1, EMPTY,
    -1, OBSTACLE, 0, RESOURCE, 0, EMPTY, 1, RESOURCE, 1, EMPTY, -1, OBSTACLE,
    2, EMPTY, -1, OBSTACLE, 2, RESOURCE, 1, EMPTY, 1, CITY, -1, OBSTACLE,
    2, RESOURCE, 2, CITY, -1, OBSTACLE, 1, EMPTY, 1, RESOURCE, 1, EMPTY,
    -1, OBSTACLE, 2, EMPTY, 2, RESOURCE, -1, OBSTACLE, 1, EMPTY, 1, RESOURCE,
    -1, EMPTY, -1, RESOURCE, -1, OBSTACLE, -1, RESOURCE, -1, OBSTACLE, -1, EMPTY,
    -1, RESOURCE, -1, OBSTACLE, -1, EMPTY, -1, OBSTACLE, -1, RESOURCE, -1, OBSTACLE
  };

  computeOwnership(map, 8, 6, cityCenters, 3, actionOrder, 4);

  for (int i = 0; i < 8 * 6 * 2; i++) {
    EXPECT_EQ(map[i], expectedResult[i]);
  }
}

TEST(computeOwnership, UnclaimedVillage) {
  int32_t map[] = {
    -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY,
    -1, CITY, -1, EMPTY, -1, EMPTY, -1, CITY,
    -1, EMPTY, -1, EMPTY, -1, EMPTY, -1, EMPTY
  };
  int32_t cityCenters[] = {1, 0};
  int32_t actionOrder[] = {0, 0};

  int32_t expectedResult[] = {
    0, EMPTY, 0, EMPTY, 0, EMPTY, -1, EMPTY,
    0, CITY, 0, EMPTY, 0, EMPTY, -1, CITY,
    0, EMPTY, 0, EMPTY, 0, EMPTY, -1, EMPTY
  };

  computeOwnership(map, 3, 4, cityCenters, 1, actionOrder, 2);

  for (int i = 0; i < 3 * 4 * 2; i++) {
    EXPECT_EQ(map[i], expectedResult[i]);
  }
}
