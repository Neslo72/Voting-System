/**
 * @file start.cc
 * @author Justin Lau
 * @brief Begins the election system & processes the ballot file
 */

#include <start.h>
#include <ir.h>
#include <closedpartylist.h>
#include <election.h>

/**
 * @brief provided ballot file
 */ 
fstream ballotFile;
/**
 * @brief name of ballot file
 */
string ballotFileName;

/**
 * @brief the type of election
 */
string electionType;

Election* election = nullptr;

/**
 * @brief Main function to begin the voting system 
 * which handles the ballot file & determines the election type
 * 
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return int: 0 on success and 1 on failure
 */
int main(int argc, char* argv[]) {
    // if ballot file is inputted as a command line argument
    if (argc > 1) {
        ballotFileName = argv[1];
    }
    // ballot file needs to be inputted as it wasn't provided as an argument
    else {
        cout << "Please enter a valid ballot file." << endl;
        cin >> ballotFileName;
    }
    // Attempt to open the ballot file with the provided ballot file name
    ballotFile.open(ballotFileName.c_str(), std::ios::in);

    // Test if the ballot file failed to open
    // If it does fail, the user is prompted to re-enter the ballot file
    if (ballotFile.is_open()) {    
        // Get the first line of the ballot file to get the election type
        getline(ballotFile, electionType);

        // If IR election type, create a new IR object to run the election, 
        // print the results, and write election information to the audit file
        if (electionType == "IR") {
            election = new Election(ballotFile, electionType);
            election->makeAuditFile(electionType);
            IR* ir = new IR();
            ir->readBallots(ballotFileName);
            ir->runElection();
        }
        else if (electionType == "CPL") {
            election = new Election(ballotFile, electionType);
            election->makeAuditFile(electionType);
            CPL* cpl = new CPL();
            cpl->readBallots(ballotFileName);
            cpl->runElection();
        }
    }
    ballotFile.close();
    // return 0 on success
    return 0;
}
