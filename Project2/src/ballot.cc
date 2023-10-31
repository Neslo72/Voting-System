/**
 * @file ballot.cc
 * @author Lucas Olsen
 * @brief Implementation of ballot class functions
 * @date 2023-03-21
 */
#include "ballot.h"

/// @brief Adds a choice to the ballot's choices vector
/// @param toAdd The choice to add to the choices vector
void Ballot::addChoice(int toAdd)
{
    choices.push(toAdd);
}

/// @brief Gets the first choice from the choices vector.  Pops it from choices
/// @return returns the string of first choice, or NULL if nothing left
int Ballot::getChoice()
{
    if (!choices.empty())
    {
        int ret = choices.front();
        choices.pop();
        return ret;
    }
    return -1;
}