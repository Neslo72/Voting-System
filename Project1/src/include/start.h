/**
 * @file start.h
 * @author Justin Lau
 * @brief Begins the election system & processes the ballot file
 */

/// START_H_
#ifndef START_H_
#define START_H_

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

/**
 * @class Start
 * @brief Start Class: Begins the election system & processes the ballot file
 */
class Start
{
  public:
    /// @brief Constructor: Construct a new Start object
    Start(){};

    /// @brief Destructor: Destroy the Start object
    ~Start(){};

    /// @brief reset function to handle an interruption while the system is running
    void reset();

    /// @brief prompts the user for a valid ballot file
    void enterBallotFile();

    /**
     * @brief calculates the type of election based on the first
     * line of the ballot file
     *
     * @param ballotFileName the name of the ballot file
     * @return string: the type of election ('CPL' or 'IR')
     */
    string calculate(string ballotFileName);

    /**
     * @brief Main function to begin the voting system
     * which handles the ballot file & determines the election type
     *
     * @param argc number of command line arguments
     * @param argv array of command line arguments
     * @return int: 0 on success and 1 on failure
     */
    int main(int argc, char *argv[]);

  private:
    /// provided ballot files
    ifstream ballotFile;
    /// the type of election
    string electionType;
    /// number of ballots
    int numBallots;
};

#endif /// START_H_