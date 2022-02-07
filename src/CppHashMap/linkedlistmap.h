#pragma once

#include <vector>

#include "ihashmap.h"


template<class Key, class Value>
class LinkedListMap : public IHashMap<Key, Value> {
private:

    struct Node;

public:

    virtual bool insert(const Key& key, const Value& value) override {
        // size_t hash = hasher<Key>(key);
        return false;
    }
    

    virtual Value& get(const Key& key) override {
        // TODO: Implement
        Value val;
        return val; 
    }


    virtual void remove(const Key& key) override {
        // TODO: Implement
    }

    
    virtual size_t count() const override {
        // TODO: Implement
        return 0; 
    }


private:

    void resizeAndRehash() {
        
    }


public:

    std::vector<int> buckets{1};

private:

    struct Node {
        Key key;
        Value value;
        size_t hash;
        Node* next = nullptr;
    };

};