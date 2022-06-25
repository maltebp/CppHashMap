#include <vector>
#include <tuple>

#include "ihashmap.h"

template<class Key, class Value>
class ProbingMap : public IHashMap<Key,Value> {
private:

    struct Entry;

    enum class EntryState : char;

public:

    
    virtual void insert(const Key& key, const Value& value) override {

        // We rehash before we actually know if we will insert (could be improved)
        size_t newLoadFactor = (numEntries + 1) / buckets.size();
        if( newLoadFactor > maxLoadFactor ) {
            resizeAndRehash();
        }

        size_t hash = calculateHash(key);

        insert(key, value, hash);
    }


    virtual Value& get(const Key& key) override {
        size_t hash = calculateHash(key);

        Entry* firstDeletedEntry = nullptr;

        for( size_t i = 0; i < buckets.size(); i++ ) {
            
            size_t index =  (hash + (i * i + i)/2) % buckets.size();

            Entry& entry = buckets[index];

            if( entry.state == EntryState::EMPTY ) break;  

            if( entry.state == EntryState::DELETED ) {
                firstDeletedEntry = &entry;
                continue;
            }                

            if( entry.hash == hash && entry.keyValuePair.first == key ) {
                if( firstDeletedEntry != nullptr ) {
                    *firstDeletedEntry = entry;
                    entry.state = EntryState::DELETED;
                    return firstDeletedEntry->keyValuePair.second;
                }
                else {
                    return entry.keyValuePair.second;
                }
            }
        }

        throw std::out_of_range("Could not find key");
    }


    virtual bool remove(const Key& key) override {
        size_t hash = calculateHash(key);

        for( size_t i = 0; i < buckets.size(); i++ ) {
            
            size_t index =  (hash + (i * i + i)/2) % buckets.size();

            Entry& entry = buckets[index];

            if( entry.state == EntryState::DELETED ) continue;
            if( entry.state == EntryState::EMPTY ) return false;

            if( entry.hash == hash && entry.keyValuePair.first == key ) {
                numEntries--;
                entry.state = EntryState::DELETED;
                return true;
            }
        }

        return false;
    };


    virtual size_t count() const override {
        return numEntries;
    }


    virtual size_t rehashCount() const override {
        return numRehashes;
    }


private:

    
    void resizeAndRehash() {
        std::vector<Entry> oldBuckets = std::move(buckets);
        buckets.clear();
        buckets.resize(oldBuckets.size() * 2);        
        numEntries = 0;
        
        for( const Entry& entry : oldBuckets ) {
            if( entry.state != EntryState::OCCUPIED ) continue; 
            insert(
                entry.keyValuePair.first,
                entry.keyValuePair.second,
                entry.hash
            );
        }

        numRehashes++;
    }

    
    void insert(const Key& key, const Value& value, size_t hash) {
        Entry* firstDeletedEntry = nullptr;
        Entry* unoccupiedEntry = nullptr;

        for( size_t i = 0; i < buckets.size(); i++ ) {
            // As long as size of entries has a size that is a power of 2, we
            // know that we will traverse the entire list, and visit each
            // element exactly once
            
            size_t index = (hash + (i * i + i)/2) % buckets.size();
            
            Entry& entry = buckets[index];

            if( firstDeletedEntry == nullptr && entry.state == EntryState::DELETED) {
                firstDeletedEntry = &entry;
                continue;
            }

            if( entry.state == EntryState::OCCUPIED ) {
                if( hash == entry.hash && entry.keyValuePair.first == key ) {
                    entry.keyValuePair.second = value;
                    return;
                }
                continue;
            }

            unoccupiedEntry = &entry;
            break;
        }

        Entry* entryToInsertInto = firstDeletedEntry == nullptr 
            ?   unoccupiedEntry 
            :   firstDeletedEntry;

        // We rely on the list never being full

        entryToInsertInto->hash = hash;
        entryToInsertInto->keyValuePair.first = key;
        entryToInsertInto->keyValuePair.second = value;
        entryToInsertInto->state = EntryState::OCCUPIED;

        numEntries++;
    }


private:

    std::vector<Entry> buckets{2};

    size_t numEntries = 0;

    uint8_t numRehashes = 0;

    static inline const float maxLoadFactor = 0.75;

private:

    
    enum class EntryState : char {
        EMPTY,    
        OCCUPIED,
        DELETED
    };


    struct Entry {
        std::pair<Key,Value> keyValuePair;
        size_t hash;
        EntryState state;
    };

};