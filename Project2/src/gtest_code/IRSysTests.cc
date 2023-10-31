#include "../include/ballot.h"
#include "../include/candidate.h"
#include "../include/election.h"
#include "../include/ir.h"
#include "gtest/gtest.h"
#include <cstdlib>

class IRSysTest : public ::testing::Test
{
  protected:
    IR ir;
};

std::tuple<Candidate, int> run_test(vector<string>& inputFiles)
{
    int ch = chdir("testing/");
    string electionType;
    ifstream firstBallot(inputFiles[0]);
    getline(firstBallot, electionType);
    firstBallot.close();

    if (electionType == "IR") {
        IR *ir = new IR();
        string aFileNameIR = ir->makeAuditFile("IR");
        ch = chdir("..testing/");
        // Parse first header, the following files are assumed to have the same header
        if(ir->parseHeader(inputFiles[0]) != 0) {
            Candidate fail;
            std::tuple<Candidate, int> ret(fail, 1);
            return ret;
        }
        for(int i = 0; i < (int) inputFiles.size(); i++)  // Loop over ALL ballot files
        {
            if(ir->parseBallots(inputFiles[i]) != 0) {  // Parse and note invalid files
                cout << "Mismatching ballot file information " << inputFiles[i] << ".  Ballot data unused" << endl;
            }
        }
        ir->runElection();  // Run election with all info gathered
        // Returns a tuple, first index is winner and second index is if there's an error
        std::tuple<Candidate, int> ret(ir->getWinner(), 0);
        return ret;
    }
    else {
        Candidate fail;
        std::tuple<Candidate, int> ret(fail, 2);
        return ret;
    }
}

// A normal test which tests basic system functionality for IR
TEST_F(IRSysTest, IRNormalSystemTest)
{
    vector<string> inputFiles;
    int ch = chdir("../testing");
    string ballotFileName = "IRNormalTest.csv"; // read CSV
    inputFiles.push_back(ballotFileName);
    tuple<Candidate, int> vals = run_test(inputFiles);
    EXPECT_FALSE(2 == get<1>(vals)) << ballotFileName << " was not able to be read";
    EXPECT_FALSE(1 == get<1>(vals)) << "Election Type was not read as IR";
    Candidate winner = get<0>(vals);
    std::string expectedWinner = "Rosen ";
    EXPECT_TRUE(winner.getName() == expectedWinner) << "Winner Not Correct"; // check winner
}

// Tests a case where one candidate wins all votes
TEST_F(IRSysTest, IRWipeoutSystemTest)
{
    vector<string> inputFiles;
    int ch = chdir("../testing");
    string ballotFileName = "IRWipeoutTest.csv"; // read CSV
    inputFiles.push_back(ballotFileName);
    std::tuple<Candidate, int> vals = run_test(inputFiles);
    EXPECT_FALSE(2 == get<1>(vals)) << ballotFileName << " was not able to be read";
    EXPECT_FALSE(1 == get<1>(vals)) << "Election Type was not read as IR";
    Candidate winner = get<0>(vals);
    std::string expectedWinner = "Rosen ";
    EXPECT_TRUE(winner.getName() == expectedWinner) << "Winner Not Correct"; // Check winner
}

// Tests a case with a tie
TEST_F(IRSysTest, IRTieTestSystemTest)
{
    vector<string> inputFiles;
    int ch = chdir("../testing");
    string ballotFileName = "IRTieTest.csv"; // read CSV
    inputFiles.push_back(ballotFileName);
    std::tuple<Candidate, int> vals = run_test(inputFiles);
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
    vector<string> inputFiles;
    int ch = chdir("../testing");
    string ballotFileName = "IRMultipleElimTest.csv"; // read CSV
    inputFiles.push_back(ballotFileName);
    std::tuple<Candidate, int> vals = run_test(inputFiles);
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
