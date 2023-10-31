/**
 * @file popularonly.cc
 * @author Marcus Rana (rana0066)
 * @brief PO implementation
 */
#include "popularonly.h"
#include "candidate.h"

using namespace std;

/// @brief runElection: Runs the election
/// @return returns 0 to indicate success
int PO::runElection()
{
    string temp = "Running the PO election.\n";
    writeOutput(temp);
    // check that there are candidates
    if (candidates.size() == 0)
    {
        printResults();
        return -1;
    }

    int max = 0;
    std::vector<Candidate *> tieVector;
    // loop to check the number of votes for each candidate
    for (int i = 0; i < (int)candidates.size(); i++)
    {
        if (candidates[i]->getNumVotes() > max)
        {
            max = candidates[i]->getNumVotes();
        }
    }

    // loop to check if there is a tie
    for (int i = 0; i < (int)candidates.size(); i++)
    {
        if (candidates[i]->getNumVotes() == max)
        {
            tieVector.push_back(candidates[i]);
        }
    }

    // set the winner
    setWinner(breakTie(tieVector));

    // print the number of votes each candidate got
    for (int i = 0; i < (int)candidates.size(); i++) {
        // string stream to print percentage
        ostringstream ss;
        // divide each candidate's number of votes by the total
        double percentage = (static_cast<double>(candidates[i]->getNumVotes()) / numBallots) * 100;
        // round to 2 decimal places
        double rounded = round(percentage * 100) / 100.0;
        // set variables to strings for output
        string numVotes = to_string(candidates[i]->getNumVotes());
        string numBals = to_string(numBallots);
        ss << fixed << setprecision(2) << rounded;
        string percent = ss.str();
        if (numBallots == 1) {
            temp = "Candidate " + candidates[i]->getName() + " received " + numVotes + " out of " + numBals + " vote (" + percent + "%)\n";
        }
        else {
            temp = "Candidate " + candidates[i]->getName() + " received " + numVotes + " out of " + numBals + " votes (" + percent + "%)\n";
        }
        writeOutput(temp);
    }

    // output results
    temp = "\nCandidate " + winner->getName() + " has won the election!\n";
    writeOutput(temp);
    printResults();
    return 0;
}

/// @brief breakTie: Breaks a tie of multiple candidates
/// @param tieVector vector of candidates in a tie
/// @return winning candidate
Candidate *PO::breakTie(std::vector<Candidate *> tieVector)
{
    int num_test = 100000 + rand() % 500000;
    int rand_value = 0;
    for (int i = 0; i < num_test; i++)
    {
        rand_value = rand() % tieVector.size();
    }
    return tieVector[rand_value];
}

/// @brief parseHeader: parses the header of a ballot file
/// @param balotFileName name of the ballotFile
/// @return returns 1 on error, 0 on success.  Isues written to the audit file
int PO::parseHeader(string ballotFileName) {
    // ballot file
    ifstream ballotFilePO(ballotFileName);
    // check that ballot file can be opened
    if (!ballotFilePO)
    {
        string err = "Error: " + ballotFileName + " could not be opened\n";
        writeOutput(err);
        return 1;
    }
    // read header info
    string doesNotMatter;
    string candidatesStr;

    // Skip first two lines
    getline(ballotFilePO, doesNotMatter);
    getline(ballotFilePO, doesNotMatter);
    // third line has the candidates
    getline(ballotFilePO, candidatesStr);

    // used to separate the candidates by their names/parties
    stringstream ss(candidatesStr);
    string field;
    string name;
    string party;
    string skip;

    // put candidates into vector from ballot file
    // ASSUMES all input PO ballots have the same header information...
    if (candidates.size() == 0) {
        size_t pos = 0;
        while (pos < candidatesStr.length()) {
            size_t start_pos = candidatesStr.find("[", pos);
            if (start_pos == string::npos) {
                break;
            }
            size_t end_pos = candidatesStr.find("]", start_pos);
            if (end_pos == string::npos) {
                break;
            }
            string cand_str = candidatesStr.substr(start_pos + 1, end_pos - start_pos - 1);
            size_t comma_pos = cand_str.find(",");
            name = cand_str.substr(0, comma_pos);
            skip = cand_str.substr(comma_pos, 2);
            party = cand_str.substr(comma_pos + 2, 3);
            candidates.push_back(new Candidate(name, party));
            pos = end_pos + 1;
        }
    }
    return 0;
}

/// @brief parseBallots: Parses the ballots contained in the ballotFile
/// @param ballotFileName name of a ballot file
/// @return returns 1 on failure, 0 on success
int PO::parseBallots(string ballotFileName) {
    // ballot file
    ifstream ballotFilePO(ballotFileName);
    // check that ballot file can be opened
    if (!ballotFilePO) {
        string err = "Error: " + ballotFileName + " could not be opened\n";
        writeOutput(err);
        return 1;
    }

    // setup some vars for header matching
    string temp;
    string electionType;
    int numCandidates;

    // Confirm same election type
    getline(ballotFilePO, electionType);
    if(electionType != "PO") {
        string err = "Error: mismatching election types in " + ballotFileName + "\n";
        writeOutput(err);
        return 1;
    }
    
    // Confirm same number of candidates
    getline(ballotFilePO, temp);
    if (!ballotFilePO.fail()) {
        numCandidates = stoi(temp);
        if((int) candidates.size() != numCandidates) {
            string err = "Error: mismatching number of candidates in " + ballotFileName + "\n";
            writeOutput(err);
            return 1;
        }
    }
    else {
        string err = "Error reading candidates in " + ballotFileName + "\n";
        writeOutput(err);
        return 1;
    }

    // Skip candidates (line 3), we are assuming they are the same...
    getline(ballotFilePO, temp);
    // Get the number of ballots
    getline(ballotFilePO, temp);
    if (!ballotFilePO.fail()) {
        numBallots += stoi(temp);
    }
    else {
        string err = "Error reading ballots in " + ballotFileName + "\n";
        writeOutput(err);
        return 1;
    }

    string line;
    for (int i = 0; i < numBallots; i++)
    {
        if (getline(ballotFilePO, line))
        {
            int choice = line.find('1');
            Ballot ballot;
            ballot.addChoice(choice);
            // Increments the party based on choice
            candidates[choice]->addBallot(ballot);
        }
    }
    // close file and return
    ballotFilePO.close();
    return 0;
}

/// @brief writeOutput: helper to print to audit file and terminal
/// @param str string to output
void PO::writeOutput(string str) 
{
    writeToAuditFile(str);
    text += str;
}