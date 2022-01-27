#pragma once

#include <unordered_map>

#include "ihashmap.h"


template <typename Key, typename Value>
class UnorderedMapWrapper : public IHashMap<Key, Value> {
public:


    virtual void insert(const Key& key, const Value& value) override {
        map[key] = value;
    }


    virtual Value& get(const Key& key) override {
        return map.at(key);
    }


    virtual void remove(const Key& key) override {
        map.erase(key);
    }


    virtual size_t count() const override {
        return map.size();
    }


private:

    std::unordered_map<Key, Value> map;

};