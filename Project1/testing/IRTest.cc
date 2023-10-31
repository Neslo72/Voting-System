#include "gtest/gtest.h"
#include "../include/ir.h"
#include "../include/candidate.h"
#include "../include/ballot.h"
#include <cstdlib>
#include <vector>

using namespace std;

class IRTest:public::testing::Test {
    public:
        void SetUp() {
            ir1 = new IR();

            can1 = new Candidate("Can1", "Up");
            can2 = new Candidate("Can2", "Down");
            can3 = new Candidate("Can3", "Middle");

            Ballot *b1 = new Ballot();
            Ballot *b2 = new Ballot();
            Ballot *b3 = new Ballot();
            Ballot *b4 = new Ballot();
            Ballot *b5 = new Ballot();
            Ballot *b6 = new Ballot();

            b1->addChoice(1);
            b2->addChoice(1);
            b3->addChoice(1);
            b4->addChoice(2);
            b5->addChoice(2);
            b6->addChoice(3);

            can1->addBallot(*b1);
            can1->addBallot(*b4);
            can1->addBallot(*b6);

            can2->addBallot(*b2);
            can2->addBallot(*b5);

            can3->addBallot(*b3);

            candidates.push_back(can1);
            candidates.push_back(can2);
            candidates.push_back(can3);
        }
        void TearDown() {
        }
    protected:
        IR* ir1;
        Candidate* can1;
        Candidate* can2;
        Candidate* can3;
        std::vector<Candidate*> candidates;
};

TEST_F(IRTest, IRRunElectionTest) {
    // run election
    string ballotFileName = "IRBallots.csv";
    ir1->runElection();

    // check number of candidates
    EXPECT_EQ(3, candidates.size()) << "IR Run Election does not have accurate number of candidates";

    // check number of votes for each candidate
    EXPECT_EQ(3, candidates[0]->getNumVotes()) << "IR Run Election does not have accurate number of votes for candidate"; 
    EXPECT_EQ(2, candidates[1]->getNumVotes()) << "IR Run Election does not have accurate number of votes for candidate"; 
    EXPECT_EQ(1, candidates[2]->getNumVotes()) << "IR Run Election does not have accurate number of votes for candidate";

    // check total number of votes
    int total = 0;
    total = candidates[0]->getNumVotes() + candidates[1]->getNumVotes() + candidates[2]->getNumVotes();
    EXPECT_EQ(6, total) << "IR Run Election does not total number of votes correctly";

    // Equal number of votes for each candidate
    Ballot *b7 = new Ballot();
    Ballot *b8 = new Ballot();
    Ballot *b9 = new Ballot();
    b7->addChoice(2);
    b8->addChoice(3);
    b9->addChoice(3);
    can2->addBallot(*b7);
    can3->addBallot(*b8);
    can3->addBallot(*b9);
}
/*
TEST_F(IRTest, IRBreakTieTest) {
    // ir1->breakTie(candidates);
    Ballot *b7 = new Ballot();
    b7->addChoice("can2");
    can2->addBallot(*b7);
    // EXPECT_EQ({can1, can1}, tieVector) << "IR Break Tie is not provided with a valid tieVector";
}


TEST_F(IRTest, IRReadBallotsTest) {
    string name = "IRBallots.csv";
    EXPECT_EQ(0, ir1->readBallots(name)) << "IR Read Ballots does not operate correctly";
}
*/

TEST_F(IRTest, IRGetCandidateIndexTest) {
    EXPECT_EQ(0, ir1->getCandidateIndex("can1")) << "IR Get Candidate Index does not work properly";
    EXPECT_EQ(1, ir1->getCandidateIndex("can2")) << "IR Get Candidate Index does not work properly";
    EXPECT_EQ(2, ir1->getCandidateIndex("can3")) << "IR Get Candidate Index does not work properly";
    EXPECT_EQ(-1, ir1->getCandidateIndex("jimmy")) << "IR Get Candidate Index does not work properly";
}

TEST_F(IRTest, IRRedistributeVotesTest) {
    ir1->redistributeVotes(can3);
    EXPECT_EQ(4, candidates[0]->getNumVotes()) << "IR Redistribution of Votes does not work properly";
}


// Main to run the tests
// Needed in every test file because of the makefile's compilation method
int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
