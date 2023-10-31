/**
 * @file round.h
 * @author Marcus Rana
 * @brief Header file for round objects
 * @date 2023-04-19
 */

#ifndef ROUND_H_
#define ROUND_H_

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>
#include "candidate.h"
using namespace std;

/**
 * @class Round
 * @brief Round class: Stores various fields related to a round in IRV election
 *
 */
class Round
{
  public:
    Round(Round former_round, vector<Candidate*> candidates_ptr);
    Round(vector<Candidate*> candidates_ptr);
    vector<int> getChangeVotes(){return change_votes;};
    vector<Candidate> getCandidates(){return candidates;};
    vector<int> getNumVotes(){return num_votes;};
    

    void updateNumVotes();
    vector<int> change_votes;
  private:
    // Vector of Candidates
    vector<Candidate> candidates;
    vector<int> num_votes;
};

#endif