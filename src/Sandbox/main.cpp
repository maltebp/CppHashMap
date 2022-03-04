#include <iostream>
#include <string>

#include "CppHashMap/linkedlistmap.h"

using namespace std;


template<>
size_t calculateHash<int>(const int& key) {
    return (size_t)key;
}


int main(int argc, char** argv) {

    LinkedListMap<int,string> map;

    cout << "Elements: " << map.count() << endl;

    map.insert(10, "Hello world");

    cout << map.get(10) << endl;

    map.insert(15, "Goodbye hello");
    cout << map.get(15) << endl;

    cout << "Elements: " << map.count() << endl;

    map.insert(10, "World, hello!");
    cout << map.get(10) << endl;

    cout << "12 removed: " << (map.remove(12) ? "true" : "false") << endl;
    cout << "15 removed: " << (map.remove(15) ? "true" : "false") << endl;

    try {
        cout << map.get(15) << endl;
    } catch(std::out_of_range e) {
        cout << 15 << " does not exist" << endl;
    }

    cout << "Elements: " << map.count() << endl;
    
}