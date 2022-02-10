#pragma once

#include <iostream>

#include "catch.hpp"

#include "data.h"
#include "CppHashMap/ihashmap.h"
#include "CppHashMap/unorderedmapwrapper.h"
#include "CppHashMap/linkedlistmap.h"

using namespace std;

// TODO: Define custom hashing function for unit tests 
// TODO: Create test for inserting with different keys that hash to the same value

void testMap(IHashMap<int, int>& hashMap) {

    SECTION("Insert and get (Basic)") {
        hashMap.insert(1,1);
        hashMap.insert(500,2);
        hashMap.insert(5784387, 3);
        REQUIRE( hashMap.get(1) == 1 );
        REQUIRE( hashMap.get(500) == 2 );
        REQUIRE( hashMap.get(5784387) == 3 );
    }

    SECTION("Count (Basic)") {
        REQUIRE( hashMap.count() == 0 );

        for( int i=0; i<10000; i++ ) {
            hashMap.insert(i, i);
        }
        REQUIRE( hashMap.count() == 10000 );
    }

    SECTION("Remove (Basic)") {
        hashMap.insert(1,2);
        hashMap.insert(2,3);
        REQUIRE( hashMap.get(1) == 2 );
        REQUIRE( hashMap.get(2) == 3 );
        REQUIRE( hashMap.count() == 2 );

        hashMap.remove(1);
        REQUIRE( hashMap.get(2) == 3 );
        REQUIRE( hashMap.count() == 1 );

        hashMap.remove(2);
        REQUIRE( hashMap.count() == 0 );
    }

    SECTION("Override (Basic)") {
        hashMap.insert(1, 2);
        hashMap.insert(2, 3);
        REQUIRE( hashMap.get(1) == 2 );
        REQUIRE( hashMap.get(2) == 3 );

        hashMap.insert(1, 3);
        REQUIRE( hashMap.get(1) == 3 );
        REQUIRE( hashMap.get(2) == 3 );
    }

    SECTION("Insert (Advanced)") {
        for( int i : DATA ) {
            hashMap.insert(i, i);
        }

        bool allMatched = true;

        for( int i : DATA ) {
            int value = hashMap.get(i);
            if( value != i ) {
                allMatched = false;
                break;
            }
        }

        REQUIRE(allMatched);
    }
    

    SECTION("Re-insert (Advanced)") {
        for( int i : DATA ) {
            hashMap.insert(i, i);
        }
        REQUIRE( hashMap.count() == DATA.size() );

        bool allMatched = true;
        for( int i : DATA ) {
            int value = hashMap.get(i);
            if( value != i ) {
                allMatched = false;
                break;
            }
        }
        REQUIRE( allMatched );

        for( int i : DATA ) {
            hashMap.remove(i);
        }

        REQUIRE( hashMap.count() == 0 );

        for( int i : DATA ) {
            hashMap.insert(i, i);
        }

        allMatched = true;
        for( int i : DATA ) {
            int value = hashMap.get(i);
            if( value != i ) {
                allMatched = false;
                break;
            }
        }

        REQUIRE( allMatched );
    }

    
    SECTION("Overwrite (Advanced)") {
        for( int i : DATA ) {
            hashMap.insert(i, i);
        }
        REQUIRE( hashMap.count() == DATA.size() );

        bool allMatched = true;
        for( int i : DATA ) {
            int value = hashMap.get(i);
            if( value != i ) {
                allMatched = false;
                break;
            }
        }
        REQUIRE( allMatched );

        for( int i : DATA ) {
            hashMap.insert(i, 1337);
        }

        allMatched = true;
        for( int i : DATA ) {
            int value = hashMap.get(i);
            if( value != 1337 ) {
                allMatched = false;
                break;
            }
        }
        REQUIRE( allMatched );
    }
}


TEST_CASE("unordered_map", "") {    
    UnorderedMapWrapper<int, int> map;
    testMap(map);
}


TEST_CASE("Linked List Map", "") {    
    LinkedListMap<int, int> map;
    testMap(map);
}
