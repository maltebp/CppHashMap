#pragma once


template<typename Key, typename Value>
class HashMap {

    struct Entry;

public: 



private:

    Entry** buckets = nullptr;
    
    size_t numBuckets = 0;
    
    float maxLoadFactor = 1f;

private:

    struct Entry {
        Entry*  next = nullptr;
        Key key;
        Value value;
    };

};