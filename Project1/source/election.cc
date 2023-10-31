/**
 * @file election.cc
 * @author Justin Lau
 * @brief Handles parsing the ballot file to determine the number of ballots,
 * creates the audit file, writes to the audit file, and prints the results of 
 * the election
 */

#include <election.h>
#include <ir.h>
#include <closedpartylist.h>

/**
 * Find today's date to name the audit file \n
 * Information gathered from:
 * https://stackoverflow.com/questions/64782/how-do-you-append-an-int-to-a-string-in-c
*/
time_t now = time(0);
tm* ltm = localtime(&now);

/**
 * Audit file
 */
ofstream auditFile;

/**
 * auditFile Name
 */
string auditFileName;

/**
 * @brief Constructor for Election object
 * @param electionType Type of election
 */
Election::Election(fstream& _ballotFile, string _electionType) {
    electionType = _electionType;
}

/**
 * @brief Counts the number of ballots in a provided ballot file
 * 
 * @param ballotFileName name of the provided ballot file
 * @return int: number of ballots
 */

/**
 * @brief Creates the audit file
 * 
 * @param ballotFileName name of the provided ballot file
 * @return string: audit file path
 */
string Election::makeAuditFile(string electionType) {
    // determine if the AuditFiles folder exists in the misc folder
    int ch = chdir("misc/");
    if (ch == 0) {
        if (mkdir("AuditFiles", 0777) == -1 || mkdir("AuditFiles", 0777) == 0) {
            // IR election type
            if (electionType == "IR") {
                // name audit file
                auditFileName = "IR_" + to_string(ltm->tm_mon + 1) + to_string(ltm->tm_mday) + to_string(ltm->tm_year + 1900);
                // change directories
                int ch = chdir("AuditFiles/");
                if (ch == 0) {
                    ofstream auditFile(auditFileName);
                }
                else {
                    cout << "Audit file creation error." << endl;
                    cout << "Please try again." << endl;
                    exit(-1);
                }
            }
            // CPL election type
            else if (electionType == "CPL") {
                // name audit file
                auditFileName = "CPL_" + to_string(ltm->tm_mon + 1) + to_string(ltm->tm_mday) + to_string(ltm->tm_year + 1900);
                // change directories
                int ch = chdir("AuditFiles/");
                if (ch == 0) {
                    ofstream auditFile(auditFileName);
                }
                else { 
                    cout << "Audit file creation error." << endl;
                    cout << "Please try again." << endl;
                    exit(-1);
                }
            }
            else {
                cout << "Failed to identify the type of election.  Please try again." << endl;
                exit(-1);
            }
        }
    }
    // return the path to the current audit file
    return auditFileName;
}

/**
 * @brief Writes to the audit file
 * 
 * @param text text being written to the audit file
 */
void Election::writeToAuditFile(string text) {
    cout << text << endl;
    auditFile.open(auditFileName, std::ios::app);
    auditFile << text << endl;
    auditFile.close();
}

// @brief Prints the results of the election
void Election::printResults() {
    if (electionType == "IR") {
        cout << "IR Election Information" << endl;
        // additional IR election info:

    }
    else if (electionType == "CPL") {
        cout << "CPL Election Information" << endl;
        // additional CPL election info:
        
    }
    else {
        cout << "Election type was not determined." << endl;
        cout << "Please ensure you're in the 'Project1' directory and try again." << endl;
    }
}

//int Election::runElection(string ballotFileName) {}