/**
 * @file ir.h
 * @author Marcus Rana (rana0066)
 * @brief IR election class
 */

#ifndef IR_H_
#define IR_H_
#include "candidate.h"
#include "election.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>
using namespace std;

/**
 * @class IR
 * @brief IR election class, inherits election, provides instant
 * runoff voting election capabilities
 */
class IR : public Election
{
  public:
    /// @brief Default constructor: Construct a new IR object
    IR(){};

    /// @brief Default Destructor
    ~IR(){};

    /// @brief runElection: Runs the election
    /// @param cansFromBallots vector of candidates created by readBallots
    /// @param aFileName name of the audit file
    int runElection(std::vector<Candidate *> &cansFromBallots, string auditFileName);

    /// @brief breakTie: Breaks a tie of multiple candidates
    /// @param tieVector vector of candidates in a tie
    /// @return selected candidate
    Candidate *breakTie(std::vector<Candidate *> tieVector);

    /// @brief readBallots: Reads all ballots
    /// @param ballotFileName name of ballotFile to read
    /// @param aFileName name of the audit file
    std::vector<Candidate *> readBallots(string ballotFileName, string auditFileName);

    /// @brief Gets index of a given candidate
    /// @param name Name of candidate to get index of
    int getCandidateIndex(std::string name);

    /// @brief redistributeVotes: Redistributes votes in an IR election
    /// @param candidate Candidate who has been eliminated
    void redistributeVotes(Candidate *candidate);

    /// @brief returns the winner
    /// @return winner
    Candidate getWinner()
    {
        return *winner;
    };

    /// @brief sets the winner
    /// @param winner Candidate who has won the election
    void setWinner(Candidate *candidate)
    {
        winner = candidate;
    };

    string printing()
    {
        return text;
    };

  private:
    // Vector of Candidates
    vector<Candidate *> candidates;
    // Candidate which wins
    Candidate *winner;
    // Ballotfile
    ifstream ballotFile;
    // Election object to print results
    Election election;
    // string text to print text
    string text;
};

#endif