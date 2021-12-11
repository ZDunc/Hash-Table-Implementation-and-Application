// Zach Duncan
// Section 04

#include <iostream>
#include <utility> // for pairs
#include <cstdio> // for toupper()
#include "passserver.h"
#include "hashtable.h"

using namespace std;
using namespace cop4530;

void Menu();

int main()
{
  cout << "Enter preferred hash table capacity: ";
  int capacity;
  cin >> capacity;

  PassServer ps(capacity); // Create PassServer object of inputted capacity
  // cout << ps.table.prime_below(capacity) < "\n";

  char c; //MENU INPUT
  do{
    Menu(); // Print menu each iteration
    cin >> c; // Take in input
    tolower(c);

    if (c == 'l') // load case
    {
      cout << "Enter password file name to load from: "; // User provides filename
      string name;
      cin >> name;

      ps.load(name.c_str());
    }
    else if (c == 'a') // add case
    {
      cout << "Enter username: "; // User inputs user/password to add
      string user, password;
      cin >> user;
      cout << "Enter password: ";
      cin >> password;

      pair<string, string> entry= make_pair(user, password); // put it in a pair
      if (ps.addUser(entry)) // attempt to add if possible
        cout << "User " << user << " added.\n";
      else
        cout << "Could not add " << user << ".\n";
    }
    else if (c == 'r') // remove case
    {
      cout << "Enter username: "; // User inputs username to remove
      string user;
      cin >> user;

      if (ps.removeUser(user)) // attempt to remove if possible
        cout << "User " << user << " deleted.\n";
      else
        cout << "*****Error: User not found.  Could not delete user.\n";
    }
    else if (c == 'c') // change password case
    {
      cout << "Enter username: "; // User inputs user/password and a new password
      string user, password, newPassword;
      cin >> user;
      cout << "Enter password: ";
      cin >> password;

      cout << "\nEnter new password: ";
      cin >> newPassword;

      pair<string, string> entry= make_pair(user, password); // create a pair
      if (ps.changePassword(entry, newPassword)) // attempt to change password
        cout << "Password changed for user " << user << "\n";
      else
        cout << "*****Error: Could not change user password.\n";
    }
    else if (c == 'f') // find case
    {
      cout << "Enter username: "; // User inputs username to find in PassServer
      string user;
      cin >> user;

      if (ps.find(user)) // Call PassServer's find function
        cout << "User '" << user << "' found.\n";
      else
        cout << "User '" << user << "' not found.\n";
    }
    else if (c == 'd') // dump case
      ps.dump();
    else if (c == 's') // size case
      cout << "Size of hastable: " << ps.size() << "\n";
    else if (c == 'w') // write to file case
    {
      cout << "Enter password file name to write to: "; // User provides filename
      string filename;
      cin >> filename;

      if (! ps.write_to_file(filename.c_str()))
        cout << "*****Error. Could not write " << filename << "\n";
    }
    else if (c == 'x') // exit program
      break;
    else
      continue; // Invalid entry, continue

  } while (tolower(c) != 'x');

  return 0;
}

void Menu()
{
  cout << "\n\n";
  cout << "l - Load From File" << endl;
  cout << "a - Add User" << endl;
  cout << "r - Remove User" << endl;
  cout << "c - Change User Password" << endl;
  cout << "f - Find User" << endl;
  cout << "d - Dump HashTable" << endl;
  cout << "s - HashTable Size" << endl;
  cout << "w - Write to Password File" << endl;
  cout << "x - Exit program" << endl;
  cout << "\nEnter choice : ";
}
