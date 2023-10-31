/**
 * @file ir.cc
 * @author Marcus Rana (rana0066) and Liam Mcguigan (mcgui479)
 *           Agile Refactoring by Lucas Olsen (olse0280)
 * @brief IR implementation
 */
#include "ir.h"
#include "candidate.h"

using namespace std;

/// @brief runElection: Runs the election
/// @return returns 0 to indicate success
int IR::runElection()
{
    Round round = Round(candidates);
    rounds.push_back(round);
    string temp = "Running the election\n";
    writeOutput(temp);
    if (candidates.size() == 0)
    {
        printResults();
        return -1;
    }
    // ensure all candidates are not eliminated at the start
    for (int i = 0; i < (int)candidates.size(); i++)
    {
        candidates[i]->eliminated = false;
    }

    // Initial vote tally header
    temp = "INITIAL VOTE TALLIES:";
    writeOutput(temp);
    text += "\n"; // extra newline in terminal output not in auditfile

    // Print initial candidate vote tallies
    for (int i = 0; i < (int)candidates.size(); i++)
    {
        if (candidates[i]->eliminated == false)
        {
            string audit_temp = " " + candidates[i]->getName() + " (";
            audit_temp += to_string(candidates[i]->getNumVotes()) + " votes)";
            writeToAuditFile(audit_temp);
            text += " " + candidates[i]->getName() + " (" + to_string(candidates[i]->getNumVotes()) + " votes)\n";
        }
    }

    // IR distribution loop
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
        {
            winner = candidates[max_index];
            setWinner(winner);
            string temp = "Candidate " + winner->getName() + " has won the election!\n";
            writeOutput(temp);
            printResults();
            printTable();
            cout << endl;  // add a newline to make the final output look nice
            return 0;
        }

        // Redistribution is necessary (no winner), find the candidate(s) with the least votes
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
            string temp = "Error: possible elimination candidate has no items.\n";
            writeOutput(temp);
            return -1;
        }

        // Print results header
        string temp = "RUNOFF ELIMINATION RESULTS: \n";
        temp += "Candidate " + to_elim->getName() + " has been eliminated!\n";
        temp += "The remaining candidates after redistribution are: ";
        writeOutput(temp);
        text += "\n"; // extra newline in terminal output not in auditfile

        // Print Candidates for the results
        for (int i = 0; i < (int)candidates.size(); i++)
        {
            if (candidates[i]->eliminated == false)
            {
                temp = " " + candidates[i]->getName() + " (";
                temp += to_string(candidates[i]->getNumVotes()) + " votes)";
                writeOutput(temp);
                text += '\n';
            }
        }
    }
    return 0; // never reached
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

/// @brief parseHeader: parses the header of a ballot file
/// @param ballotFileName name of the ballotFile
/// @return returns 1 on error, 0 on success.  Issues written to audit file
int IR::parseHeader(string ballotFileName)
{
    // ballot file
    ifstream ballotFileIR(ballotFileName);
    if (!ballotFileIR)
    {
        string err = "Error: " + ballotFileName + " could not be opened\n";
        writeOutput(err);
        return 1;
    }

    // read header info
    string doesNotMatter;
    string candidatesStr;

    // Skip first two lines
    getline(ballotFileIR, doesNotMatter);
    getline(ballotFileIR, doesNotMatter);
    // third line has the candidates
    getline(ballotFileIR, candidatesStr);

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
    return 0;
}

/// @brief parseBallots: Parses the ballots contained in the ballotFile
/// @param ballotFileName name of a ballot file
/// @return returns 1 on failure, 0 on success
int IR::parseBallots(string ballotFileName)
{
    // ballot file
    ifstream ballotFileIR(ballotFileName);
    if (!ballotFileIR)
    {
        string err = "Error: " + ballotFileName + " could not be opened\n";
        writeOutput(err);
        return 1;
    }

    // setup some vars for header matching
    string temp;
    string electionType;
    int numCandidates;
    int numBallots;

    // Confirm same election type
    getline(ballotFileIR, electionType);
    if (electionType != "IR")
    {
        string err = "Error: mismatching election types in " + ballotFileName + "\n";
        writeOutput(err);
        return 1;
    }
    // Confirm same number of candidates
    getline(ballotFileIR, temp);
    if (!ballotFileIR.fail())
    {
        numCandidates = stoi(temp);
        if ((int)candidates.size() != numCandidates)
        {
            string err = "Error: mismatching number of candidates in " + ballotFileName + "\n";
            writeOutput(err);
            return 1;
        }
    }
    else
    {
        string err = "Error reading candidates in " + ballotFileName + "\n";
        writeOutput(err);
        return 1;
    }

    getline(ballotFileIR, temp); // Skip candidates (line 3), we are assuming they are the same...
    getline(ballotFileIR, temp); // Get the number of ballots
    if (!ballotFileIR.fail())
    {
        numBallots = stoi(temp);
    }
    else
    {
        string err = "Error reading ballots in " + ballotFileName + "\n";
        writeOutput(err);
        return 1;
    }

    for (int i = 0; i < numBallots; i++)
    {
        string line;
        if (getline(ballotFileIR, line))
        {
            // Parse and store 1 valid ballot from the csv file
            istringstream ss(line);
            int read_candidates = 0;
            int total_candidates = (int)candidates.size();
            int vote_pref[total_candidates] = {};

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
            for (int i = 2; i <= read_candidates; i++)
            {
                for (int j = 0; j < total_candidates; j++)
                {
                    if (vote_pref[j] == i)
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
    ballotFileIR.close();
    return 0;
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
    Round round = Round(rounds.back(), candidates);
    candidate->eliminated = true;
    int candIndex = getCandidateIndex(candidate->getName());
    // Loop through the candidate's ballots
    Ballot temp;
    while (!candidate->removeBallot(&temp))
    {
        round.change_votes[candIndex]--;
        int choice;
        do
        {
            choice = temp.getChoice();
            if (choice == -1)
            {
                round.change_votes.back()++;
                break;
            }
        } while (candidates[choice]->eliminated);

        // Add to new candidate if OK choice
        if (choice != -1)
        {
            round.change_votes[choice]++;
            candidates[choice]->addBallot(temp);
        }
    }
    round.updateNumVotes();
    rounds.push_back(round);
}
void printBox(string str, int box_size)
{
    if ((int)str.length() > box_size)
    {
        for (int i = 0; i < (box_size); i++)
        {
            std::cout << str[i];
        }
    }
    else
    {
        for (int i = 0; i < (box_size - (int)str.length()); i++)
        {
            std::cout << " ";
        }
        std::cout << str << "|";
    }
}

void IR::printTable()
{
    int candidateNameBoxLen = 14;
    int partyNameBoxLen = 6;
    int voteBoxLen = 5;
    std::cout << "|";
    printBox("Candidates    ", candidateNameBoxLen + partyNameBoxLen + 1);

    for (int i = 0; i < (int)rounds.size(); i++)
    {
        string roundStr = "Round " + std::to_string(i + 1);
        printBox(roundStr, 2 * voteBoxLen + 1);
    }

    std::cout << std::endl;

    std::cout << "|";
    printBox("Candidate", candidateNameBoxLen);
    printBox("Party", partyNameBoxLen);
    for (int i = 0; i < (int)rounds.size(); i++)
    {
        printBox("Votes", voteBoxLen);
        printBox("+=", voteBoxLen);
    }

    std::cout << std::endl;
    int i = 0;
    int total = 0;
    for (; i < (int)rounds[0].getCandidates().size(); i++)
    {
        std::cout << "|";
        printBox(rounds[0].getCandidates()[i].getName(), candidateNameBoxLen);
        printBox(rounds[0].getCandidates()[i].getParty(), partyNameBoxLen);
        for (int j = 0; j < (int)rounds.size(); j++)
        {
            Round round = rounds[j];
            printBox(std::to_string(round.getNumVotes()[i]), voteBoxLen);
            printBox(std::to_string(round.getChangeVotes()[i]), voteBoxLen);
        }
        std::cout << std::endl;
    }

    std::cout << "|";
    printBox("EXHAUSTED PILE", candidateNameBoxLen);
    printBox(" ", partyNameBoxLen);
    for (int j = 0; j < (int)rounds.size(); j++)
    {
        Round round = rounds[j];
        printBox(std::to_string(round.getNumVotes()[i]), voteBoxLen);
        printBox(std::to_string(round.getChangeVotes()[i]), voteBoxLen);
    }
    std::cout << std::endl;
    std::cout << "|";

    for (int i = 0; i < (int)rounds[0].getCandidates().size(); i++)
    {
        total += rounds[0].getChangeVotes()[i];
    }
    printBox("TOTALS", candidateNameBoxLen);
    printBox(" ", partyNameBoxLen);
    printBox(std::to_string(total), voteBoxLen);
    printBox(std::to_string(total), voteBoxLen);

    std::cout << std::endl;
}

/// @brief writeOutput: helper to print to audit file and terminal
/// @param str string to output
void IR::writeOutput(string str)
{
    writeToAuditFile(str);
    text += str;
}
