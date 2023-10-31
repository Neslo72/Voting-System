/**
 * @file election.h
 * @author Justin Lau
 * @brief Handles parsing the ballot file to determine the number of ballots,
 * creates the audit file, writes to the audit file, and prints the results of
 * the election
 */

/// ELECTION_H_
#ifndef ELECTION_H_
#define ELECTION_H_

#include "start.h"
#include <cstdio>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// using namespace std;

/**
 * @class Election
 * @brief Sets-up basic election processing before running election algorithms \n
 * Determines the number of ballots, creates the audit file, writes to the audit file, and
 * prints the results of the election
 */
class Election
{
  public:
    /// @brief Constructor: Construct a new Election object
    Election(){};

    /// @brief Destructor: Destroy the Election object
    ~Election(){};

    /// @brief Virtual function to be implemented by either the CPL or IR classes
    // virtual int runElection(string ballotFileName);

    /**
     * @brief Creates the audit file
     *
     * @param auditFileName name of the audit file
     * @param ballotFileName name of the provided ballot file
     * @return string: path to the audit file
     */
    string makeAuditFile(string electionType);

    /**
     * @brief Writes to the audit file
     *
     * @param text text to write to the file
     */
    void writeToAuditFile(string text);

    /**
     * @brief prints the results of the election
     */
    void printResults(string text);

  protected:
    /// name of the provided ballot file
    string ballotName;
    // auditFile Name
    string auditFileName;

  private:
    /// path to created audit file
    string path;
    // ballot file
    fstream ballotFile;
    // Audit file
    ofstream auditFile;
};

#endif /// ELECTION_H_