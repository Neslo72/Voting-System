/**
 * @file start.cc
 * @author Justin Lau
 * @brief Begins the election system & processes the ballot file
 */

#include "start.h"
#include "closedpartylist.h"
#include "election.h"
#include "ir.h"
#include "popularonly.h"
#include <fstream>
#include <vector>


/// @brief Vector of openable input files
vector<string> inputFiles;

/// @brief The election's type
string electionType;

/**
 * @brief Reads ballot files and stores the contents 
 * @param fileName name of the ballot file
 */
void readBallotFile(string fileName) {
    // attempt to open file
    string testing_dir = "../testing/" + fileName;
    ifstream input;
    input.open(testing_dir);
    // if successful
    if (!input.is_open()) {
        // error opening file
        cout << "Couldn't open " << fileName << endl;
        return;
    }
    cout << "Ballot file " << fileName << " validated!" << endl;
    inputFiles.push_back(testing_dir);
    input.close();
    return;
}

/**
 * @brief Main function to begin the voting system
 * which handles the ballot file & determines the election type
 * Used https://stackoverflow.com/questions/15138785/how-to-read-a-file-into-vector-in-c
 * as reference for file handling
 *
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return int: 0 on success and 1 on failure
 */
int main(int argc, char *argv[])
{
    // ballot files are inputed as arguments
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            string fileName = argv[i];
            readBallotFile(fileName);
        }
    }
    // prompt for ballot files
    else {
        cout << "Please enter a valid ballot file.  Ensure to include the full path to the file." << endl;
        cout << "If there are no more files to enter, please type 'done'." << endl;
        while (true) {
            string fileName;
            cin >> fileName;
            // check if done was entered
            if (fileName == "done") {
                break;
            }
            // check if name is blank
            if (fileName == "") {
                cout << "Please enter a valid ballot file." << endl;
            }
            readBallotFile(fileName);
        }
    }

    // // change directories 
    // int ch = chdir("../");
    // if (ch != 0) {
    //     cout << "Error changing directories" << endl;
    // }

    if(inputFiles.size() < 1) {
        cout << "No valid files input!  Please try again" << endl;
        return 1;
    }

    // Open the first input ballotfile to get the election type
    ifstream firstBallot(inputFiles[0]);
    getline(firstBallot, electionType);
    firstBallot.close();

    // Election dependent logic
    // Run IR election
    if(electionType == "IR")
    {
        // init Election vars
        IR *ir = new IR();
        string aFileNameIR = ir->makeAuditFile("IR");

        // Parse first header, the following files are assumed to have the same header
        if(ir->parseHeader(inputFiles[0]) != 0) {
            return 1;
        }
        for(int i = 0; i < (int) inputFiles.size(); i++)  // Loop over ALL ballot files
        {
            if(ir->parseBallots(inputFiles[i]) != 0) {  // Parse and note invalid files
                cout << "Mismatching ballot file information " << inputFiles[i] << ".  Ballot data unused" << endl;
            }
        }
        ir->runElection();  // Run election with all info gathered
        return 0;
    }

    // Run CPL election
    else if(electionType == "CPL")
    {
        // Decide whether or not to follow the IR format for mutliple files....
        // TODO: NEEDS MULTIPLE FILE IMPLEMENTATION

        // init Election vars
        CPL *cpl = new CPL();
        string aFileNameCPL = cpl->makeAuditFile(electionType);

        int reading = cpl->readBallots(inputFiles[0]);
        if (reading == 0) {
            cpl->runElection();
        }
        return 0;
    }

    // Run Popular vote election
    else if(electionType == "PO") 
    {
        // init Election vars
        PO *po = new PO();
        string aFileNamePO = po->makeAuditFile("PO");

        // Parse first header, the following files are assumed to have the same header
        if(po->parseHeader(inputFiles[0]) != 0) {
            return 1;
        }
        for(int i = 0; i < (int) inputFiles.size(); i++)  // Loop over ALL ballot files
        {
            if(po->parseBallots(inputFiles[i]) != 0) {  // Parse and note invalid files
                cout << "Mismatching ballot file information " << inputFiles[i] << ".  Ballot data unused" << endl;
            }
        }
        po->runElection();  // Run election with all info gathered
        return 0;
    }

    cout << "Error determining election type.  Please confirm the formatting of your ballot file" << endl;
    return 1;

}