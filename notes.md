# Hash Map

## Notes

 - If the hash is simply an index into an array that can hold all valid hash values, that array will almost always have significant gaps of unused memory

 - To reduce the problem with gaps, we use an array of _buckets/bins_. Each hash key is computed to a specific bucket index, e.g. `hash key % num buckets`. The bucket itself is some dynamic data structure, e.g. an linked list or tree structure (as mentioned in [this SO post](https://stackoverflow.com/questions/37959941/what-exactly-is-bucket-in-hashmap)). Using a linked list is what is the method called [separate chaining](https://en.wikipedia.org/wiki/Hash_table#Separate_chaining).

 - The C++ Standard of unordered_map ((SO post)[https://stackoverflow.com/questions/31112852/how-stdunordered-map-is-implemented]):
    - _Effectively_ mandates _seperate chaining_
    - The _load factor_ (`entries / buckets`) must not exceed 1 per default
    - Won't rehash unless load factor is exceeded


## Unanswered questions

 - Should the number of buckets be dynamic throughout the maps lifetime?

 - How many buckets should there be?
 
 - What is the difference between "open" and "closed" hash map implementations?

 - C++ mentions something about iterators remaining valid

 - Does C++ implementation use linked list? Sounds slow due dynamic allocations and cache coherence.