#include <vector>
#include <tuple>

#include "ihashmap.h"

template<class Key, class Value>
class ProbingMap : public IHashMap {
private:

    struct Entry;

    enum class EntryState;

public:

    void insert(const Key& key, const Value& value) {
        size_t hash = calculateHash(key);

        size_t startingIndex = hash % entries.size();
        size_t index = startingIndex;

        Entry* firstDeletedEntry = nullptr;

        // TODO: Check if we need to rehash

        for( size_t i = 0; i < entries.size(); i++ ) {
            // As long as size of entries has a size that is a power of 2, we
            // know that we will traverse the entire list, and visit each
            // element exactly once
            
            index = (hash + (i * i + i)/2) % entries.size();
            
            Entry& entry = entries[index];

            if( firstDeletedEntry == nullptr && entry.state == EntryState::DELETED) {
                firstDeletedEntry = &entry;
                continue;
            }

            if( entry.state == EntryState::OCCUPIED ) {
                if( entry.keyValuePair.first == key ) {
                    entry.keyValuePair.second = value;
                    return;
                }
                continue;
            }

            Entry* entryToInsertInto = firstDeletedEntry == nullptr ? firstDeletedEntry : &entry;

            entryToInsertInto->hash = hash;
            entryToInsertInto->keyValuePair.first = key;
            entryToInsertInto->keyValuePair.second = value;
            entryToInsertInto->state = EntryState::OCCUPIED;
        }
    }

private:

    std::vector<Entry> entries{1};

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