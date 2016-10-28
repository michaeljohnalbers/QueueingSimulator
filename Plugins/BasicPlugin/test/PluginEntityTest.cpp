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
  // Test empty construction
  EXPECT_NO_THROW(QS::PluginEntity({}, ""));

  // Test construction
  QS::PluginEntity::Properties properties = {
    {"Key1", "Value1"},
    {"Dummy", "Dummy, too"}};
  std::string tag{"EntityInstance1"};

  EXPECT_NO_THROW(QS::PluginEntity(properties, tag));

  // Test properties getter
  QS::PluginEntity entity{properties, tag};
  EXPECT_EQ(properties, entity.getProperties());

  // Test tag getter
  EXPECT_EQ(tag, entity.getTag());

  // Verify copy/move functions
  QS::PluginEntity entityCopy(entity);
  entityCopy = entity;
  QS::PluginEntity entityMove(std::move(entity));
  entityMove = std::move(entity);
}
