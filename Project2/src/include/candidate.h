/**
 * @file candidate.h
 * @author Lucas Olsen
 * @brief Header file for candidate objects
 * @date 2023-03-21
 */

#ifndef CANDIDATE_H_
#define CANDIDATE_H_

#include "ballot.h"
#include <queue>
#include <string>

/**
 * @class Candidate
 * @brief Candidate class: Stores various fields related to a candidate
 *
 */
class Candidate
{
  public:
    /// @brief Default constructor: Construct a new Candidate object
    Candidate(){};
    /// @brief  Constructor, creates new Candidate objec t
    /// @param _name name for candidate object
    /// @param _party party of the candidate object
    Candidate(std::string _name, std::string _party);

    /// @brief Destructor
    ~Candidate(){};

    /// @brief Getter for name
    /// @return returns candidate's name
    std::string getName()
    {
        return name;
    };

    /// @brief Getter for party
    /// @return returns candidate's party
    std::string getParty()
    {
        return party;
    };

    /// @brief Getter for the candidate's vote total
    /// @return returns the size of the ballots vector
    int getNumVotes()
    {
        return (int) ballots.size();
    };

    /// @brief Add a ballot to the candidate's ballot vector
    /// @param _ballot the ballot to add
    void addBallot(Ballot _ballot);

    /// @brief Removes a ballot from the candidate's ballot vector
    /// @param removedBallot pointer to where removed ballot will be stored
    /// @return returns 0 on success, 1 on error
    int removeBallot(Ballot *removedBallot);

    /// bool to determine elimination
    bool eliminated;
    std::queue<Ballot> getBallots()
    {
        return ballots;
    };

  private:
    /// Candidate's ballot vector
    std::queue<Ballot> ballots;
    /// Candidate name
    std::string name;
    /// Candidate party
    std::string party;
};

#endif