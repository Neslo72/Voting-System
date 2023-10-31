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
};

// Check that a header is parsed
TEST_F(IRTest, ParseHeaderTest) {
    ir.makeAuditFile("IR");
    string fileName = "IR.csv";
    int ch = chdir("../testing/");
    int result = ir.parseHeader(fileName);
    ir.writeToAuditFile("Parsing header test.\n");
    EXPECT_EQ(0, result);
}

// Check that ballots are parsed
TEST_F(IRTest, ParseBallotsTest) {
    ir.makeAuditFile("IR");
    int ch = chdir("../testing/");
    string fileName = "IR.csv";
    ir.parseHeader(fileName);
    int result = ir.parseBallots(fileName);
    ir.writeToAuditFile("Parsing ballots test.\n");
    EXPECT_EQ(0, result);
}

// Check that the election requires candidates
TEST_F(IRTest, IRRunElectionNoCandidates)
{
    ir.makeAuditFile("IR");
    int ch = chdir("../testing/");
    string fileName = "IRNoCans.csv";
    int header = ir.parseHeader(fileName);
    EXPECT_EQ(0, header);
    int ballots = ir.parseBallots(fileName);
    EXPECT_EQ(1, ballots);
    ir.writeToAuditFile("No candidates test.\n");
    int result = ir.runElection();
    EXPECT_EQ(-1, result);
}

// Check an election with one candidate and one ballot
TEST_F(IRTest, IRRunElectionOneCanOneBal)
{
    ir.makeAuditFile("IR");
    int ch = chdir("../testing/");
    string fileName = "IROneCan.csv";
    int header = ir.parseHeader(fileName);
    EXPECT_EQ(0, header);
    int ballots = ir.parseBallots(fileName);
    EXPECT_EQ(0, ballots);
    ir.writeToAuditFile("One candidate with one ballot test.\n");
    int result = ir.runElection();
    EXPECT_EQ(0, result);
    EXPECT_EQ(ir.getWinner().getName(), "Rosen ");
}

// Check an election with two candidates and two ballots
TEST_F(IRTest, IRRunElectionTwoCanTwoBal)
{
    ir.makeAuditFile("IR");
    int ch = chdir("../testing/");
    string fileName = "IRTwoCan.csv";
    int header = ir.parseHeader(fileName);
    EXPECT_EQ(0, header);
    int ballots = ir.parseBallots(fileName);
    EXPECT_EQ(0, ballots);
    ir.writeToAuditFile("Two candidate with two ballot test.\n");
    int result = ir.runElection();
    EXPECT_EQ(0, result);
    EXPECT_EQ(ir.getWinner().getName(), "Rosen ");
}

// Check an election with three candidates and three ballots
TEST_F(IRTest, IRRunElectionThreeCanThreeBal)
{
    ir.makeAuditFile("IR");
    int ch = chdir("../testing/");
    string fileName = "IRThreeCan.csv";
    int header = ir.parseHeader(fileName);
    EXPECT_EQ(0, header);
    int ballots = ir.parseBallots(fileName);
    EXPECT_EQ(0, ballots);
    ir.writeToAuditFile("Two candidate with two ballot test.\n");
    int result = ir.runElection();
    EXPECT_EQ(0, result);
    EXPECT_EQ(ir.getWinner().getName(), " Kleinberg ");
}

// Check an election with multiple eliminations
TEST_F(IRTest, IRRunElectionMultipleElim)
{
    ir.makeAuditFile("IR");
    int ch = chdir("../testing/");
    string fileName = "IRMultipleElimTest.csv";
    int header = ir.parseHeader(fileName);
    EXPECT_EQ(0, header);
    int ballots = ir.parseBallots(fileName);
    EXPECT_EQ(0, ballots);
    ir.writeToAuditFile("Multiple Elim Test.\n");
    int result = ir.runElection();
    EXPECT_EQ(0, result);
    EXPECT_EQ(ir.getWinner().getName(), " Marcus");
}

// Check an election with multiple files
TEST_F(IRTest, IRRunElectionMultipleFiles)
{
    ir.makeAuditFile("IR");
    int ch = chdir("../testing/");

    vector<string> fileNames;
    string fileName1 = "IR.csv";
    fileNames.push_back(fileName1);
    string fileName2 = "IR2.csv";
    fileNames.push_back(fileName2);

    int header = ir.parseHeader(fileNames[0]);
    EXPECT_EQ(0, header);

    int ballots;
    for (int i = 0; i < (int) fileNames.size(); i++) {
        ballots = ir.parseBallots(fileNames[i]);
        EXPECT_EQ(0, ballots);
    }
    ir.writeToAuditFile("Multiple Elim Test.\n");
    int result = ir.runElection();
    EXPECT_EQ(0, result);
    EXPECT_EQ(ir.getWinner().getName(), "Rosen ");
}

// Check an election with a tie 
// Runs 100 times to determine if the winner found is random
TEST_F(IRTest, IRRunElectionTie)
{
    // stores winners
    std::vector<Candidate> results;
    for (int i = 0; i < 100; i++) {
        // ir object
        IR ir1;
        // make audit file and store name
        string auditFileName = ir1.makeAuditFile("IR");
        string fileName = "IRTieTest.csv";
        ir1.writeToAuditFile("Tie Test.\n");
        // parse the header of fileName
        ir1.parseHeader(fileName);
        // parse the ballots of fileName
        ir1.parseBallots(fileName);
        // run the election
        ir1.runElection();
        // get the winner
        Candidate winner = ir1.getWinner();
        // add the winner to results vector
        results.push_back(winner);
        // convert auditFileName to const char* to remove it
        const char* auditFileNameChar = auditFileName.c_str();
        // remove audit file because we don't want 100 of them
        remove(auditFileNameChar);
    }
    // test that 'Rosen' wins between 45 and 55 times to show it's fair
    int c = 0;
    string name = "Rosen ";
    for (int i = 0; i < results.size(); i++) {
        if (name.compare(results[i].getName()) == 0) {
            c++;
        }
    }
    EXPECT_TRUE((c >= 45) && (c <= 55));
}

// Main to run the tests
// Needed in every test file because of the makefile's compilation method
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}