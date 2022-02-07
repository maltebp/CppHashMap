#include <iostream>
#include <string>

#include "CppHashMap/linkedlistmap.h"

using namespace std;


template<>
size_t calculateHash<int>(const int& key) {
    return (size_t)(key%6);
}


int main(int argc, char** argv) {

    LinkedListMap<int,std::string> map;

    bool existed = map.insert(10, "Hello world");
    existed = map.insert(10, "Goodbye hello");
}