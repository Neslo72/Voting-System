/**
 * @file ir.h
 * @author Marcus Rana (rana0066)
 * @brief IR election class
 */

#ifndef IR_H_
#define IR_H_
#include "round.h"
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
    /// @return returns 0 to indicate success
    int runElection();

    /// @brief breakTie: Breaks a tie of multiple candidates
    /// @param tieVector vector of candidates in a tie
    /// @return selected candidate
    Candidate *breakTie(std::vector<Candidate *> tieVector);

    /// @brief parseHeader: Parses the header of a ballot file
    /// @param ballotFileName name of ballot file to parse
    /// @return returns 1 on failure, 0 on success
    int parseHeader(string ballotFileName);

    /// @brief parseBallots: Parses the ballots contained in the ballotFile
    /// @param ballotFileName name of a ballot file
    /// @return returns 1 on failure, 0 on success
    int parseBallots(string ballotFileName);

    /// @brief Gets index of a given candidate
    /// @param name Name of candidate to get index of
    int getCandidateIndex(std::string name);

    /// @brief redistributeVotes: Redistributes votes in an IR election
    /// @param candidate Candidate who has been eliminated
    void redistributeVotes(Candidate *candidate);

    /// @brief printTable: Prints the table of rounds
    void printTable();
    
    /// @brief writeOutput: helper to print to audit file and terminal
    /// @param str string to output
    void writeOutput(string str);

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

    /// @brief printResults:  Print out the result of the election to terminal
    void printResults() { cout << text << endl; };

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
    //stores each round in IR election
    vector<Round> rounds;
};

#endif