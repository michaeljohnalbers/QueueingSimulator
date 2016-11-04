/**
 * @file ExitDefinitionTest.cpp
 * @brief Unit test of ExitDefinition class
 *
 * @author Michael Albers
 */

#include "ExitDefinition.h"
#include "gtest/gtest.h"

GTEST_TEST(ExitDefinitionTest, testConstruction)
{
  EXPECT_NO_THROW(QS::ExitDefinition exit(""));
}

GTEST_TEST(ExitDefinition, testCopyMove)
{
  QS::ExitDefinition exitDef("StandardExit");

  // Test copy constructor
  QS::ExitDefinition exitCopy(exitDef);
  EXPECT_EQ(exitDef.getName(), exitCopy.getName());

  // Test move constructor
  QS::ExitDefinition exitMove(std::move(exitCopy));
  EXPECT_EQ(exitDef.getName(), exitMove.getName());

  // Test copy assignment operator
  QS::ExitDefinition exitAssignCopy{""};
  exitAssignCopy = exitDef;
  EXPECT_EQ(exitDef.getName(), exitAssignCopy.getName());

  // Test move assignment operator
  QS::ExitDefinition exitAssignMove{""};
  exitAssignMove = exitDef;
  EXPECT_EQ(exitDef.getName(), exitAssignMove.getName());
}

GTEST_TEST(ExitDefinition, testGetters)
{
  QS::ExitDefinition exitDef("StandardExit");
  EXPECT_EQ("StandardExit", exitDef.getName());
}

GTEST_TEST(ExitDefinition, testEqualityOperator)
{
  QS::ExitDefinition exitDef("StandardExit");
  auto exitCopy(exitDef);
  EXPECT_TRUE(exitDef == exitCopy);
  EXPECT_TRUE(exitCopy == exitDef);

  // Test exit with a different name.
  {
    QS::ExitDefinition exitDiff("Standardexit");
    EXPECT_FALSE(exitDef == exitDiff);
    EXPECT_FALSE(exitDiff == exitDef);
  }
}
