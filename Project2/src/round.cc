#include "round.h"

Round::Round(Round former_round, vector<Candidate *> candidates_ptr)
{
    // for (int i = 0; i < (int)former_round.GetCandidates().size(); i++){
    //     candidates.push_back(former_round.GetCandidates()[i]);
    // }
    for (int i = 0; i < (int)candidates_ptr.size(); i++)
    {
        candidates.push_back(*candidates_ptr[i]);
        num_votes.push_back(former_round.getNumVotes()[i]);
        change_votes.push_back(0);
    }
    num_votes.push_back(former_round.getNumVotes().back());
    change_votes.push_back(0);
}

Round::Round(vector<Candidate *> candidates_ptr)
{
    for (int i = 0; i < (int)candidates_ptr.size(); i++)
    {
        candidates.push_back(*candidates_ptr[i]);
    }
    for (int i = 0; i < (int)candidates.size(); i++)
    {
        num_votes.push_back(candidates[i].getNumVotes());
        change_votes.push_back(candidates[i].getNumVotes());
    }
    // TODO these should have some values, need to fix this
    num_votes.push_back(0);
    change_votes.push_back(0);
}
void Round::updateNumVotes(){
    for(int i = 0;i<(int)num_votes.size();i++){
        num_votes[i]+=change_votes[i];
    }
}