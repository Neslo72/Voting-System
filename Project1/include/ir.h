/**
 * @file ir.h
 * @author Marcus Rana (rana0066)
 * @brief IR election class
 */

#ifndef IR_H_
#define IR_H_
#include <vector>
#include "candidate.h"
#include "election.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <stdio.h>
#include <unistd.h>
using namespace std;

/**
 * @class IR
 * @brief IR election class, inherits election, provides instant
 * runoff voting election capabilities
 */
class IR : public Election {
    public:
        /// @brief Default constructor: Construct a new IR object
        IR() {};

        /// @brief Default Destructor
        ~IR() {};

        /// @brief runElection: Runs the election 
        int runElection();

        /// @brief breakTie: Breaks a tie of multiple candidates
        /// @param tieVector vector of candidates in a tie
        /// @return selected candidate
        Candidate* breakTie(std::vector<Candidate*> tieVector);

        /// @brief readBallots: Reads all ballots
        /// @param ballotFileName name of ballotFile to read
        int readBallots(string ballotFileName);

        /// @brief Gets index of a given candidate
        /// @param name Name of candidate to get index of
        int getCandidateIndex(std::string name);

        /// @brief redistributeVotes: Redistributes votes in an IR election
        /// @param candidate Candidate who has been eliminated
        void redistributeVotes(Candidate* candidate);

        /// @brief returns the winner
        /// @return winner
        Candidate getWinner(){return winner;};
        
    private:
        //Vector of Candidates
        vector<Candidate> candidates;
        //Candidate which wins
        Candidate winner;
        
        //Ballotfile
        ifstream ballotFile;
};


#endif