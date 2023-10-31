#include "../include/ballot.h"
#include "../include/candidate.h"
#include "../include/closedpartylist.h"
#include "../include/election.h"
#include "../include/party.h" // Addition
#include "gtest/gtest.h"
#include <cstdlib>

class CPLTest : public ::testing::Test
{
  protected:
    CPL *cpl = new CPL();
};

// Test election with no candidates
TEST_F(CPLTest, CPLRunElectionTest)
{
    int ch = chdir("../");
    if (ch != 0)
    {
        cout << "Error" << endl;
    }
    string aFileNameCPL = cpl->makeAuditFile("CPL");
    int test = cpl->runElection();
    EXPECT_NE(test, 0);
}

/*
// Test readBallots
TEST_F(CPLTest, CPLReadBallotsTestType)
{
    int ch = chdir("src/gtest_code");
    cpl.readBallots("testing/CPL.csv", auditFileName);
    EXPECT_EQ(cpl.getElectionTypeCP(), "CPL");
}

// Test readBallots and check number of parties
TEST_F(CPLTest, CPLReadBallotsTestNumParty)
{
    int ch = chdir("src/gtest_code");
    cpl.readBallots("testing/CPL.csv", auditFileName);
    EXPECT_EQ(cpl.parties.size(), 6);
}

// Test readBallots and check names of parties
TEST_F(CPLTest, CPLReadBallotsPartyNames)
{
    int ch = chdir("src/gtest_code");
    cpl.readBallots("testing/CPL.csv", auditFileName);
    vector<string> names1;
    names1.resize(6);
    names1 = cpl.getNames();
    ASSERT_EQ(names1[0], "Democratic");
    ASSERT_EQ(names1[1], " Republican");
    ASSERT_EQ(names1[2], " New Wave");
    ASSERT_EQ(names1[3], " Reform");
    ASSERT_EQ(names1[4], " Green");
    ASSERT_EQ(names1[5], " Independent");
}

// Test readBallots and check names of candidates
TEST_F(CPLTest, CPLReadBallotsTestCandidates)
{
    // Foster, Volz, Pike
    // Green, Xu, Wang
    // Jacks, Rosen
    // McClure, Berg
    // Zheng, Melvin
    // Peters
    int ch = chdir("src/gtest_code");
    cpl.readBallots("testing/CPL.csv", auditFileName);
    ASSERT_EQ(cpl.parties[0].getMembers()[0].getName(), "Foster");
    ASSERT_EQ(cpl.parties[0].getMembers()[1].getName(), " Volz");
    ASSERT_EQ(cpl.parties[0].getMembers()[2].getName(), " Pike");
    ASSERT_EQ(cpl.parties[1].getMembers()[0].getName(), "Green");
    ASSERT_EQ(cpl.parties[1].getMembers()[1].getName(), " Xu");
    ASSERT_EQ(cpl.parties[1].getMembers()[2].getName(), " Wang");
    ASSERT_EQ(cpl.parties[2].getMembers()[0].getName(), "Jacks");
    ASSERT_EQ(cpl.parties[2].getMembers()[1].getName(), " Rosen");
    ASSERT_EQ(cpl.parties[3].getMembers()[0].getName(), "McClure");
    ASSERT_EQ(cpl.parties[3].getMembers()[1].getName(), " Berg");
    ASSERT_EQ(cpl.parties[4].getMembers()[0].getName(), "Zheng");
    ASSERT_EQ(cpl.parties[4].getMembers()[1].getName(), " Melvin");
    ASSERT_EQ(cpl.parties[5].getMembers()[0].getName(), "Peters");
}

// Test read ballots and check the number of seats
TEST_F(CPLTest, CPLReadBallotsTestNumSeats)
{
    int ch = chdir("src/gtest_code");
    cpl.readBallots("testing/CPL.csv", auditFileName);
    ASSERT_EQ(cpl.getNumSeats(), "3");
}

// Test read ballots and check the number of ballots
TEST_F(CPLTest, CPLReadBallotsTestNumBallots)
{
    int ch = chdir("src/gtest_code");
    cpl.readBallots("testing/CPL.csv", auditFileName);
    ASSERT_EQ(cpl.getNumBallots(), 9);
}

// Test read ballots and check number of ballots for each party
TEST_F(CPLTest, CPLReadBallotsTestBallotTotals)
{
    int ch = chdir("src/gtest_code");
    cpl.readBallots("testing/CPL.csv", auditFileName);
    ASSERT_EQ(cpl.parties[0].getBallotTotal(), 3);
    ASSERT_EQ(cpl.parties[1].getBallotTotal(), 2);
    ASSERT_EQ(cpl.parties[2].getBallotTotal(), 0);
    ASSERT_EQ(cpl.parties[3].getBallotTotal(), 2);
    ASSERT_EQ(cpl.parties[4].getBallotTotal(), 1);
    ASSERT_EQ(cpl.parties[5].getBallotTotal(), 1);
}

// Test breakTie
TEST_F(CPLTest, CPLBreakTieTest) {
    CPL * cplTie = new CPL();
    Party democrat = Party();
    Party republican = Party();
    Candidate candidate("Foster", "Democratic");
    Candidate candidate1("Green", "Republican");
    democrat.addMember(candidate);
    republican.addMember(candidate1);
    democrat.partyName = "democrat";
    republican.partyName = "republican";
    // vector<int> results = {0,0};
    int results[2];
    results[0] = 0;
    results[1] = 0;

    int win;
    //0 represent democrat win, 1 represents republican
    for(int i = 0; i < 100; i++){
        vector <Party *> tieVector;
        tieVector.push_back(&democrat);
        tieVector.push_back(&republican);
        Party *result = cplTie->breakTie(tieVector);
        //cout << "test " << result->partyName;
        if(result->partyName == "democrat") {
            results[0]++;
        }
        else if(result->partyName == "republican") {
            results[1]++;
        }
    }
    // Expect results for random wins to be in the range of 40 < 60
    // If it is not on one test, re-run the test and it will likely be in this range
    if((results[0] >= 40 && results[0] <= 60) && (results[1] >= 40 && results[1] <= 60)){
        win = 1;
    }
    else
    {
        win = 0;
    }
    EXPECT_EQ(1, win);
}

// Tests AssignSeats (fails)
// Potential bug in getHighestRemainder() ?
TEST_F(CPLTest, CPLAssignSeatsTest) {
    cpl.num_seats = 3;  // This acutally works...
    cpl.parties.resize(6);

    cpl.parties[0].addBallots(3);
    cpl.parties[0].addMember(Candidate("test1", "party"));
    cpl.parties[1].addBallots(2); // 2
    cpl.parties[1].addMember(Candidate("test2", "party2"));
    cpl.parties[2].addBallots(0);
    cpl.parties[2].addMember(Candidate("test3", "party3"));
    cpl.parties[3].addBallots(2); // 2
    cpl.parties[3].addMember(Candidate("test4", "party4"));;
    cpl.parties[4].addBallots(1);
    cpl.parties[4].addMember(Candidate("test5", "party5"));
    cpl.parties[5].addBallots(1);
    cpl.parties[5].addMember(Candidate("test6", "party6"));

    cpl.assignSeats();

    ASSERT_EQ(1, cpl.parties[0].getSeatsWon());
    ASSERT_EQ(1, cpl.parties[1].getSeatsWon());
    ASSERT_EQ(0, cpl.parties[2].getSeatsWon());
    ASSERT_EQ(1, cpl.parties[3].getSeatsWon());  // Test fails, problem with getHighestRemainder??
    ASSERT_EQ(0, cpl.parties[4].getSeatsWon());
    ASSERT_EQ(0, cpl.parties[5].getSeatsWon());
}
*/
// Main to run the tests
// Needed in every test file because of the makefile's compilation method
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}