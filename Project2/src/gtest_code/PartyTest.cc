#include "../include/party.h"
#include "../include/candidate.h"
#include "gtest/gtest.h"

using namespace std;

class PartyTest : public ::testing::Test
{
  public:
    void SetUp()
    {
        p1 = Party();

        c1 = Candidate("Colton Herta", "Andretti");
        c2 = Candidate("Romain Grosjean", "Andretti");
        c3 = Candidate("Kyle Kirkwood", "Andretti");

        p1.addMember(c1);
        p1.addMember(c2);
        p1.addMember(c3);
    }
    void TearDown()
    {
        // delete pointers from SetUp
    }

  protected:
    Party p1;
    Candidate c1;
    Candidate c2;
    Candidate c3;
};

// PartyTest_1
// Test for getters / setters of BallotTotal
TEST_F(PartyTest, BallotTotalManipulation)
{
    srand(time(0));

    EXPECT_EQ(p1.getBallotTotal(), 0); // 0 ballots
    p1.incBallotTotal();
    EXPECT_EQ(p1.getBallotTotal(), 1); // 1 ballot

    int to_add = rand() % 1000;
    for (int i = 0; i < to_add; i++)
    {
        p1.incBallotTotal();
    }
    EXPECT_EQ(p1.getBallotTotal(), to_add + 1); // random ballots
}

// PartyTest_2
// Testing seat allocation through winSeats and getSeatsWon
TEST_F(PartyTest, SeatManipulation)
{
    int res = p1.winSeats(1);
    EXPECT_EQ(res, 0);              // No overflow
    EXPECT_EQ(p1.getSeatsWon(), 1); // 1 seat won

    res = p1.winSeats(5);
    EXPECT_EQ(res, 3);              // 3 extra seats
    EXPECT_EQ(p1.getSeatsWon(), 3); // Can only win 3 seats with 3 members

    res = p1.winSeats(29);
    EXPECT_EQ(res, 29);             // 29 extra seats
    EXPECT_EQ(p1.getSeatsWon(), 3); // Still only 3 winners max
}

// PartyTest_3
// Test member manipulation
TEST_F(PartyTest, MemberManipulation)
{
    // p1 has 3 andretti added via setup()
    vector<Candidate> andretti = p1.getMembers();
    EXPECT_EQ(andretti.size(), 3);
    EXPECT_EQ(andretti[0].getName(), "Colton Herta");
    EXPECT_EQ(andretti[1].getName(), "Romain Grosjean");
    EXPECT_EQ(andretti[2].getName(), "Kyle Kirkwood");

    Candidate c4 = Candidate("Devlin Defranchesco", "Andretti");
    p1.addMember(c4);
    andretti = p1.getMembers();
    EXPECT_EQ(andretti.size(), 4);
    EXPECT_EQ(andretti[3].getName(), "Devlin Defranchesco");

    Party p2 = Party();
    andretti = p2.getMembers();
    EXPECT_EQ(andretti.size(), 0); // Empty party
}

// PartyTest_4
// Test getWinners
TEST_F(PartyTest, getWinners)
{
    vector<string> winners;
    winners = p1.getWinners();
    EXPECT_EQ(winners.size(), 0); // 0 winners

    p1.winSeats(2);
    winners = p1.getWinners();
    EXPECT_EQ(winners.size(), 2); // 2 winners
    EXPECT_EQ(winners[0], "Colton Herta");
    EXPECT_EQ(winners[1], "Romain Grosjean");

    p1.winSeats(99); // potential overflow in winners
    winners = p1.getWinners();
    EXPECT_EQ(winners.size(), 3); // 3 winners
    EXPECT_EQ(winners[0], "Colton Herta");
    EXPECT_EQ(winners[1], "Romain Grosjean");
    EXPECT_EQ(winners[2], "Kyle Kirkwood");
}

// Main to run the tests
// Needed in every test file because of the makefile's compilation method
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
