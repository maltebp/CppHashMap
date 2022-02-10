#include <iostream>
#include <string>

#include "CppHashMap/linkedlistmap.h"

using namespace std;


template<>
size_t calculateHash<int>(const int& key) {
    return (size_t)(key%6);
}


int main(int argc, char** argv) {

    LinkedListMap<int,string> map;

    map.insert(10, "Hello world");

    cout << map.get(10) << endl;

    map.insert(15, "Goodbye hello");
    cout << map.get(15) << endl;

    map.insert(10, "World, hello!");
    cout << map.get(10) << endl;

    try {
        cout << map.get(20) << endl;
    } catch(std::out_of_range e) {
        cout << 20 << " does not exist" << endl;
    }
    
}