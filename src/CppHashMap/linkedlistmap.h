#pragma once

#include <vector>
#include <tuple>

#include "ihashmap.h"


template<class Key, class Value>
class LinkedListMap : public IHashMap<Key, Value> {
private:

    struct Node;

public:

    class Iterator;

public:


    virtual void insert(const Key& key, const Value& value) override {
        size_t hash = calculateHash(key);
        auto[ node, nodePointer ] = getNode(key, hash);

        if( node != nullptr ) {
            node->keyValuePair.second = value;
            return;
        }

        Node* newNode = new Node();
        newNode->keyValuePair.first = key;
        newNode->keyValuePair.second = value;
        newNode->hash = hash;

        *nodePointer = newNode; 

        numElements++; 

        if ((double)numElements / buckets.size() > 1 ){
            resizeAndRehash();
        }              
    }


    virtual Value& get(const Key& key) override {
        size_t hash = calculateHash(key);
        auto[ node, nodePointer ] = getNode(key, hash);

        if( node == nullptr ) {
            throw std::out_of_range("Could not find key");
        }

        return node->keyValuePair.second;
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


    Iterator begin() const {
        return Iterator(*this, true);
    }


    Iterator end() const {
        return Iterator(*this, false);
    }


private:

    void resizeAndRehash() {
        rehashes++;

        std::vector<Node*> oldBuckets = std::move(buckets);
        
        buckets.clear();
        buckets.resize(oldBuckets.size() * 2);
        numElements = 0;

        for( Node* bucket : oldBuckets ){
            Node* node = bucket;
            while( node != nullptr ){
                insertNode(node);

                Node* nextNode = node->next;
                node->next = nullptr;
                node = nextNode;
            }
        }
    }


    void insertNode(Node* node){
        auto[ _, nodePointer ] = getNode(node->keyValuePair.first, node->hash);
        *nodePointer = node; 
        numElements++;   
    }


    /**
     * @return  Tuple where first element is pointer to the node, and second is
     *          a pointer to the pointer (parent node next or bucket) that
     *          points to the node or to the space into which the node should be
     *          inserted
     */
    std::tuple<Node*, Node**> getNode(const Key& key, size_t hash){
        size_t bucketIndex = hash % buckets.size();

        Node** nodePointer = &(buckets[bucketIndex]);
        Node* currentNode = *nodePointer;
        while( currentNode != nullptr ) {
            if( hash == currentNode->hash && currentNode->keyValuePair.first == key ) {
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
        std::pair<Key,Value> keyValuePair;
        size_t hash = 0;
        Node* next = nullptr;
    };

public:

    class Iterator {
    public:
        
        using iterator_category = std::forward_iterator_tag; // I believe forward iterator is approriate for 
        using difference_type = std::ptrdiff_t; // Not if this is the appropriate type
        using value_type = std::pair<Key,Value>;
        using pointer = std::pair<Key,Value>*;
        using reference = std::pair<Key,Value>&;

    public:

        
        Iterator(const LinkedListMap& map, bool begin)
            :   map(map)
        { 
            if( !begin || map.numElements == 0 ) {
                bucket = map.buckets.size();
                return;
            }

            bucket = 0;
            node = map.buckets[0];
            
            while( node == nullptr ) {
                bucket++;
                node = map.buckets[bucket];
            }
        }


        bool operator ==(const Iterator& other) const {
            if( &map != &other.map ) return false;
            if( node != other.node) return false;
            if( bucket != other.bucket) return false;
            return true;
        }


        bool operator !=(const Iterator& other) const {
            return !(*this == other);
        }

        
        const std::pair<Key,Value>& operator *() const {
            if( node == nullptr ) throw std::out_of_range("Iterator is out of range");
            return node->keyValuePair;
        }

        
        const std::pair<Key,Value>* operator ->() const {
            return &(node->keyValuePair);
        }

        
        Iterator& operator++() {
            node = node->next;
            while( node == nullptr ) {
                bucket++;
                if( bucket >= map.buckets.size() ) break;
                node = map.buckets[bucket];
            }

            return *this;
        }

        Iterator operator++(int) {
            return ++(*this);
        }


    private:

        const LinkedListMap& map;

        const Node* node = nullptr;

        size_t bucket = 0;

    };

};