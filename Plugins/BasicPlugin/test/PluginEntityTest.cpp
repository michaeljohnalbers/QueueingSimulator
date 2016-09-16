/**
 * @file PluginEntity.cpp
 * @brief Unit tests for PluginEntity class
 *
 * @author Michael Albers
 */

#include "PluginEntity.h"
#include "gtest/gtest.h"

GTEST_TEST(PluginEntityTest, testAll)
{
  // Test construction
  EXPECT_NO_THROW(QS::PluginEntity{{}});
  QS::PluginEntity::Properties properties = {
    {"Key1", "Value1"},
    {"Dummy", "Dummy, too"}};

  EXPECT_NO_THROW(QS::PluginEntity{properties});

  // Test properties getter
  QS::PluginEntity entity{properties};
  EXPECT_EQ(properties, entity.getProperties());

  // Verify copy/move functions
  QS::PluginEntity entityCopy(entity);
  entityCopy = entity;
  QS::PluginEntity entityMove(std::move(entity));
  entityMove = std::move(entity);
}
