# Agile:  Voting System
 *Created by Team 7: Justin Lau, Marcus Rana, Liam Mcguigan, and Lucas Olsen*

## Makefile
A makefile is included within the `/src/` directory to make compilation of the various project targets easy.  `make help` lists the various Makefile options:
```
MAKEFILE OPTIONS:
  make                          Compiles election program `start` in current directory
  make <testname>               Compile an individual test.  The testnames include :
                                   BallotTest, CandidateTest, CPLTest, ElectionTest, IRTest, PartyTest
                                   CPLSysTests, and IRSysTests
  make tests                    Compiles testing executables to `/testing/exectuables` directory
  make clean                    Removes all compiled files
``` 

## Voting System Execution
First, take your ballot file(s) and move them to the `/testing/` directory
Navigate to the `/src/` directory in a terminal interface.  Compile the program using `make` and run it with the following command:
```
./start <validBallotFile> <additionalBallotFile> <additionalBallotFile>
```
- Note: You may enter more than one ballot file for a single election.  However, the ballot files must include the same candidates to run correctly.
- `<validBallotFile>` is a command line argument.  It should be the file name of a properly formatted ballot file.  (For example, any file run out of the `/testing/` directory would look as follows: `./start IRNormalTest.csv`.
- `<additionalBallotFile>` is a command line argument.  It should be the file name of a properly formatted ballot file from the same election as the first ballot file entered.

Alternatively, you can enter the name(s) of ballot files after the system has started:
```
./start
Please enter a valid ballot file.
<validBallotFile>
<additionalBallotFile>
<additionalBallotFile>
```
- `<validBallotFile>` is once again the name of a properly formatted ballot file; however, it is entered after starting the system.
- `<additionalBallotFile>` is once again the name of a properly formatted ballot file.  Additional ballot files must be from the same election as the ballot file entered as the argument in `<validBallotFile>`.  
Make sure that you place the ballot file(s) in the "testing" directory
## Testing
In order to test all tests quickly, navigate to the `/src/` directory
Then execute the command:
```chmod +x Do_Tests.sh```
Then execute the command
```./Do_Tests.sh```
The test results should be in ```Tests.log```

If you want to execute a single test, navigate to the src directory and execute the following commands:
```
make tests
gtest_code/executables/./BallotTest
```

Testing materials can be found in the following directories:  
- `/testing/` contains csv test files and a TestingLog.docx file that contains all of the testing logs related to this project.  
- `/gtest_code/` contains the `.cc` files of every automated google test used to complete various unit and system tests.  
- `gtest_code/exectuables` is the location of all compiled test exectuables made from `make tests`

## Cleanup

To remove any compiled objects from the `/Project2/` directory, run `make clean`
