// Zach Duncan
// Section 04

#ifndef _PASSSERVER_CPP
#define _PASSSERVER_CPP

#include "passserver.h"
#include "crypt.h"
#include <string>
#include <utility> // for pair
#include <cstddef> // for size_t

using namespace cop4530;


//-------------------------PUBLIC CLASS FUNCTINONS------------------------------

// constructor, create a hash table of the specified size. You just need to pass this
// size parameter to the constructor of the HashTable. Therefore, the real hash table
// size could be different from the parameter size (because prime_below() will be called
// in the constructor of the HashTable).
PassServer::PassServer(std::size_t size)
{ table.init(size); }

// destructor. You need to decide what you should do based on your design of PassServer
// (how you develop the adaptor class based on the adaptee HashTable). In essence, we do
// not want to have memory leak.
PassServer::~PassServer()
{ table.clear(); }

// load a password file into the HashTable object. Each line contains a pair of username
// and encrypted password.
bool PassServer::load(const char* filename)
{ return table.load(filename); }

// add a new username and password.  The password passed in is in plaintext, it should be
// encrypted before insertion.
bool PassServer::addUser(std::pair<std::string, std::string> & kv)
{
  if (! table.contains(kv.first))
  {
    std::pair<std::string, std::string> newkv = kv;
    newkv.second = encrypt(newkv.second);
    return table.insert(newkv);
  }
  else
    return false;
}

// move version of addUser
bool PassServer::addUser(std::pair<std::string, std::string> && kv)
{
  if (! table.contains(kv.first))
  {
    std::pair<std::string, std::string> newkv = std::move(kv);
    newkv.second = encrypt(newkv.second);
    return table.insert(newkv);
  }
  else
    return false;
}

// delete an existing user with username k
bool PassServer::removeUser(const std::string & k)
{ return table.remove(k); }

// change an existing user's password. Note that both passwords passed in are in plaintext.
// They should be encrypted before you interact with the hash table. If the user is not in
// the hash table, return false. If p.second does not match the current password, return
// false. Also return false if the new password and the old password are the same (i.e.,
// we cannot update the password).
bool PassServer::changePassword(const std::pair<std::string, std::string> & p, const std::string& newpassword)
{
  if (! table.contains(p.first)) // Cannot change a password for a key not in hashtable
    return false;
  else
  {
    std::pair<std::string, std::string> thisPair = p; // make a new pair, store p in it
    // cout << thisPair.first << " " << thisPair.second;
    thisPair.second = encrypt(thisPair.second); // encrypt the password pair p
    if (! table.match(thisPair)) // if inputted pair was not found in hashtable
      return false; // do not change password, they must enter a valid pair
    else
    {
      std::string password = encrypt(newpassword); // encrypt the new password user wants to change to
      thisPair.second = password;
      return table.insert(thisPair); // update pair in hashtable through insert function
    }
  }
}

// check if a user exists (if user is in the hash table).
bool PassServer::find(const std::string & user) const
{ return table.contains(user); }

// show the structure and contents of the HashTable object to the screen.
// Same format as the dump() function in the HashTable class template.
void PassServer::dump() const
{ table.dump(); }

// return the size of the HashTable (the number of username/password pairs in the table).
std::size_t PassServer::size() const
{ return table.size(); }

// save the username and password combination into a file. Same format as the write_to_file()
// function in the HashTable class template.
bool PassServer::write_to_file(const char* filename) const
{ return table.write_to_file(filename); }

//-------------------------PRIVATE CLASS FUNCTINONS------------------------------
// encrypt the parameter str and return the encrypted string.
std::string PassServer::encrypt(const std::string & str) // utilize "crypt.h"
{
  char salt[] = "$1$########"; // char array of salt provided
  std::string temp = crypt(str.c_str(), salt); // put given str and salt through crypt function, store in string
  temp = temp.substr(12, 22); // then remove the first 12 characters
  return temp; // return this string
}

#endif
