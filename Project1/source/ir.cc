/**
 * @file ir.cc
 * @author Marcus Rana (rana0066) and Liam Mcguigan (mcgui479)
 * @brief IR implementation
 */
#include "ir.h"
#include "candidate.h"

using namespace std;

string electionTypeIR;
string numCandidates;
string candidatesStr;
string numBallots;

/// @brief runElection: Runs the election 
int IR::runElection() {
    writeToAuditFile("Running the election.\n");
    // ensure all candidates are not eliminated at the start
    for (int i = 0; i<(int)candidates.size(); i++) {
        candidates[i].eliminated = false;
    }

    while(1){
        int total_count = 0;
        int max_index = 0;
        int max_val = 0;

        // Find whoever has the most votes and get total num of votes
        for (int i = 0;i<(int)candidates.size();i++){ 
            if(candidates[i].eliminated == false){
                int num_votes = candidates[i].getNumVotes();
                if(num_votes > max_val){
                    max_val = candidates[i].getNumVotes();
                    max_index = i;
                }
                total_count += num_votes;
            }
        }

        // Check for the winner
        if((double)candidates[max_index].getNumVotes() > ((double)total_count/2)){ //Determine if redistribution is necessary
            winner = candidates[max_index];

            //printResults("Candidate " + winner.getName() + "has won the election!" + endl);
            writeToAuditFile("Candidate " + winner.getName() + "has won the election!\n");
            return 0;
        }

        // Redistribution is necessary, find the candidate(s) with the least votes
        int min = 2000000000;  // If there is an election with over 2 billion votes this will fail...
        std::vector<Candidate*> poss_elim_cand;
        for(int i = 0; i < (int) candidates.size(); i++) {
            if(candidates[i].eliminated == false) {
                int temp = candidates[i].getNumVotes();
                if(temp < min) {
                    poss_elim_cand.clear();
                    poss_elim_cand.push_back(&candidates[i]);
                    min = temp;
                }
                else if(temp == min) {
                    poss_elim_cand.push_back(&candidates[i]);
                }
            }
        }

        // Redistribute the votes from the current lowest total candidate
        Candidate *to_elim;
        if(poss_elim_cand.size() == 1) {  // One candidate to eliminate
            to_elim = poss_elim_cand[0];
            redistributeVotes(to_elim);
        }
        else if(poss_elim_cand.size() > 1) {  // Multiple candidates to eliminate
            to_elim = breakTie(poss_elim_cand);
            redistributeVotes(to_elim);
        }
        else {
            //cout << "error: poss_elim has no items" << endl;
            return -1;
        }
        
        // GONNA PRINT THEM FOR NOW...
        writeToAuditFile("RUNOFF ELIMINATION RESULTS: \n");
        //cout << "RUNOFF ELIMINATION RESULTS: " << endl;
        writeToAuditFile("Candidate " + to_elim->getName() + "has been eliminated \n");
        //cout << "Candidate " << to_elim->getName() << "has been eliminated " << endl;
        writeToAuditFile("The remaining candidates after redistribution are:");
        //cout << "The remaining candidates are:"; 
        for(int i = 0; i < (int) candidates.size(); i++) {
            if(candidates[i].eliminated == false) {
                //cout << " " << candidates[i].getName();
                writeToAuditFile(" " + candidates[i].getName() + " (" + to_string(candidates[i].getNumVotes()) + " votes)");
            }
        }
        //cout << endl;
    }
    return 0;
}

/// @brief breakTie: Breaks a tie of multiple candidates
/// @param tieVector vector of candidates in a tie
/// @return selected candidate
Candidate* IR::breakTie(std::vector<Candidate*> tieVector) {
    int num_test = 100000 + rand() % 500000;
    int rand_value = 0;
    for(int i = 0;i<num_test;i++){
        rand_value = rand() % tieVector.size();
    }
    //cout << "DEBUG TIE VALUE " << rand_value << endl;
    writeToAuditFile("DEBUG TIE VALUE " + to_string(rand_value) + " ");
    return tieVector[rand_value];
}

/// @brief readBallots: Reads all ballots
/// @param ballotFileName name of ballotFile to read
int IR::readBallots(string ballotFileName) {
    // change director to 'Project1' folder
    int ch = chdir("../..");
    writeToAuditFile("IR Election\n");
    writeToAuditFile("Reading ballots...\n");
    // ballot file
    ifstream ballotFileIR(ballotFileName);
    if (ch == 0) {
        if (!ballotFileIR) {
            //cout << "Could not open the ballot file.  Please try again." << endl;
            return -1;
        }
        // read header info
        // first line has election type
        getline(ballotFileIR, electionTypeIR);
        // second line has number of candidates
        getline(ballotFileIR, numCandidates);
        // third line has the candidates
        getline(ballotFileIR, candidatesStr);
        // fourth line has the number of ballots
        getline(ballotFileIR, numBallots);
    }
    else {
        //cout << "Unable to locate the ballot file.  Please try again" << endl;
        return -1;
    }
    // used to separate the candidates by their names/parties
    stringstream ss(candidatesStr);
    string field;
    string name;
    string party;
    // put candidates into vector from ballot file
    while (getline(ss, field, ',')) {
        stringstream ss_field(field);
        getline(ss_field, name, '(');
        getline(ss_field, party, ' ');
        Candidate candidate(name, party);
        candidates.push_back(candidate);
    }

    // TODO: Read ballots & assign to candidates:
    int numberBallots = stoi(numBallots);
    string line;
    if (!ballotFileIR) {
        //cout << "Error opening the ballot file. Please try again." << endl;
        return -1;
    }
    //cout << "Num ballots: " << numberBallots << endl;
    for (int i = 0; i < numberBallots; i++) {
        if (getline(ballotFileIR, line)) {
             
            // Parse and store 1 valid ballot from the csv file
            istringstream ss(line);
            int read_candidates = 0;
            int total_candidates = (int) candidates.size();
            int vote_pref[total_candidates];

            // Read in the candidate ordering to an int array
            int i = 0;
            while(i < total_candidates) {
                char curr_char;
                ss >> curr_char;
                if(curr_char == ',' || ss.eof()) {
                    i++;
                }
                else if(curr_char > '0' && curr_char <= '9') {
                    vote_pref[i] = curr_char - '0';
                    read_candidates++;
                }
            }

            // Add candidates to Ballot in order of choosing
            // do NOT add the first order candidate, deal with that next
            Ballot ballot;
            for(int i = 1; i <= read_candidates; i++) {
                for(int j = 0; j < total_candidates; j++) {
                    if(i != 1 && vote_pref[j] == i) {
                        ballot.addChoice(j);
                        break;
                    }
                }
            }

            // Store the ballot with the first order candidate
            for(int i = 0; i < total_candidates; i++) {
                if(vote_pref[i] == 1) {
                    candidates[i].addBallot(ballot);
                    break;
                }
            }
        }
    }

    // print for testing
    //for (int i = 0; i < (int)candidates.size(); i++) {
        //cout << "Candidate: " << candidates[i].getName() << " (" << candidates[i].getParty() << endl;
        //cout << "Number of original votes: " << candidates[i].getNumVotes() << endl;
    //}

    // close file and return 
    writeToAuditFile("Done w/ ballot reading.\n");
    ballotFileIR.close();
    return 0;
}

/// @brief Gets index of a given candidate
/// @param name Name of candidate to get index of
int IR::getCandidateIndex(string name){
    for(int i = 0; i < (int)candidates.size(); i++){
        if(name.compare(candidates[i].getName()) == 0){
            return i;
        }
    }
    return -1;
}

/// @brief redistributeVotes: Redistributes votes in an IR election
/// @param candidate Candidate who has been eliminated
void IR::redistributeVotes(Candidate* candidate) {
    candidate->eliminated = true;

    // Loop through the candidate's ballots
    for(int i = 0; i < candidate->getNumVotes(); i++){
        Ballot curr_ballot;
        if(!candidate->removeBallot(&curr_ballot)) {
            int to_add = curr_ballot.getChoice();
            if(to_add != -1) {
                candidates[to_add].addBallot(curr_ballot);  // Add to next choice
            }
        }
    }
}
