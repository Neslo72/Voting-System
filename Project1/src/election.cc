/**
 * @file election.cc
 * @author Justin Lau
 * @brief Handles parsing the ballot file to determine the number of ballots,
 * creates the audit file, writes to the audit file, and prints the results of
 * the election
 */

#include <closedpartylist.h>
#include <election.h>
#include <ir.h>

/**
 * Find today's date to name the audit file \n
 * Information gathered from:
 * https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
 */
time_t now = time(0);
tm *ltm = localtime(&now);

/**
 * @brief Constructor for Election object
 * @param electionType Type of election
 */
Election::Election(fstream &_ballotFile, string _electionType)
{
    electionType = _electionType;
}

/**
 * @brief Creates the audit file
 *
 * @param ballotFileName name of the provided ballot file
 * @return string: audit file path
 */
string Election::makeAuditFile(string electionType)
{
    // determine if the AuditFiles folder exists in the misc folder
    int ch = chdir("misc/AuditFiles/");
    if (ch != 0)
    {
        cout << "Audit file creation error" << endl;
        exit(-1);
    }
    // IR election type
    if (electionType == "IR")
    {
        // name audit file
        auditFileName = "IR_" + to_string(ltm->tm_mon + 1) + to_string(ltm->tm_mday) + to_string(ltm->tm_year + 1900);
        // create audit file
        ofstream auditFile(auditFileName);
    }
    else if (electionType == "CPL")
    {
        // CPL election type
        auditFileName = "CPL_" + to_string(ltm->tm_mon + 1) + to_string(ltm->tm_mday) + to_string(ltm->tm_year + 1900);
        // create audit file
        ofstream auditFile(auditFileName);
    }
    else
    {
        cout << "Failed to identify the type of election. Please try again." << endl;
        exit(-1);
    }
    // return the audit file name
    return auditFileName;
}

/**
 * @brief Writes to the audit file
 *
 * @param text text being written to the audit file
 */
void Election::writeToAuditFile(string text, string auditFileName)
{
    int ch = chdir("misc/AuditFiles/");
    if (ch != 0)
    {
        cout << "Error writing to the audit file." << endl;
    }
    auditFile.open(auditFileName, std::ios::app);
    auditFile << text << endl;
    auditFile.close();
    ch = chdir("../..");
}

/// @brief Prints the results of the election
/// @param text text to be printed
void Election::printResults(string text)
{
    cout << text << endl;
}
