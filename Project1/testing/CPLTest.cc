#include "gtest/gtest.h"
#include "../include/closedpartylist.h"
#include "../include/candidate.h"
#include "../include/ballot.h"
#include "../include/election.h"
#include <cstdlib>

class CPLTest:public::testing::Test {
    public:
        void SetUp() {
            cpl = new CPL();


            
        }
        void TearDown() {
            delete cpl;
        }
    protected:
        CPL *cpl;
};

// TEST_F(CPLTest, CPLConstructorTest) {

// }

// TEST_F(CPLTest, CPLDestructorTest) {
// }

TEST_F(CPLTest, CPLRunElectionTest) {
    int test = cpl->runElection();
    EXPECT_EQ(test, 0);
}
TEST_F(CPLTest, CPLInValidBallotFileName) {
    int test = cpl->runElection();
    EXPECT_NE(test, 0);
}

// TEST_F(CPLTest, CPLReadBallotsTestType) {
//     cpl->readBallots("CPLBallots.csv");
//     ASSERT_EQ(cpl->electionTypeCP, "CPL");
// }

// TEST_F(CPLTest, CPLReadBallotsTestNumParty) {
//     cpl->readBallots("CPLBallots.csv");
//     ASSERT_EQ(cpl->numParties, 6);
// }

// TEST_F(CPLTest, CPLReadBallotsPartyNames) {
//     cpl->readBallots("CPLBallots.csv");
//     ASSERT_EQ(cpl->names[0], "Democratic");
//     ASSERT_EQ(cpl->names[1], "Republican");
//     ASSERT_EQ(cpl->names[2], "New Wave");
//     ASSERT_EQ(cpl->names[3], "Reform");
//     ASSERT_EQ(cpl->names[4], "Green");
//     ASSERT_EQ(cpl->names[5], "Independent");
// }

TEST_F(CPLTest, CPLReadBallotsTestCandidates) {
// Foster, Volz, Pike
// Green, Xu, Wang
// Jacks, Rosen
// McClure, Berg
// Zheng, Melvin
// Peters
    cpl->readBallots("CPLBallots.csv");
    ASSERT_EQ(cpl->parties[0].getMembers()[0].getName(), "Foster");
    ASSERT_EQ(cpl->parties[0].getMembers()[1].getName(), "Volz");
    ASSERT_EQ(cpl->parties[0].getMembers()[2].getName(), "Pike");
    ASSERT_EQ(cpl->parties[1].getMembers()[0].getName(), "Green");
    ASSERT_EQ(cpl->parties[1].getMembers()[1].getName(), "Xu");
    ASSERT_EQ(cpl->parties[1].getMembers()[2].getName(), "Wang");
    ASSERT_EQ(cpl->parties[2].getMembers()[0].getName(), "Jacks");
    ASSERT_EQ(cpl->parties[2].getMembers()[1].getName(), "Rosen");
    ASSERT_EQ(cpl->parties[3].getMembers()[0].getName(), "McClure");
    ASSERT_EQ(cpl->parties[3].getMembers()[1].getName(), "Berg");
    ASSERT_EQ(cpl->parties[4].getMembers()[0].getName(), "Zheng");
    ASSERT_EQ(cpl->parties[4].getMembers()[1].getName(), "Melvin");
    ASSERT_EQ(cpl->parties[5].getMembers()[0].getName(), "Peters");

}

TEST_F(CPLTest, CPLReadBallotsTestNumSeats) {
    cpl->readBallots("CPLBallots.csv");
    ASSERT_EQ(cpl->num_seats, 3);
}

TEST_F(CPLTest, CPLReadBallotsTestNumBallots) {
    cpl->readBallots("CPLBallots.csv");
    ASSERT_EQ(cpl->num_seats, 9);
}

TEST_F(CPLTest, CPLReadBallotsTestBallotTotals) {
    cpl->readBallots("CPLBallots.csv");
    ASSERT_EQ(cpl->parties[0].getBallotTotal(), 3);
    ASSERT_EQ(cpl->parties[1].getBallotTotal(), 2);
    ASSERT_EQ(cpl->parties[2].getBallotTotal(), 0);
    ASSERT_EQ(cpl->parties[3].getBallotTotal(), 2);
    ASSERT_EQ(cpl->parties[4].getBallotTotal(), 1);
    ASSERT_EQ(cpl->parties[5].getBallotTotal(), 1);

}

TEST_F(CPLTest, CPLAssignSeatsTest) {
    cpl->num_seats = 3;
    cpl->parties.resize(6);

    cpl->parties[0].addBallots(3);
    cpl->parties[1].addBallots(2);
    cpl->parties[2].addBallots(0);
    cpl->parties[3].addBallots(2);
    cpl->parties[4].addBallots(1);
    cpl->parties[5].addBallots(1);
    cpl->assignSeats();

    ASSERT_EQ(1, cpl->parties[0].getSeatsWon());
    ASSERT_EQ(1, cpl->parties[1].getSeatsWon());
    ASSERT_EQ(0, cpl->parties[2].getSeatsWon());
    ASSERT_EQ(1, cpl->parties[3].getSeatsWon());
    ASSERT_EQ(0, cpl->parties[4].getSeatsWon());
    ASSERT_EQ(0, cpl->parties[5].getSeatsWon());
}

// TEST_F(CPLTest, CPLGetWinnersTest) {
//     cpl->runElection();
//     // std::vector<Candidate> expected = {"Foster", "Green", "McClure"};
//     ASSERT_EQ(cpl->getWinners(), expected);
// }


// Main to run the tests
// Needed in every test file because of the makefile's compilation method
int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}