#include "../include/ballot.h"
#include "../include/candidate.h"
#include "../include/election.h"
#include "../include/ir.h"
#include "gtest/gtest.h"
#include <cstdlib>

class IRSysTest : public ::testing::Test
{
  public:
    void SetUp()
    {
        ir1 = new IR();
        ir2 = new IR();
        ir3 = new IR();
        ir4 = new IR();
    }
    void TearDown()
    {
        delete ir1;
        delete ir2;
        delete ir3;
        delete ir4;
    }

  protected:
    IR *ir1;
    IR *ir2;
    IR *ir3;
    IR *ir4;
};

std::tuple<Candidate, int> run_test(std::string ballotFileName, IR *ir)
{
    fstream ballotFile;
    string electionType;
    Election *election;
    int ch = chdir("../");
    if (ch == 0)
    {
        // Attempt to open the ballot file with the provided ballot file name
        ballotFile.open(ballotFileName.c_str(), std::ios::in);
    }
    // Test if the ballot file failed to open
    // If it does fail, the user is prompted to re-enter the ballot file
    if (ballotFile.is_open())
    {
        // Get the first line of the ballot file to get the election type
        getline(ballotFile, electionType);
        // If IR election type, create a new IR object to run the election,
        // print the results, and write election information to the audit file
        if (electionType == "IR")
        {
            election = new Election(ballotFile, electionType);
            string aFileNameIR = election->makeAuditFile(electionType);
            IR *ir = new IR();
            vector<Candidate *> cansFromBallots = ir->readBallots(ballotFileName, aFileNameIR); // Read Ballots
            if (cansFromBallots.size() != 0)
            {
                ir->runElection(cansFromBallots, aFileNameIR); // Run the election
            }
            std::tuple<Candidate, int> ret(
                ir->getWinner(), 0); // Returns a tuple, first index is winner second index is if there is an error
            int ch = chdir("src/");
            return ret;
        }
        else
        {
            Candidate fail;
            std::tuple<Candidate, int> ret(fail, 1); // Election type read incorrectly
            int ch = chdir("src/");
            return ret;
        }
    }
    else
    {
        Candidate fail;
        std::tuple<Candidate, int> ret(fail, 2); // Cant open file
        int ch = chdir("src/");
        return ret;
    }
}
// A normal test which tests basic system functionality for IR
TEST_F(IRSysTest, IRNormalSystemTest)
{
    std::string ballotFileName = "testing/IRNormalTest.csv"; // read CSV
    std::tuple<Candidate, int> vals = run_test(ballotFileName, ir1);
    EXPECT_FALSE(2 == get<1>(vals)) << ballotFileName << " was not able to be read";
    EXPECT_FALSE(1 == get<1>(vals)) << "Election Type was not read as IR";
    Candidate winner = get<0>(vals);
    std::string expectedWinner = "Rosen ";
    EXPECT_TRUE(winner.getName() == expectedWinner) << "Winner Not Correct"; // check winner
}

// Tests a case where one candidate wins all votes
TEST_F(IRSysTest, IRWipeoutSystemTest)
{
    std::string ballotFileName = "testing/IRWipeoutTest.csv"; // read CSV
    std::tuple<Candidate, int> vals = run_test(ballotFileName, ir2);

    EXPECT_FALSE(2 == get<1>(vals)) << ballotFileName << " was not able to be read";
    EXPECT_FALSE(1 == get<1>(vals)) << "Election Type was not read as IR";
    Candidate winner = get<0>(vals);
    std::string expectedWinner = "Rosen ";
    EXPECT_TRUE(winner.getName() == expectedWinner) << "Winner Not Correct"; // Check winner
}

// Tests a case with a tie
TEST_F(IRSysTest, IRTieTestSystemTest)
{
    std::string ballotFileName = "testing/IRTieTest.csv";
    std::tuple<Candidate, int> vals = run_test(ballotFileName, ir3);

    EXPECT_FALSE(2 == get<1>(vals)) << ballotFileName << " was not able to be read";
    EXPECT_FALSE(1 == get<1>(vals)) << "Election Type was not read as IR";
    Candidate winner = get<0>(vals);
    std::string expectedWinner = "Rosen ";
    std::string expectedWinner2 = "Kleinberg ";
    // Check if winner is either Rosen or Kleinberg in a tie
    EXPECT_TRUE((winner.getName() == expectedWinner) || (winner.getName() == expectedWinner2))
        << "Winner Not Correct"; // Note, this test does not test if the randomness is correct, just that it can
                                 // successfully decide in a tie
}

// Test multiple eliminations
TEST_F(IRSysTest, IRMultipleElimTest)
{
    std::string ballotFileName = "testing/IRMultipleElimTest.csv";
    std::tuple<Candidate, int> vals = run_test(ballotFileName, ir3);

    EXPECT_FALSE(2 == get<1>(vals)) << ballotFileName << " was not able to be read";
    EXPECT_FALSE(1 == get<1>(vals)) << "Election Type was not read as IR";
    Candidate winner = get<0>(vals);
    std::string expectedWinner = " Marcus";

    EXPECT_TRUE((winner.getName() == expectedWinner)) << "Winner Not Correct"; // Check winner
}

// Main to run the tests
// Needed in every test file because of the makefile's compilation method
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
