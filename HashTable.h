// HashTable.h created by Brandon Knutson on 4/13/2015

#ifndef __Working__HashTable__
#define __Working__HashTable__

#include <vector>
#include <string>
#include <climits>

struct Element
{
    std::string key;
    int value;
};

class HashTable
{
private:
    std::vector< std::vector<Element *> * > *table;
    bool dynamicCapability;
    int MAXIMUM_ALLOWED_CHAIN_SIZE;
    int largestChainSize;
    int numberOfElements;
    
    void insertElementByPointer( Element *element );
    int getElementChainIndex( std::string key );
    std::vector<Element *> * moveElementsFromTable();
    void moveElementsToTable( std::vector<Element *> *elements );
    void resetLargestChainSize();
    void resize();
    
public:
    HashTable( int initialTableSize, bool dynamicCapability, int MAXIMUM_ALLOWED_CHAIN_SIZE );
    virtual ~HashTable();
    
    void insertElement( std::string key, int value );
    bool removeElement( std::string key );
    Element * getElement( std::string key );
    
    void setDynamicCapability( bool dynamicCapability, int maximumAllowedChainSize );
    bool getDynamicCapability();
    
    void setMaximumAllowedChainSize( int maximumAllowedChainSize );
    int getMaximumAllowedChainSize();
    
    void setTableSize( int tableSize );
    int getTableSize();
    
    int getLargestChainSize();
    
    int getNumberOfElements();
    
    int hashFunction( std::string key );
    
    std::string getElementsString();
};

#endif /* defined(__Working__HashTable__) */
