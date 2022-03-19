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

        *nodePointer = node->next;
        delete node;        
        numElements--;

        return true;
    }

    
    virtual size_t count() const override {
        return numElements;
    }


    virtual size_t rehashCount() const override {
        return rehashes;
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

        Node** nodePointer = &(buckets[bucketIndex]);
        Node* currentNode = *nodePointer;
        while( currentNode != nullptr ) {
            if( hash == currentNode->hash && currentNode->key == key ) {
                return { currentNode, nodePointer };
            }

            nodePointer = &(currentNode->next);
            currentNode = currentNode->next;           
        }

        return { nullptr, nodePointer };
    }


private:

    std::vector<Node*> buckets{1};
    size_t numElements = 0;
    size_t rehashes = 0;

private:

    struct Node {
        Key key;
        Value value;
        size_t hash;
        Node* next = nullptr;
    };

};