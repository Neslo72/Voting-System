/**
 * @file party.h
 * @author Lucas Olsen
 * @brief Implementation of Party class
 * @date 2023-03-21
 */

#ifndef PARTY_H_
#define PARTY_H_

#include "candidate.h"
#include <string>
#include <vector>

/**
 * @class Party
 * @brief Party class, used to store relevant information for party objects
 */
class Party
{
  public:
    /// @brief Constructor
    Party();

    /// @brief Destructor
    ~Party(){};

    /// @brief Add an input number of ballots to the party's ballot total
    /// @param x Number to add to ballot total
    void addBallots(int x)
    {
        ballotTotal += x;
    };

    /// @brief Increment ballot total by 1
    void incBallotTotal()
    {
        ballotTotal++;
    };

    /// @brief Getter for ballot Total
    /// @return ballot total
    int getBallotTotal()
    {
        return ballotTotal;
    };

    /// @brief "Win seat," adds to winSeats depending on remaining party candidates
    int winSeats(int x);

    /// @brief Getter for seatsWon
    /// @return seatsWon
    int getSeatsWon()
    {
        return seatsWon;
    };

    /// @brief Getter for the number of members in the Party
    /// @return number of Members
    int getNumMembers()
    {
        return members.size();
    };

    /// @brief Getter for the party's members string vector
    /// @return Members string vector
    std::vector<Candidate> getMembers()
    {
        return members;
    };

    /// @brief Add a member to the members string vector
    /// @param _member member to add to the members vector
    void addMember(Candidate _member);

    /**
     * @brief Get the winners from this party, based on numSeats
     * and members vector.
     * @return String vector of winning candidate from this party
     */
    std::vector<std::string> getWinners();

    /// @brief take away ballots from party
    /// @param x num ballots taken away
    void decrementBallots(int x)
    {
        ballotTotal -= x;
    }

    std::string partyName;

  private:
    // Total ballots (vote count)
    int ballotTotal;
    // Number of seats won
    int seatsWon;
    // Candidate vector of party members
    std::vector<Candidate> members;
};

#endif