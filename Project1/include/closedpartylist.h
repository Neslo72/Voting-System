/**
 * @file closedpartylist.h
 * @author Marcus Rana (rana0066)
 * @brief CPL election header file
 */

#ifndef CPL_H_
#define CPL_H_
#include <vector>
#include "candidate.h"
#include "party.h"
#include "election.h"
#include <fstream>   
#include <sstream>
#include <cstdio>
#include <unistd.h>
#include<cstdlib>
#include<iostream>
#include<string>

/**
 * @class CPL
 * @brief CPL class, inherits Election, run closed party list
 * election type functionality
 */
class CPL : public Election {
    public:
        /// @brief Constructor
        CPL() {};
        
        /// @brief Destructor
        ~CPL() {};
        /// @brief runElection: Runs election
        int runElection();
    
        /// @brief breaks a tie between parties
        /// @param tieVector: Parties of which are tied
        /// @return selected party of tie
        Party* breakTie(std::vector<Party*> tieVector);

        /// @brief readBallots: reads ballots
        int readBallots(std::string ballotFileName);

        /// @brief assignSeats: Assign seats to Parties
        void assignSeats();

        /// @brief getHighestRemainderIndex: Gets the index of the party with the highest remainder
        /// @return returns index of party with highest remainder
        int getHighestRemainderIndex();
        
        /// @brief gets the winners
        /// @return winners
        std::vector<Candidate> getWinners(){return winner;};

        //Vector of parties
        std::vector<Party> parties;

        //Number of total seats available
        int num_seats;

    private:
        //Vector of ballots
        std::vector<std::vector<int> > ballots;

        //Number of total ballots
        int num_ballots;

        //Number of candidates
        int num_candidates;

        //Vector of candidates which won
        std::vector<Candidate> winner;
};

#endif