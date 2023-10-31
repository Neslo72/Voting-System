#include "gtest/gtest.h"

class StartTest : public ::testing::Test
{
  public:
    void SetUp()
    {
        // no current setup
    }
    void TearDown()
    {
        // delete pointers from SetUp
    }

  protected:
    // define any variables here
};

// Main to run the tests
// Needed in every test file because of the makefile's compilation method
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}