# See https://youtu.be/Wt6KGlMkLz0
# for up to line 29

CXX = g++
CXXFLAGS = -g -Wall -Werror -I $(INC)

TARGET = start

SRC = source
INC = include
TEST = testing
BIN = bin

SOURCE = $(wildcard $(SRC)/*.cc)
TESTING = $(wildcard $(TEST)/*.cc)
OBJECT = $(patsubst %,$(BIN)/%, $(notdir $(SOURCE:.cc=.o)))


########## PROGRAM FILES ##########


$(TARGET) : $(OBJECT)
	$(CXX) -o $@ $^

# makes all .o files in /bin directory
$(BIN)/%.o : $(SRC)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@
	

########## TESTING ##########

# Variable definitions
TEST_EXEC = testing/executables

BALLOT_T = $(TEST_EXEC)/BallotTest
CANDIDATE_T = $(TEST_EXEC)/CandidateTest
CPL_T = $(TEST_EXEC)/CPLTest
ELECTION_T = $(TEST_EXEC)/ElectionTest
IR_T = $(TEST_EXEC)/IRTest
PARTY_T = $(TEST_EXEC)/PartyTest

# makes all tests (when they work...)
tests : $(BALLOT_T) $(CANDIDATE_T) $(PARTY_T) $(CPL_T) # $(IR_T) $(ELECTION_T)


$(BALLOT_T) : $(BIN)/ballot.o $(TEST)/BallotTest.cc
	$(CXX) $^ -lgtest -lpthread -o $@

$(CANDIDATE_T) : $(BIN)/candidate.o $(TEST)/CandidateTest.cc
	$(CXX) $^ -lgtest -lpthread -o $@

$(CPL_T) : $(BIN)/closedpartylist.o $(BIN)/candidate.o $(BIN)/party.o $(BIN)/election.o $(TEST)/CPLTest.cc 
	$(CXX) $^ -lgtest -lpthread -o $@

# PROBLEM with double defined main (start has main and so does ElectionTest)...
$(ELECTION_T) : $(BIN)/election.o $(BIN)/start.o $(TEST)/ElectionTest.cc
	$(CXX) $^ -lgtest -lpthread -o $@

# IRTests not working yet
$(IR_T) : $(BIN)/ir.o $(TEST)/IRTest.cc
	$(CXX) $^ -lgtest -lpthread -o $@

$(PARTY_T) : $(BIN)/party.o $(BIN)/candidate.o $(TEST)/PartyTest.cc
	$(CXX) $^ -lgtest -lpthread -o $@


########## MISC ##########


clean: 
	rm -f $(TARGET)
	rm -f $(OBJECT)
	rm -f $(BALLOT_T) $(CANDIDATE_T) $(CPL_T) $(ELECTION_T) $(IR_T) $(PARTY_T)

help:
	@echo 'make'
	@echo 'make tests'
	@echo 'make clean'
