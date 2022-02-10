#include <vector>

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "tests.h"

using namespace std;


template<>
size_t calculateHash<int>(const int& key) {
    return (size_t)(6);
}


int main(int argc, char** argv) {

    vector<const char*> args;
    for(int i=0; i<argc; i++) {
        args.push_back(argv[i]);
    }
	
	// Test arguments: (i.e. tag on test cases to run)
	//args.push_back("-b"); // Break when assertion failed

    return Catch::Session().run((int)args.size(), args.data());
}