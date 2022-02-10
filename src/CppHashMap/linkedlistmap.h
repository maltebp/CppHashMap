#pragma once

#include <vector>
#include <tuple>

#include "ihashmap.h"


template<class Key, class Value>
class LinkedListMap : public IHashMap<Key, Value> {
private:

    struct Node;

public:

    virtual void insert(const Key& key, const Value& value) override {
        size_t hash = calculateHash(key);
        auto[ node, nodePointer ] = getNode(key, hash);

        if( node != nullptr ) {
            node->value = value;
            return;
        }

        Node* newNode = new Node();
        newNode->key = key;
        newNode->value = value;
        newNode->hash = hash;

        *nodePointer = newNode; 

        numElements++;               
    }
    

    virtual Value& get(const Key& key) override {
        size_t hash = calculateHash(key);
        auto[ node, nodePointer ] = getNode(key, hash);

        if( node == nullptr ) {
            throw std::out_of_range("Could not find key");
        }

        return node->value;
    }


    virtual bool remove(const Key& key) override {
        size_t hash = calculateHash(key);
        auto[ node, nodePointer ] = getNode(key, hash);

        if( node == nullptr ) return false;

        *nodePointer = nullptr;
        delete node;        
        numElements--;

        return true;
    }

    
    virtual size_t count() const override {
        return numElements;
    }


private:

    void resizeAndRehash() {
        
    }


    /**
     * @return  Tuple where first element is pointer to the node, and second is
     *          a pointer to the pointer (parent node next or bucket) that
     *          points to the node or to the space into which the node should be
     *          inserted
     */
    std::tuple<Node*, Node**> getNode(const Key& key, size_t hash) {
        size_t bucketIndex = hash % buckets.size();

        Node** previousNode = &(buckets[bucketIndex]);
        Node* currentNode = buckets[bucketIndex];
        while( currentNode != nullptr ) {
            if( hash == currentNode->hash && currentNode->key == key ) {
                return { currentNode, previousNode };
            }

            previousNode = &(currentNode->next);
            currentNode = currentNode->next;           
        }

        return { nullptr, previousNode };
    }


public:

    std::vector<Node*> buckets{9}; // TODO: Change initial size to 1
    size_t numElements = 0;

private:

    struct Node {
        Key key;
        Value value;
        size_t hash;
        Node* next = nullptr;
    };

};