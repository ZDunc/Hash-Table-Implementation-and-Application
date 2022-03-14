// Zach Duncan
// Section 04

#ifndef _PASSSERVER_H
#define _PASSSERVER_H

#include "hashtable.h"
#include <string>

namespace hash_namespace{

class PassServer {
public:
  PassServer(std::size_t size = 101); // constructor
  ~PassServer(); // destructor

  bool load(const char* filename); // load password file into HashTable object
  bool addUser(std::pair<std::string, std::string> & kv); // add new user and password
  bool addUser(std::pair<std::string, std::string> && kv); // move version
  bool removeUser(const std::string & k); // remove user and password pair
  bool changePassword(const std::pair<std::string, std::string> & p, const std::string & newpassword);
    // update an existing pair with a new password
  bool find(const std::string & user) const; // check if a user exists
  void dump() const; // print contents of HashTable object to screen
  std::size_t size() const; // return size of HashTable
  bool write_to_file(const char* filename) const; // save user and password to file
private:
  HashTable<std::string, std::string> table;
  std::string encrypt(const std::string& str);

};

} // end of hash_namespace

#endif
