#include "../include/popularonly.h"
#include "../include/ballot.h"
#include "../include/candidate.h"
#include "../include/election.h"
#include "gtest/gtest.h"
#include <cstdlib>
#include <vector>

class POTest : public ::testing::Test
{
  protected:
    // po election
    PO po;
};

// Checks that a header can be parsed
TEST_F(POTest, ParseHeaderTest) {
    po.makeAuditFile("PO");
    string fileName = "PO.csv";
    int ch = chdir("../testing/");
    po.writeToAuditFile("Parsing header test.\n");
    // parse header
    int result = po.parseHeader(fileName);
    // returns 0 on success
    EXPECT_EQ(0, result);
}

// Checks that ballots can be parsed
TEST_F(POTest, ParseBallotsTest) {
    po.makeAuditFile("PO");
    string fileName = "PO.csv";
    int ch = chdir("../testing/");
    po.writeToAuditFile("Parsing ballots test.\n");
    // parse header
    po.parseHeader(fileName);
    // parse ballots
    int result = po.parseBallots(fileName);
    // returns 0 on success
    EXPECT_EQ(0, result);
}

// Check that the election requires candidates
TEST_F(POTest, PORunElectionNoCandidates) {
    po.makeAuditFile("PO");
    string fileName = "PONoCans.csv";
    int ch = chdir("../testing/");
    po.writeToAuditFile("No candidates test.\n");
    // parse header
    int header = po.parseHeader(fileName);
    // returns 0 on success
    EXPECT_EQ(0, header);
    // parse ballots
    int ballots = po.parseBallots(fileName);
    // returns 1 on failure
    EXPECT_EQ(1, ballots);
    // run election
    int result = po.runElection();
    // returns -1 on failure
    EXPECT_EQ(-1, result);
}

// Check an election with one candidate and one ballot
TEST_F(POTest, PORunElectionOneCanOneBal) {
    po.makeAuditFile("PO");
    string fileName = "POOneCan.csv";
    int ch = chdir("../testing/");
    po.writeToAuditFile("One candidate with one ballot test.\n");
    // parse header
    int header = po.parseHeader(fileName);
    // returns 0 on success
    EXPECT_EQ(0, header);
    // parse ballots
    int ballots = po.parseBallots(fileName);
    // returns 0 on success
    EXPECT_EQ(0, ballots);
    // run election
    int result = po.runElection();
    // returns 0 on success
    EXPECT_EQ(0, result);
    // winner should be Pike
    EXPECT_EQ(po.getWinner().getName(), "Pike");
}

// Check an election with two candidates and two ballots
TEST_F(POTest, PORunElectionTwoCanTwoBal) {
    po.makeAuditFile("PO");
    string fileName = "POTwoCan.csv";
    int ch = chdir("../testing/");
    po.writeToAuditFile("Two candidate with two ballot test.\n");
    // parse header
    int header = po.parseHeader(fileName);
    // returns 0 on success
    EXPECT_EQ(0, header);
    // parse ballots
    int ballots = po.parseBallots(fileName);
    // returns 0 on success
    EXPECT_EQ(0, ballots);
    // run election
    int result = po.runElection();
    // returns 0 on success
    EXPECT_EQ(0, result);
    // winner should be Pike
    EXPECT_EQ(po.getWinner().getName(), "Pike");
}

// Check an election with three candidates and three ballots
TEST_F(POTest, PORunElectionThreeCanThreeBal) {
    po.makeAuditFile("PO");
    string fileName = "POThreeCan.csv";
    int ch = chdir("../testing/");
    po.writeToAuditFile("Two candidate with two ballot test.\n");
    // parse header
    int header = po.parseHeader(fileName);
    // returns 0 on success
    EXPECT_EQ(0, header);
    // parse ballots
    int ballots = po.parseBallots(fileName);
    // returns 0 on success
    EXPECT_EQ(0, ballots);
    // run election
    int result = po.runElection();
    // returns 0 on success
    EXPECT_EQ(0, result);
    // winner should be Foster
    EXPECT_EQ(po.getWinner().getName(), "Foster");
}

// Check an election with multiple files
TEST_F(POTest, PORunElectionMultipleFiles) {
    po.makeAuditFile("PO");
    int ch = chdir("../testing/");
    po.writeToAuditFile("Multiple Elim Test.\n");
    // vector to hold the fileNames
    vector<string> fileNames;
    string fileName1 = "PO.csv";
    // add file name to vector
    fileNames.push_back(fileName1);
    string fileName2 = "PO2.csv";
    // add file name to vector
    fileNames.push_back(fileName2);
    // parse header
    int header = po.parseHeader(fileNames[0]);
    // returns 0 on success
    EXPECT_EQ(0, header);
    // variable to hold the result of parseBallots
    int ballots;
    // loop through each file and parse the ballots
    for (int i = 0; i < (int) fileNames.size(); i++) {
        ballots = po.parseBallots(fileNames[i]);
        // returns 0 on success
        EXPECT_EQ(0, ballots);
    }
    // run election
    int result = po.runElection();
    // returns 0 on success
    EXPECT_EQ(0, result);
    // winner should be Pike
    EXPECT_EQ(po.getWinner().getName(), "Pike");
}

// Check an election with a tie 
// Runs 100 times to determine if the winner found is random
TEST_F(POTest, PORunElectionTie) {
    // stores winners
    std::vector<Candidate> results;
    for (int i = 0; i < 100; i++) {
        // po object
        PO po1;
        // make audit file and store name
        string auditFileName = po1.makeAuditFile("PO");
        string fileName = "POTieTest.csv";
        po1.writeToAuditFile("Tie Test.\n");
        // parse header of fileName
        po1.parseHeader(fileName);
        // parse ballots of fileName
        po1.parseBallots(fileName);
        // run the election
        po1.runElection();
        // get the winner
        Candidate winner = po1.getWinner();
        // add winner to results vector
        results.push_back(winner);
        // convert auditFileName to const char* to remove it
        const char* auditFileNameChar = auditFileName.c_str();
        // remove audit file because we don't want 100 of them
        remove(auditFileNameChar);
    }
    int c = 0;
    string name = "Pike";
    // loop through the results vector to find the amount of times Pike appears
    for (int i = 0; i < results.size(); i++) {
        if (name.compare(results[i].getName()) == 0) {
            c++;
        }
    }
    // Pike should win about half of the time
    EXPECT_TRUE((c >= 45) && (c <= 55));
}

// Main to run the tests
// Needed in every test file because of the makefile's compilation method
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}