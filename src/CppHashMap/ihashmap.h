#pragma once


template<typename Key, typename Value>
class IHashMap {
public:

    /**
     * @return Returns true if the key did not exist prior to insertion 
     */
    virtual void insert(const Key& key, const Value& value) = 0;

    virtual Value& get(const Key& key) = 0;

    virtual bool remove(const Key& key) = 0;

    virtual size_t count() const = 0;

};


/**
 * @brief	Hash function that is used by IHashMap implementations. No
 *          specializations are defined by default.
 */
template<class Key>
size_t calculateHash(const Key& key);