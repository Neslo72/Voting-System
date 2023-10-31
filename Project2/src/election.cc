/**
 * @file election.cc
 * @author Justin Lau
 * @brief Handles parsing the ballot file to determine the number of ballots,
 * creates the audit file, writes to the audit file, and prints the results of
 * the election
 */

#include <election.h>

/**
 * Find today's date to name the audit file \n
 * Information gathered from:
 * https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
 */
time_t now = time(0);
tm *ltm = localtime(&now);

/**
 * @brief Creates the audit file
 *
 * @param ballotFileName name of the provided ballot file
 * @return string: audit file path
 */
string Election::makeAuditFile(string electionType)
{
    // variable to keep track of the file number if an audit file with that name already exists
    int fileNumber = 0;
    string baseAuditFileName;
    string extension = ".txt";

    // IR election type
    if (electionType == "IR")
    {
        // name audit file
        baseAuditFileName = "IR_" + to_string(ltm->tm_mon + 1) + to_string(ltm->tm_mday) + to_string(ltm->tm_year + 1900);
    } else if (electionType == "CPL") {
        // CPL election type
        baseAuditFileName = "CPL_" + to_string(ltm->tm_mon + 1) + to_string(ltm->tm_mday) + to_string(ltm->tm_year + 1900);
    } else if (electionType == "PO") {
        // PO election type
        baseAuditFileName = "PO_" + to_string(ltm->tm_mon + 1) + to_string(ltm->tm_mday) + to_string(ltm->tm_year + 1900);
    } 
    else {
        cout << "Failed to identify the type of election. Please try again." << endl;
        exit(-1);
    }

    // check if the audit file already exists
    while (true) {
        if (fileNumber > 0) {
            auditFileName = baseAuditFileName + "_" + to_string(fileNumber) + extension;
        } else {
            auditFileName = baseAuditFileName + extension;
        }

        // create audit file
        ifstream file;
        auditFileName = "../misc/AuditFiles/" + auditFileName;
        file.open(auditFileName);
        if (!file.is_open()) {
            // file doesn't exist
            auditFile.open(auditFileName);
            break;
        }

        // file exists, so increment fileNumber
        file.close();
        fileNumber++;
    }

    // return the audit file name
    //ch = chdir("../..");
    return auditFileName;
}

/**
 * @brief Writes to the audit file
 *
 * @param text text being written to the audit file
 */
void Election::writeToAuditFile(string text)
{
    if(!auditFile.is_open()) {
        cout << "Error writing to audit file" << endl;
    }
    auditFile << text << endl;
}

/// @brief Prints the results of the election
/// @param text text to be printed
void Election::printResults(string text)
{
    cout << text << endl;
}
