// Zach Duncan
// Section 04

#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include <vector>
#include <list>
#include <algorithm> // for find() as used in class PowerPoint
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility> // for pair
#include <cstddef> // for size_t

namespace cop4530{

template <typename K, typename V>
class HashTable {
  public:
    HashTable(std::size_t size = 101); // constructor
    ~HashTable(); // destructor. Delete all elements in hash table.
    bool contains(const K & k) const; // check if key k is in the hash table.
    bool match(const std::pair<K, V> &kv) const; // check if key-value pair is in the hash table.
    bool insert(const std::pair<K, V> & kv); // add  the key-value pair kv into the hash table.
    bool insert (std::pair<K,  V> && kv); // move version of insert.
    bool remove(const K & k); // delete the key k and the corresponding value if it is in the hash table.
      // Return true if k is deleted, return false otherwise (i.e., if key k is not in the hash table).
    void clear(); //delete all elements in the hash table
    bool load(const char *filename);
      // load the content of the file with name filename into the hash table.
    void dump() const; // display all entries in the hash table.
    std::size_t size() const; // return the size of the hash table (i.e. number of entries stored).
    bool write_to_file(const char *filename) const;
      // write all elements in the hash table into a file with name filename.
    void init(std::size_t size); // set a HashTable to a spectific size

    // max_prime is used by the helpful functions provided
    // to you.
    static const unsigned int max_prime = 1301081;
    // the default_capacity is used if the initial capacity
    // of the underlying vector of the hash table is zero.
    static const unsigned int default_capacity = 11;

    //Private helper functions
  private:
    void makeEmpty(); // delete all elements in the hash table.
      // The public interface clear() will call this function.
    void rehash(); // Rehash function. Called when the number of elements
      // in the hash table is greater than the size of the vector.
    std::size_t myhash(const K &k) const; //  return the index of the vector entry
      // where k should be stored.

    unsigned long prime_below (unsigned long);
    void setPrimes(std::vector<unsigned long>&);
      // functions to determine the proper prime numbers used in setting up the vector size.

    //PRIVATE DATA
    std::vector< std::list< std::pair< K,V > > > theList;
    std::size_t currentSize;
};

#include "hashtable.hpp"

} //end of namespace cop4530


#endif
