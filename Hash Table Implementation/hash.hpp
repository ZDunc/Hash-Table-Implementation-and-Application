// Zach Duncan
// Section 04

using std::cout;
using std::cin;

////////////////////CLASS HashTable<K, V> PUBLIC DEFINITIONS/////////////////////

// constructor. Create a hash table, where the size of the vector is set to prime_below(size)
// (where size is default  to 101), where prime_below() is a private member function of the
// HashTable and provided to you.
template <typename K, typename V>
HashTable<K, V>::HashTable(std::size_t size)
{
  theList.resize(prime_below(size)); // call vector's resize function with given size
  currentSize = 0; // set size to 0 (current number of entries in hash)
}

// destructor. Delete all elements in hash table.
template <typename K, typename V>
HashTable<K, V>::~HashTable()
{ makeEmpty(); }

// check if key k is in the hash table.
template <typename K, typename V>
bool HashTable<K, V>::contains(const K& k) const
{
  auto& whichList = theList[myhash(k)];
  for (auto& thisList : whichList) // iterate through hashtable
  {
    if (thisList.first == k) // if key is found, success
      return true;
  }
  return false; // else, failure

  // auto& whichList = theList[myhash(k)]; // derived from code in class PowerPoint
	// return find(begin(whichList), end(whichList), k) != end(whichList);
}

// check if key-value pair is in the hash table.
template <typename K, typename V>
bool HashTable<K, V>::match(const std::pair<K, V>& kv) const
{
  auto& whichList = theList[myhash(kv.first)];
  for (auto& thisList : whichList) // iterate through hashtable
  {
    if ((thisList.first == kv.first) && (thisList.second == kv.second)) // if match, success
      return true;
  }
  return false; // else, failure
}

// add  the key-value pair kv into the hash table.
// Don't add if kv is already in the hash table. If the key is in the hash table but with
// a different value, the value should be updated to the new one with kv. Return true
// if kv is inserted or the value is updated; return false otherwise (i.e., if kv is in
// the hash table).
template <typename K, typename V>
bool HashTable<K, V>::insert(const std::pair<K, V>& kv)
{
	if (match(kv)) // if key AND value are already in hashtable
    return false;

  auto& whichList = theList[myhash(kv.first)];
  if (contains(kv.first)) // if just key with wrong password is in hashtable
  {
    for (auto& thisList : whichList) // iterate hashtable
    {
      if (thisList.first == kv.first) // find that key
        thisList.second = kv.second; // Update password
    }
  }
  else  // If NEITHER key nor value are in hash table, add new pair
    whichList.push_back(kv);   // derived from code in class PowerPoint

	if (++currentSize > theList.size())
		rehash();
	return true;
}

// move version of insert
template <typename K, typename V>
bool HashTable<K, V>::insert(std::pair<K, V>&& kv)
{
  if (match(kv)) // if key AND value are already in hashtable
    return false;

  auto& whichList = theList[myhash(kv.first)];
  if (contains(kv.first)) // if just key with wrong password is in hashtable
  {
    for (auto& thisList : whichList) // iterate hashtable
    {
      if (thisList.first == kv.first) // find that key
        std::swap(thisList.second, kv.second); // Update password, MOVE VERSION
    }
  }
  else
    whichList.push_back(std::move(kv)); // // derived from code in class PowerPoint, MOVE VERSION

  if (++currentSize > theList.size())
		rehash();
	return true;
}

// delete the key k and the corresponding value if it is in the hash table. Return true
// if k is deleted, return false otherwise (i.e., if key k is not in the hash table).
template <typename K, typename V>
bool HashTable<K, V>::remove(const K& k)
{
  if (! contains(k)) // cannot remove a key not in the hashtable
    return false;

  auto& whichList = theList[myhash(k)];
  int counter = 0;
  for (auto& thisList : whichList) // iterate through hashtable with a counter
  {
    if (thisList.first == k) // if key is found
    {
      // if (thisList.first == end(whichList))
    		// return false;
      auto itr = whichList.begin(); // make iterator to beginning of list
      for (int i = 0; i < counter; i++) // then advance iterator to counter
        itr++;

    	itr = whichList.erase(itr); // erase pair at that iterator
    	--currentSize; // and decrement size
    	return true;
    }
    counter++; // increment counter
  }
  return false; // should not get to this code, but return false if we do

  // auto& whichList = theList[myhash(k)]; // derived from code in class PowerPoint
	// auto itr = find(begin(whichList), end(whichList), k);

	// if (itr == end(whichList))
	//	return false;
	// whichList.erase(itr);
	// --currentSize;
	// return true;
}

// delete all elements in the hash table
template <typename K, typename V>
void HashTable<K, V>::clear()
{ makeEmpty(); }

// load the content of the file with name filename into the hash table. In the file,
// each line contains a single pair of key and value, separated by a white space.
template <typename K, typename V>
bool HashTable<K, V>::load(const char* filename)
{
  std::ifstream infile; // create ifstream object
  infile.open(filename); // open filename with ifstream object infile

  if (! infile) // If file does not exist or cannot be opened
  {
    cout << "Cannot open file.\n";
    return false;
  }

  std::string line, s; // string s for each word (each line is [key value])
  while (std::getline(infile, line)) // read in the first line
  {
    std::istringstream iss(line); // create string stream for each line in file

    std::pair<K, V> toInsert; // create pair
    int counter = 0; // counter used to make sure there are two words in each line
    while (iss >> s) // FOR EACH LINE
    {
      if (counter == 0) // key
        toInsert.first = s;
      else if (counter == 1) // value
        toInsert.second = s;
      else // error
      {
        cout << "Error occurred in write_to_file(). More than 2 words in a line.\n";
        return false; // There should only be two words in each line!! The key and the value
      }
      counter++; // increment counter
    } // end of while loop for individual line

    insert(toInsert); // insert the new pair and reiterate if there is another line
  }

  infile.close(); // close file
  return true;
}

// display all entries in the hash table. If an entry contains multiple key-value pairs,
// separate them by a semicolon character (:) (see the provided executable for the exact
// output format).
template <typename K, typename V>
void HashTable<K, V>::dump() const
{
   for (int i = 0; i < theList.size(); i++) // iterate through theList vector
   {
     cout << "v[" << i << "]: "; // print index
     auto itr = theList[i].begin(); // make an itr to begin of linked list at theList[i]
     for (auto& thisList : theList[i]) // iterate through the linked list
     {
       if (itr != theList[i].begin()) // if there is more than one node in linked list
         cout << " : "; // print this colon to separate user/password pairs in this array index
       cout << thisList.first << " " << thisList.second; // print out user/password pair
       itr++; // advance iterator
     }
     cout << "\n";
   }
}

// return the size of the hash table (i.e. number of entries stored)
template <typename K, typename V>
std::size_t HashTable<K, V>::size() const
{ return currentSize; }

// write all elements in the hash table into a file with name filename. Similar to the
// file format in the load function, each line contains a pair of key-value pair, separated
// by a white space.
template <typename K, typename V>
bool HashTable<K, V>::write_to_file(const char* filename) const
{
  std::ofstream outfile; // create ofstream object
  outfile.open(filename); // use it to open a file of given filename

  if (! outfile) // if we cannot open that
  {
    cout << "Cannot create file \"" << filename << ".\"\n";
    return false;
  }

  int counter = static_cast<int>(size()); // put size of hashtable in a counter variable
  for(auto & thisList : theList) // increment through hash
  {
    for(auto & keyOrPair : thisList) // increment through linked lists
    {
      outfile << keyOrPair.first << " " << keyOrPair.second; // output to file
      if (counter > 1) // use counter to avoid putting newline after last entry
        outfile << "\n";
      counter--;
    }
  }
  outfile.close(); // close file
  return true;

}

// set a HashTable to a spectific size, used in PassServer class
template <typename K, typename V>
void HashTable<K, V>::init(std::size_t size)
{
  makeEmpty();
  theList.resize(prime_below(size));
}

////////////////////CLASS HashTable<K, V> PRIVATE DEFINITIONS/////////////////////

// delete all elements in the hash table. The public interface clear() will call this
// function.
template <typename K, typename V>
void HashTable<K, V>::makeEmpty()
{
  if (currentSize == 0)
    return;
  else // Need to clear EACH list inside the vector to clear its contents
  {    // derived from code in class PowerPoint
    for (auto& thisList : theList)
        thisList.clear(); // call clear function from STL's list container class
    currentSize = 0;
  }
}

// Rehash function. Called when the number of elements in the hash table is greater than \
// the size of the vector.
template <typename K, typename V>
void HashTable<K, V>::rehash() // code derived from class PowerPoint
{
  auto oldList = theList;

	// create new double-sized, empty table
	theList.resize(prime_below(2 * theList.size()));
	makeEmpty();

	// copy table over
	for (auto & thisList : oldList)
		for (auto & x : thisList)
			insert(std::move(x));
}

// return the index of the vector entry where k should be stored.
template <typename K, typename V>
std::size_t HashTable<K, V>::myhash(const K& k) const
{
  static std::hash<K> hf; // derived from code in class PowerPoint, create from STL hash
	return hf(k) % theList.size();
}

// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below (unsigned long n)
{
  if (n > max_prime)
    {
      std::cerr << "** input too large for prime_below()\n";
      return 0;
    }
  if (n == max_prime)
    {
      return max_prime;
    }
  if (n <= 1)
    {
		std::cerr << "** input too small \n";
      return 0;
    }

  // now: 2 <= n < max_prime
  std::vector <unsigned long> v (n+1);
  setPrimes(v);
  while (n > 2)
    {
      if (v[n] == 1)
	return n;
      --n;
    }

  return 2;
}

//Sets all prime number indexes to 1. Called by method prime_below(n)
template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long>& vprimes)
{
  int i = 0;
  int j = 0;

  vprimes[0] = 0;
  vprimes[1] = 0;
  int n = vprimes.capacity();

  for (i = 2; i < n; ++i)
    vprimes[i] = 1;

  for( i = 2; i*i < n; ++i)
    {
      if (vprimes[i] == 1)
        for(j = i + i ; j < n; j += i)
          vprimes[j] = 0;
    }
}
