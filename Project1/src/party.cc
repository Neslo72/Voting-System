/**
 * @file party.cc
 * @author Lucas Olsen
 * @brief Party class implementation
 * @date 2023-03-21
 */
#include "party.h"

using namespace std;

/// @brief Constructs a party object, sets inital fields
Party::Party()
{
    ballotTotal = 0;
    seatsWon = 0;
}

/// @brief "Win seat," increment seatsWon variable
/// @return the number of extra seats not assigned to the party
int Party::winSeats(int x)
{
    int seats_left = members.size() - seatsWon;
    if (x <= seats_left)
    {
        seatsWon += x;
        return 0;
    }
    seatsWon += seats_left;
    return (x - seats_left);
}

/// @brief Adds a member to the part class's members vector
/// @param _member member to be added to members
void Party::addMember(Candidate _member)
{
    members.push_back(_member);
}

/**
 * @brief Gets a vector of the seat winners from this party
 * Based on seatsWon and members vector.
 *
 * @return String vector of winners
 */
vector<string> Party::getWinners()
{
    vector<string> winners;
    for (int i = 0; i < seatsWon; i++)
    {
        winners.push_back(members.at(i).getName());
    }
    return winners;
}
