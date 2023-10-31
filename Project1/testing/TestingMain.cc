#include "gtest/gtest.h"

// DEFUNCT FILE ????  prob gonna delete soon

// ACTUALLY DONT USE THIS, copy this main into each __test.cc file
// The compiler has to do less linking like that

// To make different tests, I have the compiler link different
// files at compile time
int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}