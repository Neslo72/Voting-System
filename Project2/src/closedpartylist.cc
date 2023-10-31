/**
 * @file closedpartylist.cc
 * @author Marcus Rana (rana0066) and Liam Mcguigan (mcgui479)
 * @brief CPL implementation
 */

#include "closedpartylist.h"
#include "party.h"

using namespace std;

/// @brief runElection Runs election
/// @return returns 0 to indicate success
int CPL::runElection()
{
    for (int i = 0; i < (int)names.size(); i++)
    {
        parties[i].partyName = names[i];
    }

    writeToAuditFile("Running the election.\n");
    text += "Running the election.\n";
    if (names.size() == 0)
    {
        return -1;
    }

    srand(time(0));  // set random seed for later breaktie operations...
    assignSeats();//Assign seats to parties

    writeToAuditFile("CPL ELECTION RESULTS: \n");
    text += "CPL ELECTION RESULTS: \n";

    for (int i = 0; i < (int)parties.size(); i++)
    { // Assign Winners to winner list
        for (int j = 0; j < parties[i].getSeatsWon(); j++)
        {
            winner.push_back(parties[i].getMembers()[j]);
        }
    }
    // Write Winners to the audit file
    for (int i = 0; i < (int)winner.size(); i++)
    {
        writeToAuditFile("Winner: " + winner[i].getName());
        text += "Winner: " + winner[i].getName() + "\n";
    }
    printResults(text);
    return 0;
}

/// @brief breaks a tie between parties
/// @param tieVector: Parties of which are tied
/// @return returns the party that gets the winning seat
Party* CPL::breakTie(std::vector<Party *> tieVector) {
    
    if(tieVector.size() < 1) {  // empty tieVector (bad)
        return nullptr;
    }
    if(tieVector.size() == 1) {  // 1 party in the 'tie'
        return tieVector[0];
    }

    // break an actual tie
    int num_test = 100000 + rand() % 500000;
    int rand_value = 0;
    for (int i = 0; i < num_test; i++)
    {
        rand_value = rand() % tieVector.size();
    }
    return tieVector[rand_value];
}

/// @brief getHighestRemainderIndex: Gets the index of the party with the highest remainder
/// @return returns pointer to party with highest remainder
Party* CPL::getHighestRemainderIndex()
{
    int max = 0;
    std::vector<Party*> tieVector;

    for(int i = 0; i < (int)parties.size(); i++) 
    {
        if(parties[i].getBallotTotal() > max) {  // new max case
            max = parties[i].getBallotTotal();
            tieVector.clear();
            tieVector.push_back(&parties[i]);
        }
        else if (parties[i].getBallotTotal() == max) {  // prep tie case 
            tieVector.push_back(&parties[i]);
        }
    }
    return breakTie(tieVector);
}

/// @brief readBallots: reads ballots
/// @return 0 to indicate succes, -1 on failure
int CPL::readBallots(string ballotFileName)
{
    ifstream BallotFileCP(ballotFileName);
    // Checking for the file openging properly
    if (!BallotFileCP)
    {
        writeToAuditFile("Could not open the ballot file.  Please try again.\n");
        text += "Could not open the ballot file.  Please try again.\n";
        printResults(text);
        exit(-1);
    }
    // election type
    getline(BallotFileCP, electionTypeCP);

    // Reading the second line for number of parties
    string numPartiesString;

    getline(BallotFileCP, numPartiesString);
    numParties = stoi(numPartiesString);
    parties.resize(numParties);
    string line;

    // getting the name of parties
    getline(BallotFileCP, line);
    stringstream ss(line);
    for (int j = 0; j < numParties; j++)
    {
        getline(ss, partyName, ',');
        names.push_back(partyName);
    }
    string temp;
    for (int j = 0; j < numParties; j++)
    {
        // read the 4th line through offset to get candidates
        getline(BallotFileCP, temp);

        // adding members to parties
        stringstream ss(temp);
        string name;
        while (getline(ss, name, ','))
        {
            Candidate candidate(name, names[j]);
            parties[j].addMember(candidate);
        }
    }

    // right after candidates gettint number of seats
    getline(BallotFileCP, numSeats);
    num_seats = stoi(numSeats);
    // Read the following line to get total ballots
    getline(BallotFileCP, numberBallots);
    num_ballots = stoi(numberBallots);
    // getting ballet choices
    for (int i = 0; i < num_ballots; i++)
    {
        // string line;
        getline(BallotFileCP, line);
        size_t choice = line.find('1');
        // Increments the party based on choice
        parties[choice].incBallotTotal();
    }
    BallotFileCP.close();
    election.printResults(text);
    return 0;
}

/// @brief assignSeats: Assign seats to Parties
void CPL::assignSeats()
{
    int seat_thresh = num_ballots / num_seats;
    int seats_left = num_seats;

    // Assign seats to parties over the threshold
    for(int i = 0; i < (int)parties.size(); i++) 
    {
        int seatsWon = parties[i].getBallotTotal() / seat_thresh;
        if(seatsWon > 0) {
            seatsWon -= parties[i].winSeats(seatsWon);  //subtract any extra allocated seats
            seats_left -= seatsWon;
            parties[i].decrementBallots(seat_thresh * seatsWon);
        }
    }

    // Loop over the remaining seats
    int inf_flag = 0;  // Flag to prevent infinite loop
    while(seats_left > 0) 
    {
        Party* to_win = getHighestRemainderIndex();
        if(to_win == nullptr || inf_flag >= numParties) {
            // Break out on the infinite cases:
            // no parites -- no remaining candidates in the parties
            break;
        }
        if(to_win->winSeats(1) == 0) {  // Only add seats if the party has candidates to recieve
            seats_left--;
            inf_flag = 0;
        }
        to_win->decrementBallots(to_win->getBallotTotal());  // remove from furhter contention
        inf_flag++;
    }
}