#include "../include/ballot.h"
#include "../include/candidate.h"
#include "../include/election.h"
#include "../include/popularonly.h"
#include "gtest/gtest.h"
#include <cstdlib>

class POSysTest : public ::testing::Test
{
  protected:
    PO po;
};

std::tuple<Candidate, int> run_test(vector<string>& inputFiles)
{
    int ch = chdir("testing/");
    string electionType;
    ifstream firstBallot(inputFiles[0]);
    getline(firstBallot, electionType);
    firstBallot.close();

    if (electionType == "PO") {
        PO *po = new PO();
        string aFileNamePO = po->makeAuditFile("PO");
        ch = chdir("../testing");
        // Parse first header, the following files are assumed to have the same header
        if(po->parseHeader(inputFiles[0]) != 0) {
            Candidate fail;
            std::tuple<Candidate, int> ret(fail, 1);
            return ret;
        }
        for(int i = 0; i < (int) inputFiles.size(); i++)  // Loop over ALL ballot files
        {
            if(po->parseBallots(inputFiles[i]) != 0) {  // Parse and note invalid files
                cout << "Mismatching ballot file information " << inputFiles[i] << ".  Ballot data unused" << endl;
            }
        }
        po->runElection();  // Run election with all info gathered
        // Returns a tuple, first index is winner and second index is if there's an error
        std::tuple<Candidate, int> ret(po->getWinner(), 0);
        return ret;
    }
    else {
        Candidate fail;
        std::tuple<Candidate, int> ret(fail, 2);
        return ret;
    }
}

// A normal test which tests basic system functionality for PO
TEST_F(POSysTest, PONormalSystemTest)
{
    vector<string> inputFiles;
    int ch = chdir("../testing");
    string ballotFileName = "PONormalTest.csv"; // read CSV
    inputFiles.push_back(ballotFileName);
    tuple<Candidate, int> vals = run_test(inputFiles);
    EXPECT_FALSE(2 == get<1>(vals)) << ballotFileName << " was not able to be read";
    EXPECT_FALSE(1 == get<1>(vals)) << "Election Type was not read as PO";
    Candidate winner = get<0>(vals);
    std::string expectedWinner = "Pike";
    EXPECT_TRUE(winner.getName() == expectedWinner) << "Winner Not Correct"; // check winner
}


// Tests a case with a tie
TEST_F(POSysTest, POTieTestSystemTest)
{
    vector<string> inputFiles;
    int ch = chdir("../testing");
    string ballotFileName = "POTieTest.csv"; // read CSV
    inputFiles.push_back(ballotFileName);
    std::tuple<Candidate, int> vals = run_test(inputFiles);
    EXPECT_FALSE(2 == get<1>(vals)) << ballotFileName << " was not able to be read";
    EXPECT_FALSE(1 == get<1>(vals)) << "Election Type was not read as PO";
    Candidate winner = get<0>(vals);
    std::string expectedWinner = "Pike";
    std::string expectedWinner2 = "Foster";
    // Check if winner is either Rosen or Kleinberg in a tie
    EXPECT_TRUE((winner.getName() == expectedWinner) || (winner.getName() == expectedWinner2))
        << "Winner Not Correct"; // Note, this test does not test if the randomness is correct, just that it can
                                 // successfully decide in a tie
}

// Main to run the tests
// Needed in every test file because of the makefile's compilation method
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
