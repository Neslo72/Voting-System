/**
 * @file closedpartylist.h
 * @author Marcus Rana (rana0066)
 * @brief CPL election header file
 */

#ifndef CPL_H_
#define CPL_H_
#include "candidate.h"
#include "election.h"
#include "party.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

/**
 * @class CPL
 * @brief CPL class, inherits Election, run closed party list
 * election type functionality
 */
class CPL : public Election
{
  public:
    /// @brief Constructor
    CPL(){};

    /// @brief Destructor
    ~CPL(){};
    /// @brief runElection: Runs election
    int runElection();

    /// @brief breaks a tie between parties
    /// @param tieVector: Parties of which are tied
    /// @return selected party of tie
    Party *breakTie(std::vector<Party *> tieVector);

    /// @brief readBallots: reads ballots
    int readBallots(std::string ballotFileName);

    /// @brief assignSeats: Assign seats to Parties
    void assignSeats();

    /// @brief getHighestRemainderIndex: Gets the index of the party with the highest remainder
    /// @return returns pointer to party with highest remainder
    Party* getHighestRemainderIndex();

    /// @brief gets the winners
    /// @return winners
    std::vector<Candidate> getWinners()
    {
        return winner;
    };

    // Vector of parties
    std::vector<Party> parties;

    // Number of total seats available
    int num_seats;

    // getters for testing
    string getElectionTypeCP() const
    {
        return electionTypeCP;
    }
    int getNumParties() const
    {
        return numParties;
    }
    string getNumSeats() const
    {
        return numSeats;
    }
    int getNumBallots() const
    {
        return num_ballots;
    }
    vector<string> getNames() const
    {
        return names;
    }

  private:
    string electionTypeCP;
    int numParties;
    string partyName;
    string numSeats;
    string numberBallots;
    vector<string> names;
    // Vector of ballots
    std::vector<std::vector<int>> ballots;

    // name of audit file
    string auditFileName;

    // Number of total ballots
    int num_ballots;

    // Number of candidates
    int num_candidates;

    // Vector of candidates which won
    std::vector<Candidate> winner;

    // Election object to print results
    Election election;

    // string text to print text
    string text;
};

#endif