// HashTable.h created by Brandon Knutson on 4/13/2015

#ifndef __Working__HashTable__
#define __Working__HashTable__

#include <iostream>
#include <vector>

struct Element
{
    std::string key;
    int value;
};

class HashTable
{
private:
    std::vector< std::vector<Element *> * > *table;
    int MAXIMUM_ALLOWED_CHAIN_SIZE;
    int largestChainSize;
    int numberOfElements;
    
    int findElementIndex( std::string key );
    int hashFunction( std::string key );
    void doubleTableSize();
    
public:
    HashTable( int initialTableSize, int MAXIMUM_ALLOWED_CHAIN_SIZE );
    virtual ~HashTable();
    void insertElement( std::string key, int value );
    void removeElement( std::string key );
    void findElement( std::string key );
    void printElements();
    void printProperties();
    void resize();
    
};

#endif /* defined(__Working__HashTable__) */
