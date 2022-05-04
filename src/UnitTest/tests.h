#pragma once

#include <iostream>

#include "catch.hpp"

#include "data.h"
#include "CppHashMap/ihashmap.h"
#include "CppHashMap/unorderedmapwrapper.h"
#include "CppHashMap/linkedlistmap.h"

using namespace std;

// Required so we may define our own std::hash for an "int type" (I cannot figure
// out how override default).
struct int2 {
    
    int2() : i(0) {}
    
    int2(int i) : i(i) {}

    bool operator==(const int2& other) const {
        return i == other.i;
    }

    bool operator!=(const int2& other) const {
        return i != other.i;
    }

    int i;
};

size_t defaultHashFunction(const int2& key) {
    return (size_t)key.i;
}

function<size_t(const int2& key)> hashFunction = defaultHashFunction;

template<>
size_t calculateHash<int2>(const int2& key) {
    return hashFunction(key);
}

// Redirecting unordered_map hash to hashFunction
template<>
struct hash<int2>
{
    size_t operator()(const int2& i) const {
        return calculateHash(i);
    }
};


void testMap(IHashMap<int2,int>& hashMap) {

    hashFunction = defaultHashFunction;

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


    hashFunction = [](const int2& key) {
        return (size_t)(key.i%10);
    };

    SECTION("Same hash value") {
        for( int i=0; i<1000; i++ ) {
            hashMap.insert(i, i);    
        }

        bool allMatched = true;
        for( int i=0; i<1000; i++ ) {
            int value = hashMap.get(i);
            if( value != i ) {
                allMatched = false;
                break;
            }
        }

        REQUIRE(allMatched);
    }
}


TEST_CASE("unordered_map", "") {    
    UnorderedMapWrapper<int2, int> map;

    SECTION("General test") {
        testMap(map);
    }

    SECTION("Rehashing") {
        // unordered_map defaults to rehash when load_factor (num_buckets /
        // num_elements) exceeds 1, so we can force a rehash this way.

        hashFunction = defaultHashFunction;

        REQUIRE( map.rehashCount() == 0 );
    
        int numBuckets1 = (int)map.map.bucket_count() + 1;
        for( int i=0; i <= numBuckets1; i++ ) {
            map.insert(i, i);    
        }

        REQUIRE( map.rehashCount() == 1 );

        int numBuckets2 = (int)map.map.bucket_count();
        for( int i=numBuckets1+1; i <= numBuckets2; i++ ) {
            map.insert(i, i);    
        }

        REQUIRE( map.rehashCount() == 2 );
    }
}


TEST_CASE("Linked List Map", "") {    
    LinkedListMap<int2, int> map;

    SECTION("General test") {
        testMap(map);
    }

    SECTION("Rehashing") {
        hashFunction = defaultHashFunction;

        REQUIRE(map.rehashCount() == 0);

        map.insert(1, 1);
        map.insert(2, 2);

        REQUIRE(map.rehashCount() == 1);

        map.insert(3, 3);

        REQUIRE(map.rehashCount() == 2);

        map.insert(4, 4);
        map.insert(5, 5);

        REQUIRE(map.rehashCount() == 3);

        for( int i=1; i<=5; i++ ){
            REQUIRE(map.get(i) == i);
        }
    }
}
