# Hash Map

## **The idea:**

A **key** should produce an **integer hash**, which can be as an "partial" **index** into some internal memory.

## **The core problem:**

If use a simple array, and we insert an element with hash `n` then we have to allocate memory for all elements from `0` to `n`. This will leave much unused memory. 

In general, implementing a hash map is a **space-time** trade-off.

## Solving the problem

In general, we reduce the number of "slots" and map several hashes (indices) to the same slot (i.e. by using modulo).

- **Buckets:** The array of slots that we have available, which a hash can map to in "constant" time.

- **Collision:** When two hashes map to the same bucket

- **Collision resolution:** How we map from a **bucket** to the actual entries.

- **Load factor:** How many elements the map contains compared to the number of buckets. We want a load factor close to 1.
  
  **Clustering:** How much the elements are clustered together (hash to the same bucket). We want to avoid this as much as possible.

#### Collision Resolution

In general, we use two groups of methods:

- **Seperate Chaining**
  Each bucket is (or maps) some seperate data structure. Often this is a *linked list* or a *tree*.

- **Open Addressing**
  We compute some other index within our "bucket" structure, and test that - recursively.
  This is often called **probing**, and we often the following variants:
  
  - **Linear probing:** just use next index
  
  - **Quadratic probing:** increase added value by some quadratic function.
  
  - **Double hashing**: use some an internal hash function to compute new index.
  
  Linear probing has has higher **clustering** than quadratic probing, but also better spatial locality.

#### Rehashing

The number of buckets is often dynamically increased as the load factor increases. Because the final slot for an entry is typically based on the number of buckets, this  typically requires the map to be **rehashed**

## Other notes:

- In general hash algorithms have **poor spatial locality** (e.g. with a linked list implementation). But because the map is still very much sporadic *I suspect* that this is only a significant problem when we iterate over the map.

- Open-addressing typically have lower **load factor**

- Open-addressing deletion of many elements can be optimized (see Wiki, e.g. *lazy deletion*)

- In C# and Java (and probably others), changing the values of a used key element will break the search for that particular key (because the hash is no longer valid).

- Typically, you store the computed hash with the entry, to increase performance as recomputing the hash and performing `equals` operations can be expensive.

## Language implementations

##### C++: `std::unordered_map`

Good source: [c++ - How std::unordered_map is implemented - Stack Overflow](https://stackoverflow.com/questions/31112852/how-stdunordered-map-is-implemented) 

- The STD *effectively* mandates *seperate chaining* through its requirements (it must not rehash unless grown beyond the **max load factor**).
- The **load factor** must not exceed 1 per default
- Won't rehash unless load factor is exceeded, or explicitly called
- **Dynamic allocation on *every* insert**, because it has to create linked list nodes
- The use of buckets of linked-list makes it cache inefficient (elements are spread out all over the place)
- However, it stabilize element pointers, which can be nice. But in an implementation which doesn't stabilize, the user could easily achieve it by wrapping elements in heap allocated objects, so it may not be a good trade-off to force it.

##### C# .NET

Good source: [.NET Dictionary, impressively fast but how does it work? - Stack Overflow](https://stackoverflow.com/questions/5379871/net-dictionary-impressively-fast-but-how-does-it-work/54137050#54137050)

- **Seperate chaining** variant using an array implementation of a singly linked list

- Chaining is built by wrapping entries in `Entry` structs that are stored in an array. Each `Entry` contains an index to the next `Entry`, and the `bucket` array are indices to the first element in each chain.

- `Entry`-array and bucket array are resized when the `Entry` array is full (or the *freelist* is empty). **Note:** not entirely sure this is true, because I think you'll risk tsoo high load factor?

- Array size is computed using *primes* (see [this SO post](https://cs.stackexchange.com/questions/11029/why-is-it-best-to-use-a-prime-number-as-a-mod-in-a-hashing-function/64191#64191) on why that is effective)

- Holes are fixed by having a *freelist* (index which build a designated chain of free entries)

## Nice sources

- [CppCon 2014: Chandler Carruth &quot;Efficiency with Algorithms, Performance with Data Structures&quot;](https://www.youtube.com/watch?v=fHNmRkzxHWs&ab_channel=CppCon)
  Should be quite interesting as he talks about the fact that C++ maps are slow

- [Super high performance C/C++ hash map (table, dictionary) - Stack Overflow](https://stackoverflow.com/questions/3300525/super-high-performance-c-c-hash-map-table-dictionary)
  Answer that talks about different hash table implementations (e.g. [Google SparseHash](https://github.com/sparsehash/sparsehash)).

## Unanswered questions

- C++ mentions something about iterators remaining valid

- What *is* the advantage of having an unordered_map using spatial locality exactly?
  
  - I suspect that is only for some what rare use cases, as the accessing often is sporadic and typically not done in sequence. Also, the nature of a good hashing algorithm would cause the elements to be significantly distributed across the allocated memory.