CXX = g++
CXXFLAGS = -g -Wall -Werror -I $(INC)

TARGET = start

INC = include
TEST = gtest_code
BIN = bin


########## PROGRAM FILES ##########

BALLOT_O = $(BIN)/ballot.o
CANDIDATE_O = $(BIN)/candidate.o
ROUND_O = $(BIN)/round.o
CPL_O = $(BIN)/closedpartylist.o
PO_O = $(BIN)/popularonly.o
ELECTION_O = $(BIN)/election.o
IR_O = $(BIN)/ir.o
PARTY_O = $(BIN)/party.o

all : directories $(TARGET)

directories :
	$(shell mkdir -p bin)
	$(shell mkdir -p ../misc/AuditFiles)
#	$(shell mkdir -p ../misc/InvalidBallots)

$(TARGET) : $(BALLOT_O) $(CANDIDATE_O) $(ROUND_O) $(CPL_O) $(ELECTION_O) $(IR_O) $(PO_O) $(PARTY_O) start.cc
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BALLOT_O) : ballot.cc
	$(CXX) $(CXXFLAGS) $^ -c -o $@

$(ROUND_O) : round.cc
	$(CXX) $(CXXFLAGS) $^ -c -o $@

$(CANDIDATE_O) : candidate.cc
	$(CXX) $(CXXFLAGS) $^ -c -o $@

$(CPL_O) : closedpartylist.cc
	$(CXX) $(CXXFLAGS) $^ -c -o $@

$(PO_O) : popularonly.cc
	$(CXX) $(CXXFLAGS) $^ -c -o $@

$(ELECTION_O) : election.cc
	$(CXX) $(CXXFLAGS) $^ -c -o $@

$(IR_O) : ir.cc
	$(CXX) $(CXXFLAGS) $^ -c -o $@

$(PARTY_O) : party.cc
	$(CXX) $(CXXFLAGS) $^ -c -o $@


########## TESTING FILES ##########

TEST_EXEC = gtest_code/executables

BALLOT_T = $(TEST_EXEC)/BallotTest
CANDIDATE_T = $(TEST_EXEC)/CandidateTest
CPL_T = $(TEST_EXEC)/CPLTest
ELECTION_T = $(TEST_EXEC)/ElectionTest
IR_T = $(TEST_EXEC)/IRTest
PARTY_T = $(TEST_EXEC)/PartyTest
PO_T = $(TEST_EXEC)/POTest

CPL_SYS_T = $(TEST_EXEC)/CPLSysTests
IR_SYS_T = $(TEST_EXEC)/IRSysTests
PO_SYS_T = $(TEST_EXEC)/POSysTests

# makes all test executables
tests : all test_dir BallotTest CandidateTest CPLTest ElectionTest IRTest PartyTest POTest CPLSysTests IRSysTests POSysTests

test_dir : 
	$(shell mkdir -p gtest_code/executables)

BallotTest : $(BALLOT_O) $(TEST)/BallotTest.cc
	$(CXX) $^ -lgtest -lpthread -o $(BALLOT_T)

CandidateTest : $(CANDIDATE_O) $(BALLOT_O) $(TEST)/CandidateTest.cc
	$(CXX) $(CXXFLAGS) $^ -lgtest -lpthread -o $(CANDIDATE_T)

CPLTest : $(CPL_O) $(CANDIDATE_O) $(PARTY_O) $(ELECTION_O) $(TEST)/CPLTest.cc 
	$(CXX) $^ -lgtest -lpthread -o $(CPL_T)

ElectionTest : $(ELECTION_O) $(START_O) $(TEST)/ElectionTest.cc
	$(CXX) $^ -lgtest -lpthread -o $(ELECTION_T)

IRTest : $(IR_O) $(BALLOT_O) $(CANDIDATE_O) $(ELECTION_O) $(TEST)/IRTest.cc
	$(CXX) $^ -lgtest -lpthread -o $(IR_T)

PartyTest : $(PARTY_O) $(CANDIDATE_O) $(TEST)/PartyTest.cc
	$(CXX) $^ -lgtest -lpthread -o $(PARTY_T)

POTest : $(PO_O) $(BALLOT_O) $(CANDIDATE_O) $(ELECTION_O) $(TEST)/POTest.cc
	$(CXX) $^ -lgtest -lpthread -o $(PO_T)

CPLSysTests : $(CPL_O) $(CANDIDATE_O) $(PARTY_O) $(ELECTION_O) $(TEST)/CPLSysTests.cc
	$(CXX) $^ -lgtest -lpthread -o $(CPL_SYS_T)

IRSysTests : $(IR_O) $(CANDIDATE_O) $(BALLOT_O) $(PARTY_O) $(ELECTION_O) $(TEST)/IRSysTests.cc
	$(CXX) $^ -lgtest -lpthread -o $(IR_SYS_T)

POSysTests : $(PO_O) $(CANDIDATE_O) $(BALLOT_O) $(ELECTION_O) $(TEST)/POSysTests.cc
	$(CXX) $^ -lgtest -lpthread -o $(PO_SYS_T)


########## MISC ##########

OBJECTS = $(patsubst %.cc,$(BIN)/%.o,$(wildcard *.cc))
TEST_OBJECTS = $(patsubst %.cc,$(TEST)/executables/%.o,$(wildcard $(TEST)/*.cc))

clean: 
# rm -f $(OBJECTS)
	rm -f $(TARGET)
	rm -rf $(BIN)
	rm -rf $(TEST_EXEC)
# rm -f $(BALLOT_T) $(CANDIDATE_T) $(CPL_T) $(ELECTION_T) $(IR_T) $(PARTY_T) $(PO_T) $(CPL_SYS_T) $(IR_SYS_T) $(PO_SYS_T)

help:
	@echo 'MAKEFILE OPTIONS: '
	@echo '  make				Compiles election program `start` in current directory'
	@echo '  make <testname>		Compile an individual test.  The testnames include :'
	@echo '				   BallotTest, CandidateTest, CPLTest, ElectionTest, IRTest, PartyTest'
	@echo '				   CPLSysTests, and IRSysTests'
	@echo '  make tests			Compiles testing executables to `/testing/exectuables` directory'
	@echo '  make clean			Removes all compiled files'