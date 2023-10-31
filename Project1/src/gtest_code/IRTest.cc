#include "../include/ir.h"
#include "../include/ballot.h"
#include "../include/candidate.h"
#include "../include/election.h"
#include "gtest/gtest.h"
#include <cstdlib>
#include <vector>

class IRTest : public ::testing::Test
{
  protected:
    // ir election
    IR ir;
    // ** NEEDS TO CHANGE DEPENDING ON DATE
    string auditFileName = "IR_3292023";
};

// Check that the election requires candidates
TEST_F(IRTest, IRRunElectionNoCan)
{
    // empty candidates vector
    std::vector<Candidate *> candidates = {};
    int ch = chdir("../");
    if (ch != 0)
    {
        cout << "Error" << endl;
    }
    EXPECT_EQ(ir.runElection(candidates, auditFileName), -1);
}

// Check that the election requires ballots
TEST_F(IRTest, IRRunElectionOneCanOneBalEmpty)
{
    // one candidate
    string name = "Arnold";
    string party = "Up";
    std::vector<Candidate *> candidates;
    candidates.push_back(new Candidate(name, party));
    // one ballot but not assigned to any candidate
    Ballot b;
    EXPECT_EQ(ir.runElection(candidates, auditFileName), -1);
}

// Check an election with one candidate and one ballot
TEST_F(IRTest, IRRunElectionOneCanOneBal)
{
    // one candidate
    string name = "Arnold";
    string party = "Up";
    std::vector<Candidate *> candidates;
    Candidate *can1 = new Candidate(name, party);
    candidates.push_back(can1);
    // one ballot
    Ballot b;
    candidates[0]->addBallot(b);
    // check if election runs successfully
    EXPECT_EQ(ir.runElection(candidates, auditFileName), 0) << endl;
    // check winner of election
    Candidate winner = ir.getWinner();
    EXPECT_EQ(winner.getName(), "Arnold") << endl;
}

// Check an election with two candidates and two ballots
TEST_F(IRTest, IRRunElectionTwoCanTwoBal)
{
    // two candidates
    string name1 = "Arnold";
    string party1 = "Up";
    string name2 = "Gerald";
    string party2 = "Down";
    std::vector<Candidate *> candidates;
    candidates.push_back(new Candidate(name1, party1));
    candidates.push_back(new Candidate(name2, party2));
    // two ballots
    Ballot b1;
    Ballot b2;
    candidates[0]->addBallot(b1);
    candidates[0]->addBallot(b2);
    // check if election runs successfully
    EXPECT_EQ(ir.runElection(candidates, auditFileName), 0) << endl;
    // check winner of election
    Candidate winner = ir.getWinner();
    EXPECT_EQ(winner.getName(), "Arnold") << endl;
}

// Check an election with three candidates and three ballots
TEST_F(IRTest, IRRunElectionThreeCanThreeBal)
{
    // three candidates
    string name1 = "Arnold";
    string party1 = "Up";
    string name2 = "Gerald";
    string party2 = "Down";
    string name3 = "Marvin";
    string party3 = "Middle";
    std::vector<Candidate *> candidates;
    candidates.push_back(new Candidate(name1, party1));
    candidates.push_back(new Candidate(name2, party2));
    candidates.push_back(new Candidate(name3, party3));
    // three ballots
    Ballot b1;
    Ballot b2;
    Ballot b3;
    candidates[0]->addBallot(b1);
    candidates[1]->addBallot(b2);
    candidates[1]->addBallot(b3);
    // check if election runs successfully
    EXPECT_EQ(ir.runElection(candidates, auditFileName), 0) << endl;
    // check winner of election
    Candidate winner = ir.getWinner();
    EXPECT_EQ(winner.getName(), "Gerald");
}

// Check an election with two candidates and two ballots that are tied
// Loops 10,000 times to ensure the outcome of a tie is random
// DO NOT RUN THIS TEST.  The test works but creates a ton of output (over 8GB)
/*
TEST_F(IRTest, IRRunElectionTwoCandTie) {
    // two candidates
    string name1 = "Arnold";
    string party1 = "Up";
    string name2 = "Gerald";
    string party2 = "Down";
    std::vector<Candidate*> candidates;
    candidates.push_back(new Candidate(name1, party1));
    candidates.push_back(new Candidate(name2, party2));
    // four ballots
    Ballot b1;
    Ballot b2;
    Ballot b3;
    Ballot b4;
    candidates[0]->addBallot(b1);
    candidates[0]->addBallot(b2);
    candidates[1]->addBallot(b3);
    candidates[1]->addBallot(b4);
    // results vector to hold winners
    std::vector<Candidate> results;
    // run election 10,000 times and record the winner
    for (int i = 0; i < 10000; i++) {
        ir.runElection(candidates, auditFileName);
        Candidate winner = ir.getWinner();
        results.push_back(winner);
    }
    int c = 0;
    // loop through the results vector and check that candidate 1 has a random number of wins
    for (int i = 0; i < results.size(); i++) {
        if (name1.compare(results[i].getName()) == 0) {
            c++;
        }
    }
    EXPECT_TRUE((c >= 4500) && (c <= 5500));
}
*/


// Main to run the tests
// Needed in every test file because of the makefile's compilation method
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}