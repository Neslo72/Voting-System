# Project Waterfall:  Voting System
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
First, take your ballot file and move it to the testing directory
Navigate to the `/src/` directory in a terminal interface.  Compile the program using `make` and run it with the following command:
```
./start <validBallotFile>
```
- `<validBallotFile>` is a command line argument.  It should be the file location / name of a properly formatted ballot file.  (For example, any file run out of the `/testing/` directory would look as follows: `./start testing/IRNormalTest.csv`.  Please note that a leading `/` will cause the program to NOT work, i.e. `./start /testing/IRNormalTest.csv`)

Alternatively, you can enter the name of a ballot file after the system has started:
```
./start
Please enter a valid ballot file.
<validBallotFile>
```
- `<validBallotFile>` is once again the location / name of a properly formatted ballot file; however, it is entered after starting the system.
Make sure that you place the ballot file in the "testing" directory, and when specifying
## Testing
In order to test all tests quickly, navigate to the src directory
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

To remove any compiled objects from the `/Project1/` directory, run `make clean`
