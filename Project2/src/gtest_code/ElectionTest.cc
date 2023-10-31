#include "../include/election.h"
#include "gtest/gtest.h"

class ElectionTest : public ::testing::Test
{
  protected:
    Election election;
};

// check the creation of the audit file
TEST_F(ElectionTest, ElectionMakeAuditFileIRTest)
{
    int ch = chdir("../");
    if (ch != 0)
    {
        cout << "Error" << endl;
    }
    string name = election.makeAuditFile("IR");
    // check that the audit file name begins with "IR_"
    EXPECT_EQ(name.substr(0, 3), "IR_");
    // check that the size of the audit file is 10
    // 10 characters for: IR_MDDYYYY
    EXPECT_EQ(name.size(), 10);
}
/*
// check the creation of the audit file
TEST_F(ElectionTest, ElectionMakeAuditFileCPLTest)
{
    int ch = chdir("../..");
    if (ch != 0)
    {
        cout << "Error" << endl;
    }
    string name = election.makeAuditFile("CPL");
    // check that the audit file name begins with "CPL"
    EXPECT_EQ(name.substr(0, 4), "CPL_");
    // check that the size of the audit file is 11
    // 11 characters for: CPL_MDDYYYY
    EXPECT_EQ(name.size(), 11);
}

// check writing to the audit file
TEST_F(ElectionTest, ElectionWritetoAuditFileIRTest)
{
    int ch = chdir("../..");
    if (ch != 0)
    {
        cout << "Error" << endl;
    }
    string name = election.makeAuditFile("IR");
    ch = chdir("../..");
    election.writeToAuditFile("Testing, testing, testing", name);
    // open audit file and check
    ch = chdir("misc/AuditFiles/");
    ifstream auditFile(name);
    string line;
    getline(auditFile, line);
    EXPECT_EQ(line, "Testing, testing, testing");
    auditFile.close();
    ch = chdir("../");
}

// check writing to the audit file
TEST_F(ElectionTest, ElectionWritetoAuditFileCPLTest)
{
    int ch = chdir("../");
    if (ch != 0)
    {
        cout << "Error" << endl;
    }
    string name = election.makeAuditFile("CPL");
    ch = chdir("../..");
    election.writeToAuditFile("Testing, testing, testing", name);
    // open audit file and check
    ch = chdir("misc/AuditFiles/");
    ifstream auditFile(name);
    string line;
    getline(auditFile, line);
    EXPECT_EQ(line, "Testing, testing, testing");
    auditFile.close();
    ch = chdir("../");
}
*/

// Main to run the tests
// Needed in every test file because of the makefile's compilation method
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}