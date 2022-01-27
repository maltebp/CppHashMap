#pragma once


template<typename Key, typename Value>
class IHashMap {
public:

    virtual void insert(const Key& key, const Value& value) = 0;

    virtual Value& get(const Key& key) = 0;

    virtual void remove(const Key& key) = 0;

    virtual size_t count() const = 0;

};