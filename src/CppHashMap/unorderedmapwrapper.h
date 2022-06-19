    #pragma once

#include <unordered_map>

#include "ihashmap.h"


template <typename Key, typename Value>
class UnorderedMapWrapper : public IHashMap<Key, Value> {
public:


    virtual void insert(const Key& key, const Value& value) override {
        size_t buckets = map.bucket_count();
        map[key] = value;
        if( map.bucket_count() != buckets ) {
            rehashes++;
        }
    }


    virtual Value& get(const Key& key) override {
        return map.at(key);
    }


    virtual bool remove(const Key& key) override {
        return map.erase(key) == 1;
    }


    virtual size_t count() const override {
        return map.size();
    }

    
    virtual size_t rehashCount() const override {
        return rehashes;
    }

public:

    std::unordered_map<Key, Value> map;

private:

    size_t rehashes = 0;

};