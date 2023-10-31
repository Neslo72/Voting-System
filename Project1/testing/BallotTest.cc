#include "gtest/gtest.h"
#include "../include/ballot.h"

class BallotTest:public::testing::Test {
    public:
        void SetUp() {
            // no current setup
        }
        void TearDown() {
            // delete pointers from SetUp
        }
    protected:
        // define any variables here
};

TEST_F(BallotTest, BasicExampleTest) {
    EXPECT_EQ(2, 2);
}

TEST_F(BallotTest, BallotConstructorTest) {
    EXPECT_EQ(2, 2);
    // acutally make the test
}

TEST_F(BallotTest, testtesttest) {
    EXPECT_EQ(1, 2);
    // make the tests
}


// Main to run the tests
// Needed in every test file by the makefile's compilation method
int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}