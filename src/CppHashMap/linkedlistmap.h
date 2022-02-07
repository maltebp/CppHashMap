#pragma once

#include <vector>
#include <tuple>

#include "ihashmap.h"


template<class Key, class Value>
class LinkedListMap : public IHashMap<Key, Value> {
private:

    struct Node;

public:

    virtual bool insert(const Key& key, const Value& value) override {
        size_t hash = calculateHash(key);
        auto[ existingNode, previousNode ] = getNode(key, hash);

        if( existingNode != nullptr ) {
            existingNode->value = value;
            return false;
        }

        Node* node = new Node();
        node->key = key;
        node->value = value;
        node->hash = hash;

        if( previousNode == nullptr ) {
            size_t bucketIndex = hash % buckets.size();
            buckets[bucketIndex] = node;
        }
        else {
            previousNode->next = node;                
        }

        return true;
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


    /**
     * @return  Tuple where first element is pointer to the node, and second is
     *          a pointer to the node pointing to that node.
     */
    std::tuple<Node*, Node*> getNode(const Key& key, size_t hash) {
        size_t bucketIndex = hash % buckets.size();

        Node* previousNode = nullptr;
        Node* currentNode = buckets[bucketIndex];
        while( currentNode != nullptr ) {
            if( hash == currentNode->hash && currentNode->key == key ) {
                return { currentNode, previousNode };
            }

            previousNode = currentNode;
            currentNode = currentNode->next;           
        }

        return { currentNode, previousNode };
    }


public:

    std::vector<Node*> buckets{9}; // TODO: Change initial size to 1

private:

    struct Node {
        Key key;
        Value value;
        size_t hash;
        Node* next = nullptr;
    };

};