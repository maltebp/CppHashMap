#include <iostream>

#include "CppHashMap/linkedlistmap.h"

using namespace std;


template<>
size_t calculateHash<int>(const int& key) {
    return (size_t)(key%6);
}


int main(int argc, char** argv) {

    LinkedListMap<int,int> map;

    size_t hash = calculateHash<int>(10);

    cout << hash << std::endl;

}