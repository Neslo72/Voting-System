#include "gtest/gtest.h"
#include "../include/candidate.h"

class CandidateTest:public::testing::Test {
    public:
        void SetUp() {
            c1 = Candidate("Joe Jorgenson","Democrat");
        }
        void TearDown() {
            // delete pointers from SetUp
        }
    protected:
        // define any variables here
        Candidate c1;
};


TEST_F(CandidateTest, CandidateAddRemoveBallotTest) {
    Ballot testBal = Ballot();    
    c1.addBallot(testBal);
    Ballot* bal;
    int res = c1.removeBallot(bal);
    EXPECT_EQ(res,0);
//     EXPECT_FALSE(bal, NULL);
}

TEST_F(CandidateTest, BasicExampleTest) {
    Ballot *b1 = new Ballot();
    EXPECT_EQ(2, 2);
}


// Main to run the tests
// Needed in every test file by the makefile's compilation method
int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}