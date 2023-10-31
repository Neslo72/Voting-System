/**
 * @file poppularonly.h
 * @author Marcus Rana (rana0066)
 * @brief PO election class
 */

#ifndef PO_H_
#define PO_H_
#include "candidate.h"
#include "election.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>
using namespace std;

/**
 * @class PO
 * @brief PO election class, inherits election, provides instant
 * runoff voting election capabilities
 */
class PO : public Election
{
  public:
    /// @brief Default constructor: Construct a new PO object
    PO(){};

    /// @brief Default Destructor
    ~PO(){};

    /// @brief runElection: Runs the election
    /// @param aFileName name of the audit file
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

    /// @brief printResults: Print out the result of the election to terminal
    void printResults() { cout << text << endl; };

  private:
    vector<Candidate *> candidates;
    // Candidate which wins
    Candidate *winner;
    // Ballotfile
    ifstream ballotFile;
    // Election object to print results
    Election election;
    // string text to print text
    string text;
    // int to store the number of ballots
    int numBallots = 0;
};

#endif