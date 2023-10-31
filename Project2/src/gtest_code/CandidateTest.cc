#include "../include/candidate.h"
#include "../include/ballot.h"
#include "gtest/gtest.h"

class CandidateTest : public ::testing::Test
{
  public:
    void SetUp()
    {
        c1 = Candidate("Sainz", "Ferrari");

        b1.addChoice(1);
        b2.addChoice(1);
        b3.addChoice(1);

        b2.addChoice(2);
        b3.addChoice(2);

        b3.addChoice(3);
    }
    void TearDown()
    {
        // delete pointers from SetUp
    }

  protected:
    Candidate c1;
    Ballot b1;
    Ballot b2;
    Ballot b3;
};

// Test constructor and getName / getParty
TEST_F(CandidateTest, CandidateConstructor)
{
    Candidate c = Candidate();
    EXPECT_EQ(c.getName(), ""); // Default constructor == empty
    EXPECT_EQ(c.getParty(), "");

    c = Candidate("mario", "party");

    EXPECT_EQ(c.getName(), "mario"); // Constructor with parameters
    EXPECT_EQ(c.getParty(), "party");
}

// Test addBallot / getNumVotes
TEST_F(CandidateTest, getNumVotes)
{
    EXPECT_EQ(c1.getNumVotes(), 0); // No ballots assigned means no votes
    c1.addBallot(b1);
    EXPECT_EQ(c1.getNumVotes(), 1); // 1 ballot assigned
}

// Test removeBallot
TEST_F(CandidateTest, removeBallot)
{
    // Test for actual ballots being removed
    Ballot b_temp;
    c1.addBallot(b3); // b3 has order 1, 2, 3
    for (int i = 1; i <= 3; i++)
    {
        c1.removeBallot(&b_temp);
        EXPECT_EQ(b_temp.getChoice(), i);
    }

    // Fill c1 with ballots
    c1.addBallot(b1);
    c1.addBallot(b2);
    c1.addBallot(b3);
    EXPECT_EQ(c1.getNumVotes(), 3); // Confrim 3 ballots added

    int res = c1.removeBallot(&b_temp);
    EXPECT_EQ(c1.getNumVotes(), 2); // Result of removing a ballot
    EXPECT_EQ(res, 0);              // Return of successful removBallot should be 0

    res = c1.removeBallot(&b_temp);
    res = c1.removeBallot(&b_temp);
    EXPECT_EQ(res, 0); // Remove last ballot

    res = c1.removeBallot(&b_temp);
    EXPECT_EQ(res, 1); // Remove non-existent ballot
}

// High load test for adding / removing candidates
TEST_F(CandidateTest, LoadTest)
{
    for (int i = 0; i < 100000; i++)
    {
        c1.addBallot(b1);
    }
    EXPECT_EQ(c1.getNumVotes(), 100000); // Add a load of ballots

    Ballot b;
    int res = 0;
    for (int i = 0; i < 99999; i++)
    {
        res |= c1.removeBallot(&b);
    }
    EXPECT_EQ(c1.getNumVotes(), 1); // Remove a bunch of ballots
    EXPECT_EQ(res, 0);              // Test the return result of each remove

    res = c1.removeBallot(&b);
    res = c1.removeBallot(&b);
    EXPECT_EQ(res, 1); // Remove non-existent ballot
}

// Main to run the tests
// Needed in every test file by the makefile's compilation method
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}