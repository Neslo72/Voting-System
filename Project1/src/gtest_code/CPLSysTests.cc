#include "../include/ballot.h"
#include "../include/candidate.h"
#include "../include/closedpartylist.h"
#include "../include/election.h"
#include "gtest/gtest.h"
#include <cstdlib>

class CPLSysTest : public ::testing::Test
{
  public:
    void SetUp()
    {
        cpl1 = new CPL();
        cpl2 = new CPL();
        cpl3 = new CPL();
        cpl4 = new CPL();
    }
    void TearDown()
    {
        delete cpl1;
        delete cpl2;
        delete cpl3;
        delete cpl4;
    }

  protected:
    CPL *cpl1;
    CPL *cpl2;
    CPL *cpl3;
    CPL *cpl4;
};

std::tuple<std::vector<Candidate>, int> run_test(std::string ballotFileName, CPL *cpl)
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
        if (electionType == "CPL")
        {
            election = new Election(ballotFile, electionType);
            string aFileNameCPL = election->makeAuditFile(electionType);
            int reading = cpl->readBallots(ballotFileName, aFileNameCPL); // Read ballots and run election
            if (reading == 0)
            {
                cpl->runElection(aFileNameCPL);
            }
            std::tuple<std::vector<Candidate>, int> ret(
                cpl->getWinners(),
                0); // Returns a tuple, first index is winner vector second index is if there is an error
            int ch = chdir("src/");
            return ret;
        }
        else
        {
            std::vector<Candidate> fail;
            std::tuple<std::vector<Candidate>, int> ret(fail, 1);
            int ch = chdir("src/");
            return ret;
        }
    }
    else
    {
        std::vector<Candidate> fail;
        std::tuple<std::vector<Candidate>, int> ret(fail, 2);
        int ch = chdir("src/");
        return ret;
    }
}

// Test normal CPL case
TEST_F(CPLSysTest, NormalCPLSystemTest)
{
    std::string ballotFileName = "testing/CPLNormalTest.csv";
    std::tuple<std::vector<Candidate>, int> vals = run_test(ballotFileName, cpl1);

    EXPECT_FALSE(2 == get<1>(vals)) << ballotFileName << " was not able to be read";
    EXPECT_FALSE(1 == get<1>(vals)) << "Election Type was not read as CPL";
    std::vector<Candidate> winners = get<0>(vals);
    std::string winner1 = "Foster";
    std::string winner2 = "Green";
    std::string winner3 = "McClure";
    EXPECT_TRUE(winners[0].getName() == winner1) << "Winner 1 Not Correct";
    EXPECT_TRUE(winners[1].getName() == winner2) << "Winner 2 Not Correct";
    EXPECT_TRUE(winners[2].getName() == winner3) << "Winner 3 Not Correct";
}

// Test case where there is a tie but no remainders
TEST_F(CPLSysTest, TieNoRemainderCPLSystemTest)
{
    std::string ballotFileName = "testing/CPLNoRemainderTieTest.csv";
    std::tuple<std::vector<Candidate>, int> vals = run_test(ballotFileName, cpl2);

    EXPECT_FALSE(2 == get<1>(vals)) << ballotFileName << " was not able to be read";
    EXPECT_FALSE(1 == get<1>(vals)) << "Election Type was not read as CPL";

    std::vector<Candidate> winners = get<0>(vals);
    std::string winner1 = "Foster";
    std::string winner2 = "Green";
    std::string winner3 = "Jacks";
    EXPECT_TRUE(winners[0].getName() == winner1) << "Winner 1 Not Correct";
    EXPECT_TRUE(winners[1].getName() == winner2) << "Winner 2 Not Correct";
    EXPECT_TRUE(winners[2].getName() == winner3) << "Winner 3 Not Correct";
}

// Test case where one party wins all votes
TEST_F(CPLSysTest, WipeoutCPLSystemTest)
{
    std::string ballotFileName = "testing/CPLWipeoutTest.csv";
    std::tuple<std::vector<Candidate>, int> vals = run_test(ballotFileName, cpl3);

    EXPECT_FALSE(2 == get<1>(vals)) << ballotFileName << " was not able to be read";
    EXPECT_FALSE(1 == get<1>(vals)) << "Election Type was not read as CPL";
    std::vector<Candidate> winners = get<0>(vals);
    std::string winner1 = "Foster";
    std::string winner2 = " Volz";
    std::string winner3 = " Pike";

    EXPECT_TRUE(winners[0].getName() == winner1) << "Winner 1 Not Correct";
    EXPECT_TRUE(winners[1].getName() == winner2) << "Winner 2 Not Correct";
    EXPECT_TRUE(winners[2].getName() == winner3) << "Winner 3 Not Correct";
}

// Test case where no candidate gets to boundary
TEST_F(CPLSysTest, PureTieCPLSystemTest)
{
    std::string ballotFileName = "testing/CPLPureTieTest.csv";
    std::tuple<std::vector<Candidate>, int> vals = run_test(ballotFileName, cpl4);

    EXPECT_FALSE(2 == get<1>(vals)) << ballotFileName << " was not able to be read";
    EXPECT_FALSE(1 == get<1>(vals)) << "Election Type was not read as CPL";
    std::vector<Candidate> winners = get<0>(vals);
    std::string secondcand1 = " Volz";
    std::string secondcand2 = " Xu";
    std::string secondcand3 = " Rosen";
    std::string secondcand4 = " Berg";
    EXPECT_TRUE((winners[0].getName() != winners[1].getName())) << "A winner won twice"; // Check if anyone won twice
    for (int i = 0; i < 2; i++)
    { // Check if any secondary candidates won
        EXPECT_TRUE((winners[i].getName() != secondcand1))
            << "A party won two seats when it should receive a max of 1 seat";
        EXPECT_TRUE((winners[i].getName() != secondcand2))
            << "A party won two seats when it should receive a max of 1 seat";
        EXPECT_TRUE((winners[i].getName() != secondcand3))
            << "A party won two seats when it should receive a max of 1 seat";
        EXPECT_TRUE((winners[i].getName() != secondcand4))
            << "A party won two seats when it should receive a max of 1 seat";
    }
}

// Main to run the tests
// Needed in every test file because of the makefile's compilation method
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
