/**
 * @file ballot.h
 * @author Lucas Olsen
 * @brief Header file for ballot object
 * @date 2023-03-21
 */
#ifndef BALLOT_H_
#define BALLOT_H_

#include <string>
#include <queue>

/**
 * @class Ballot
 * @brief Ballot class: Stores a vector of candidate choices to be
 * used in an IR election
 */
class Ballot {
    public:

        /// @brief Constructor: Makes a new, empty ballot object
        Ballot() {};

        /// @brief Destructor
        ~Ballot() {};

        /// @brief adds a candidate choice to the ballots queue
        /// @param toAdd candidate choice to add
        void addChoice(int choice);
        
        /**
         * @brief Gets the first choice of this ballot, removes
         * said choice from choices queue
         * @return Returns the choice index removed from choices, -1 if nothing left
         */
        int getChoice();
        std::queue<int> getChoices(){return choices;};
    private:
        /// Queue of candidate choices stored as ints
        std::queue<int> choices;
};

#endif