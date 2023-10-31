/**
 * @file candidate.cc
 * @author Lucas Olsen
 * @brief Candidate class C implementation
 * @date 2023-03-21
 */

#include "candidate.h"

using namespace std;

/// @brief Constructor for Candidate object
/// @param _name Name for the new candidate
/// @param _party Party for the new candidate
Candidate::Candidate(string _name, string _party)
{
    name = _name;
    party = _party;
}

/// @brief Add a ballot to this candidate's ballots queue
/// @param _ballot The ballot to be added to ballots
void Candidate::addBallot(Ballot _ballot)
{
    ballots.push(_ballot);
}

/**
 * @brief Remove a ballot from the candidate's ballots vector.  The removed
 * ballot will be stored to the location pointed to by the removedBallot
 * pointer.
 * @param removedBallot pointer to where the removed ballot will be stored
 * @return Returns 0 on succes, 1 on failure
 */
int Candidate::removeBallot(Ballot *removedBallot)
{
    if (removedBallot != nullptr && !ballots.empty())
    {
        *removedBallot = ballots.front();
        ballots.pop();
        return 0;
    }
    return 1;
}