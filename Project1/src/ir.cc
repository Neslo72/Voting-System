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
/// @return returns 0 to indicate success
int IR::runElection(vector<Candidate *> &cansFromVector, string auditFileName)
{
    candidates = cansFromVector;
    text += "Running the election.\n";
    election.writeToAuditFile("Running the election.\n", auditFileName);
    if (candidates.size() == 0)
    {
        election.printResults(text);
        return -1;
    }
    // ensure all candidates are not eliminated at the start
    for (int i = 0; i < (int)candidates.size(); i++)
    {
        candidates[i]->eliminated = false;
    }
    while (1)
    {
        int total_count = 0;
        int max_index = 0;
        int max_val = 0;
        // Find whoever has the most votes and get total num of votes
        for (int i = 0; i < (int)candidates.size(); i++)
        {
            if (candidates[i]->eliminated == false)
            {
                int num_votes = candidates[i]->getNumVotes();
                if (num_votes > max_val)
                {
                    max_val = candidates[i]->getNumVotes();
                    max_index = i;
                }
                total_count += num_votes;
            }
        }
        // Check for the winner
        if ((double)candidates[max_index]->getNumVotes() > ((double)total_count / 2))
        { // Determine if redistribution is necessary
            winner = candidates[max_index];
            setWinner(winner);
            text += "Candidate " + winner->getName() + " has won the election!\n";
            election.writeToAuditFile("Candidate " + winner->getName() + " has won the election!\n", auditFileName);
            election.printResults(text);
            return 0;
        }

        // Redistribution is necessary, find the candidate(s) with the least votes
        int min = 2000000000; // If there is an election with over 2 billion votes this will fail...
        std::vector<Candidate *> poss_elim_cand;
        for (int i = 0; i < (int)candidates.size(); i++)
        {
            if (candidates[i]->eliminated == false)
            {
                int temp = candidates[i]->getNumVotes();
                if (temp < min)
                {
                    poss_elim_cand.clear();
                    poss_elim_cand.push_back(candidates[i]);
                    min = temp;
                }
                else if (temp == min)
                {
                    poss_elim_cand.push_back(candidates[i]);
                }
            }
        }

        // Redistribute the votes from the current lowest total candidate
        Candidate *to_elim;
        if (poss_elim_cand.size() == 1)
        { // One candidate to eliminate
            to_elim = poss_elim_cand[0];
            redistributeVotes(to_elim);
        }
        else if (poss_elim_cand.size() > 1)
        { // Multiple candidates to eliminate
            to_elim = breakTie(poss_elim_cand);
            redistributeVotes(to_elim);
        }
        else
        {
            text += "Error: possible elimination candidate has no items.\n";
            election.writeToAuditFile("Error: possible elimination candidate has no items.\n", auditFileName);
            election.printResults(text);
            return -1;
        }
        // print results and write to audit file
        election.writeToAuditFile("RUNOFF ELIMINATION RESULTS: \n", auditFileName);
        text += "RUNOFF ELIMINATION RESULTS: \n";
        election.writeToAuditFile("Candidate " + to_elim->getName() + " has been eliminated \n", auditFileName);
        text += "Candidate " + to_elim->getName() + " has been eliminated.\n";
        election.writeToAuditFile("The remaining candidates after redistribution are:", auditFileName);
        text += "The remaining candidates are: \n";
        for (int i = 0; i < (int)candidates.size(); i++)
        {
            if (candidates[i]->eliminated == false)
            {
                election.writeToAuditFile(" " + candidates[i]->getName() + " (" +
                                              to_string(candidates[i]->getNumVotes()) + " votes)",
                                          auditFileName);
                text += " " + candidates[i]->getName() + " (" + to_string(candidates[i]->getNumVotes()) + " votes)\n";
            }
        }
    }
    election.printResults(text);
    return 0;
}

/// @brief breakTie: Breaks a tie of multiple candidates
/// @param tieVector vector of candidates in a tie
/// @return selected candidate
Candidate *IR::breakTie(std::vector<Candidate *> tieVector)
{
    int num_test = 100000 + rand() % 500000;
    int rand_value = 0;
    for (int i = 0; i < num_test; i++)
    {
        rand_value = rand() % tieVector.size();
    }
    return tieVector[rand_value];
}

/// @brief readBallots: Reads all ballots
/// @param ballotFileName name of ballotFile to read
vector<Candidate *> IR::readBallots(string ballotFileName, string auditFileName)
{
    // change back to Project1 folder
    int ch = chdir("../../");
    if (ch != 0)
    {
        election.writeToAuditFile("Error finding the ballot file. Please try again.\n", auditFileName);
        text += "Error finding the ballot file.  Please try again.\n";
        election.printResults(text);
        return {};
    }
    // ballot file
    ifstream ballotFileIR(ballotFileName);
    if (!ballotFileIR)
    {
        election.writeToAuditFile("Could not open the ballot file.  Please try again.\n", auditFileName);
        text += "Could not open the ballot file.  Please try again.\n";
        election.printResults(text);
        return {};
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

    // used to separate the candidates by their names/parties
    stringstream ss(candidatesStr);
    string field;
    string name;
    string party;
    // put candidates into vector from ballot file
    while (getline(ss, field, ','))
    {
        stringstream ss_field(field);
        getline(ss_field, name, '(');
        getline(ss_field, party, ' ');
        candidates.push_back(new Candidate(name, party));
    }

    int numberBallots = stoi(numBallots);
    string line;
    for (int i = 0; i < numberBallots; i++)
    {
        if (getline(ballotFileIR, line))
        {
            // Parse and store 1 valid ballot from the csv file
            istringstream ss(line);
            int read_candidates = 0;
            int total_candidates = (int)candidates.size();
            int vote_pref[total_candidates];

            // Read in the candidate ordering to an int array
            int i = 0;
            while (i < total_candidates)
            {
                char curr_char;
                ss >> curr_char;
                if (curr_char == ',' || ss.eof())
                {
                    i++;
                }
                else if (curr_char > '0' && curr_char <= '9')
                {
                    vote_pref[i] = curr_char - '0';
                    read_candidates++;
                }
            }
            // Add candidates to Ballot in order of choosing
            // do NOT add the first order candidate, deal with that next
            Ballot ballot;
            for (int i = 1; i <= read_candidates; i++)
            {
                for (int j = 0; j < total_candidates; j++)
                {
                    if (i != 1 && vote_pref[j] == i)
                    {
                        ballot.addChoice(j);
                        break;
                    }
                }
            }
            // Store the ballot with the first order candidate
            for (int i = 0; i < total_candidates; i++)
            {
                if (vote_pref[i] == 1)
                {
                    candidates[i]->addBallot(ballot);

                    break;
                }
            }
        }
    }
    // close file and return
    ballotFileIR.close();
    return candidates;
}

/// @brief Gets index of a given candidate
/// @param name Name of candidate to get index of
int IR::getCandidateIndex(string name)
{
    for (int i = 0; i < (int)candidates.size(); i++)
    {
        if (name.compare(candidates[i]->getName()) == 0)
        {
            return i;
        }
    }
    return -1;
}

/// @brief redistributeVotes: Redistributes votes in an IR election
/// @param candidate Candidate who has been eliminated
void IR::redistributeVotes(Candidate *candidate)
{
    candidate->eliminated = true;
    // Loop through the candidate's ballots
    for (int i = 0; i < candidate->getNumVotes(); i++)
    {
        int choice;
        Ballot b = candidate->getBallots().front();
        do
        {
            choice = b.getChoice();
            if (choice == -1)
            {
                break;
            }
        } while (candidates[choice]->eliminated);
        if (choice != -1)
        {
            candidates[choice]->addBallot(b);
            candidate->removeBallot(&b);
        }
    }
}
