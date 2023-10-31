#include "gtest/gtest.h"
#include "../include/party.h"
#include "../include/candidate.h"

class PartyTest:public::testing::Test {
    public:
        void SetUp() {
            p1 = Party();
        }
        void TearDown() {
            // delete pointers from SetUp
        }
    protected:
        Party p1;
};

TEST_F(PartyTest, PartyAddMemberTest) {
    Candidate c1 = Candidate("Joe Jorganson", "Democrat");
    p1.addMember(c1);
    EXPECT_EQ(1,p1.getNumMembers());

}

TEST_F(PartyTest, BasicExampleTest) {
    EXPECT_EQ(2, 2);
}

// Main to run the tests
// Needed in every test file because of the makefile's compilation method
int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
