/**
 * @file closedpartylist.cc
 * @author Marcus Rana (rana0066) and Liam Mcguigan (mcgui479)
 * @brief CPL implementation
 */

#include "closedpartylist.h"
#include "party.h"

using namespace std;

string electionTypeCP; 
int numParties;   
string partyName;
string numSeats;
string numberBallots;
vector <string> names;

/// @brief runElection Runs election
/// @return returns 0 to indicate success
int CPL::runElection() {
    //readBallots(ballotFileName);//Read the ballots

    for(int i = 0; i < (int) names.size(); i++) {
        parties[i].partyName = names[i];
    }

    writeToAuditFile("Running the election.\n");

    assignSeats();//Assign seats to parties

    writeToAuditFile("CPL ELECTION RESULTS: \n");

    for(int i = 0;i<(int)parties.size();i++){//Assign Winners to winner list
        for(int j = 0;j<parties[i].getSeatsWon();j++){
            winner.push_back(parties[i].getMembers()[j]);
        }
    }

    // Write Winners to the audit file
    for(int i = 0; i < (int) winner.size(); i++) {
        writeToAuditFile("Winner: " + winner[i].getName());
    }

    return 0;
}

/// @brief breaks a tie between parties
/// @param tieVector: Parties of which are tied
/// @return returns the party that gets the winning seat
Party* CPL::breakTie(std::vector<Party *> tieVector) {
    srand(time(0));
    int num_test = 100000 + rand() % 500000;
    int rand_value = 0;
    for(int i = 0;i<num_test;i++){
        rand_value = rand() % tieVector.size();
    }
    return tieVector[rand_value];
}

/// @brief getHighestRemainderIndex: Gets the index of the party with the highest remainder
/// @return returns index of party with highest remainder
int CPL::getHighestRemainderIndex(){
    int max = 0;
    int max_index = 0;
    std::vector<Party *> tieVector;
    for(int i = 0;i<(int)parties.size();i++){//Find the party with the highest number of votes
        if(parties[i].getBallotTotal()>max){
            max = parties[i].getBallotTotal();
            max_index = i;
            tieVector.push_back(&parties[i]);
        }
    }

    for(int i = 0;i<(int)parties.size();i++){//Check if there is a tie
        if(parties[i].getBallotTotal()==max && i!=max_index){
            tieVector.push_back(&parties[i]);
        }
    }

    Party highest_remainder = *breakTie(tieVector);//Deal with tie
    for(int i = 0;i<(int)parties.size();i++){//Get highest index
        if(!parties[i].partyName.compare(highest_remainder.partyName)){
            return i;
        }
    }
    return -1;
}

/// @brief readBallots: reads ballots
/// @return 0 to indicate succes, -1 on failure
int CPL::readBallots(string ballotFileName) {
    // change director to 'Project1' folder
    int ch = chdir("../..");        
    writeToAuditFile("Reading ballots...\n");
    ifstream BallotFileCP(ballotFileName);
    //Checking for the file openging properly
    if(ch == 0){
    if(!BallotFileCP){
        cout << "Could not open the ballot file.  Please try again." << endl;
        exit(-1);
    }
    //election type
    getline(BallotFileCP, electionTypeCP);

    //Reading the second line for number of parties
    string numPartiesString;

    getline(BallotFileCP, numPartiesString);
    numParties = stoi(numPartiesString);
    parties.resize(numParties);
    string line;

    //getting the name of parties
    getline(BallotFileCP, line);
    stringstream ss(line);
    for(int j = 0; j < numParties; j++){
        getline(ss, partyName, ',');
        names.push_back(partyName);
    }
    string temp;
    for(int j = 0; j < numParties; j++){
        //read the 4th line through offset to get candidates
        getline(BallotFileCP, temp);

        //adding members to parties
        stringstream ss(temp);
        string name;
        while(getline(ss, name, ',')){
            Candidate candidate(name, names[j]);
            parties[j].addMember(candidate);
        }
    }

    //right after candidates gettint number of seats
    getline(BallotFileCP, numSeats);
    num_seats = stoi(numSeats);
    //Read the following line to get total ballots
    getline(BallotFileCP, numberBallots);
    num_ballots = stoi(numberBallots);
    //getting ballet choices
    for(int i = 0; i < num_ballots; i++){
        //string line;
        getline(BallotFileCP, line);
        size_t choice = line.find('1');
        //Increments the party based on choice
        parties[choice].incBallotTotal();
    }
    BallotFileCP.close();
    }

    //Print for testing
    for (int i = 0; i < (int)parties.size(); i++) {
        cout << "Party: " << names[i] << " num votes: " << parties[i].getBallotTotal() << endl;
        cout << "Num members: " << parties[i].getNumMembers() << endl;
    }
    return 0;
}

/// @brief assignSeats: Assign seats to Parties
void CPL::assignSeats(){
    int boundary = num_ballots/num_seats;
    int total_seats = num_seats;
    for(int i = 0;i<(int)parties.size();i++){//Assign seats to parties
        int seatsWon = parties[i].getBallotTotal()/boundary; 
        parties[i].winSeats(seatsWon);
        parties[i].decrementBallots(seatsWon*boundary);
        total_seats-=seatsWon;
    }
    while(total_seats!=0){//Assign remaining seats to parties
        int ind = getHighestRemainderIndex();
        parties[ind].winSeats(1);
        parties[ind].decrementBallots(parties[ind].getBallotTotal());
        total_seats--;
    }
}