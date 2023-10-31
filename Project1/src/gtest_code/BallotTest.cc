#include "../include/ballot.h"
#include "gtest/gtest.h"

class BallotTest : public ::testing::Test
{
  public:
    void SetUp()
    {
        b = Ballot();
    }
    void TearDown()
    {
        // delete pointers from SetUp
        // (There are no pointers...)
    }

  protected:
    Ballot b;
};

// BallotTest_1
// Simple test ballot addChoice and getChoice
TEST_F(BallotTest, addChoice)
{
    b.addChoice(1);
    int val = b.getChoice();
    EXPECT_EQ(1, val);
    val = b.getChoice();
    EXPECT_EQ(-1, val);
}

// BallotTest_2
// Testing empty choices getChoice result
TEST_F(BallotTest, EmptyChoices)
{
    int val = b.getChoice(); // Should return -1, b is empty
    EXPECT_EQ(val, -1);
}

// BallotTest_3
// Testing addChoice ordering with random elements
TEST_F(BallotTest, addChoiceOrder)
{
    srand(time(0));
    int arr[10];
    for (int i = 0; i < 10; i++)
    {
        int rand_val = rand();
        arr[i] = rand_val;
        b.addChoice(rand_val);
    }

    // b should be have same ordering as arr from 0-9
    // verfiy with next loop

    for (int i = 0; i < 10; i++)
    {
        int val = b.getChoice();
        EXPECT_EQ(arr[i], val);
    }
}

// Main to run the tests
// Needed in every test file by the makefile's compilation method
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}