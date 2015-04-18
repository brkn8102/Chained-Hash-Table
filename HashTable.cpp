// HashTable.cpp created by Brandon Knutson on 4/13/2015

#include "HashTable.h"

HashTable::HashTable( int initialTableSize, bool dynamic, int maximumAllowedChainSize )
{
    table = new std::vector< std::vector<Element *> * >( initialTableSize );  // initializes HashTable properties
    dynamicCapability = dynamic;
    if ( maximumAllowedChainSize > 0 ) MAXIMUM_ALLOWED_CHAIN_SIZE = maximumAllowedChainSize;
    else MAXIMUM_ALLOWED_CHAIN_SIZE = INT_MAX;
    largestChainSize = 0;
    numberOfElements = 0;
    
    for ( int i = 0; i < initialTableSize; ++i )   // sets chain pointers to null
    {
        (*table)[ i ] = nullptr;
    }
}

HashTable::~HashTable()
{
    for ( int i = 0; i < table->size(); ++i )  // deletes all chains and their elements
    {
        std::vector<Element *> *chain = (*table)[ i ];
        
        if ( chain != nullptr )
        {
            while ( chain->size() )            // deletes chain elements
            {
                chain->erase( chain->begin() );
            }
            
            delete chain;       // deletes chain
            chain = nullptr;
        }
    }
    
    delete table;       // deletes table
    table = nullptr;
}

void HashTable::insertElement( std::string key, int value )     //inserts element with key and value into table then resizes
{
    Element *element = new Element;     // allocates new element
    element->key = key;
    element->value = value;
    
    insertElementByPointer( element );
    
    resize();     // will resize if dynamic capability has been turned on; otherwise it won't do anything
}

void HashTable::insertElementByPointer( Element *element )    // inserts element into table by element pointer; doesn't resize
{
    int tableIndex = hashFunction( element->key );
    std::vector<Element *> *chain = (*table)[ tableIndex ];
    
    if ( chain == nullptr )     // new chain allocated if chain is null
    {
        chain = new std::vector<Element *>( 1 );
        (*table)[ tableIndex ] = chain;
        chain->front() = element;
    }
    else       // adds element to ordered location in chain if chain isn't null
    {
        if ( element->key > chain->back()->key )
        {
            chain->push_back( element );
        }
        else
        {
            for ( int i = 0; i < chain->size(); ++i )
            {
                if ( element->key < (*chain)[ i ]->key )
                {
                    chain->insert( chain->begin() + i, element );
                }
            }
        }
    }
    
    if ( chain->size() > largestChainSize )  // if new chain length is larger than largest, updates largest
    {
        largestChainSize = chain->size();
    }
    
    ++numberOfElements;  // increments number of elements
}

bool HashTable::removeElement( std::string key )  // removes element with key from table
{
    int chainIndex = getElementChainIndex( key );
    
    if ( chainIndex == -1 )
    {
        return false;   // if failed to find and remove element, return false
    }
    else
    {
        std::vector<Element *> *chain = (*table)[ hashFunction( key ) ];  // remove element
        delete (*chain)[ chainIndex ];
        (*chain)[ chainIndex ] = nullptr;
        chain->erase( chain->begin() + chainIndex );
        
        if ( chain->size() == 0 )  // sets chain to null if its only element was removed
        {
            (*table)[ hashFunction( key ) ] = nullptr;
        }
        
        --numberOfElements;   // if element is successfully removed, decrements number of elements
        
        resetLargestChainSize();
        
        return true;   // if element is successfully removed, return true
    }
}

Element * HashTable::getElement( std::string key )  // return element pointer of element with given key from table
{
    int index = getElementChainIndex( key );
    
    if ( index == -1 )   // if element not found in chain, return null
        return nullptr;
    else
        return (*(*table)[ hashFunction( key ) ])[ index ];  // otherwise, return pointer to element
}

void HashTable::setDynamicCapability( bool dynamic, int maximumAllowedChainSize )  // set dynamic capability true = ON or false = OFF
{                                                                                  // and maximum chain size
    dynamicCapability = dynamic;
    
    if ( dynamicCapability )                                    // if dynamic capability is being turned ON, set maximum chain size
        MAXIMUM_ALLOWED_CHAIN_SIZE = maximumAllowedChainSize;
    else                                                        // otherwise, just make maximum chain size the maximum integer value
        MAXIMUM_ALLOWED_CHAIN_SIZE = INT_MAX;
    
    resize();     // will resize if dynamic capability has been turned on; otherwise it won't do anything
}

bool HashTable::getDynamicCapability()  // get dynamic capability true = ON or false = OFF
{
    return dynamicCapability;
}

void HashTable::setMaximumAllowedChainSize( int maximumAllowedChainSize ) // sets maximum allowed chain size before resizing
{
    MAXIMUM_ALLOWED_CHAIN_SIZE = maximumAllowedChainSize;
}

int HashTable::getMaximumAllowedChainSize() // gets maximum allowed chain size before resizing
{
    return MAXIMUM_ALLOWED_CHAIN_SIZE;
}

void HashTable::setTableSize( int tableSize )  // changes table size to set value
{
    std::vector<Element *> *elements = moveElementsFromTable();  // moves elements from table to elements vector
    
    table = new std::vector< std::vector<Element *> * >( tableSize );  // sets table size to new value
    largestChainSize = 0; // no elements in table at this point so largest chain size must be 0
    
    moveElementsToTable( elements );  // moves elements to location in new table
    
    resize();
}

int HashTable::getTableSize()
{
    return table->size();
}

void HashTable::resetLargestChainSize()
{
    largestChainSize = 0;
    
    for ( int i = 0; i < table->size(); ++i )
    {
        std::vector<Element *> *chain = (*table)[ i ];
        
        if ( chain != nullptr )
        {
            int chainSize = chain->size();
            
            if ( chainSize > largestChainSize )     // if chain size is larger than largest chain size, reset largest chain size
            {
                largestChainSize = chainSize;
            }
        }
    }
}

int HashTable::getLargestChainSize()  // gets current largest chain size in table
{
    return largestChainSize;
}

int HashTable::getNumberOfElements()  // gets current number of elements in table
{
    return numberOfElements;
}

std::string HashTable::getElementsString()  // return string of all the chains and their elements
{
    std::string elements = "";
    
    for ( int i = 0; i < table->size(); ++i )
    {
        elements += "Index " + std::to_string( i ) + ": ";  // adds table index
        
        std::vector<Element *> *chain = (*table)[ i ];
        
        if ( chain != nullptr )     // if chain not null, adds chain elements
        {
            for ( int j = 0; j < chain->size() - 1; ++j )
            {
                Element *element = (*chain)[ j ];
                elements += "( " + element->key + ", " + std::to_string( element->value ) + " ), ";
            }
            Element *lastElement = chain->back();
            elements += "( " + lastElement->key + ", " + std::to_string( lastElement->value ) + " )";
        }
        
        elements += "\n";
    }
    
    return elements;
}

int HashTable::hashFunction( std::string key ) // maps key of element to associated index in hash table (character ASCII value sum)
{
    int hashSum = 0;
    
    for ( int i = 0; i < key.size(); ++i )  // sums ASCII values of all characters in key string
    {
        hashSum += key[ i ];
    }
    
    return hashSum % table->size();  // returns remainder of hash sum so that index is less than table size
}

int HashTable::getElementChainIndex( std::string key )  // gets chain index of element with key
{
    int index = -1;  // will return -1 if element with key is not found or chain is null
    
    std::vector<Element *> *chain = (*table)[ hashFunction( key ) ];
    
    if ( chain != nullptr )
    {
        for ( int i = 0; i < chain->size(); ++i )  // searches chain for element with key
        {
            if ( key == (*chain)[ i ]->key )
            {
                index = i;
            }
        }
    }
    
    return index; // returns chain index of found element with key or -1 if unsuccessful
}

std::vector<Element *> * HashTable::moveElementsFromTable()
{
    std::vector<Element *> *elements = new std::vector<Element *>( 0 ); // temporarily stores elements
    
    for ( int i = 0; i < table->size(); ++i )  // stores elements then deletes from table
    {
        std::vector<Element *> *chain = (*table)[ i ];
        
        while ( chain != nullptr && chain->size() )
        {
            Element *element = new Element;
            *element = *chain->front();
            elements->push_back( element );
            
            removeElement( element->key );
        }
    }
    
    delete table;           // deletes entire table
    table = nullptr;
    
    return elements;        // returns elements moved from table
}

void HashTable::moveElementsToTable( std::vector<Element *> *elements )
{
    while ( elements->size() )  // moves elements into proper location in table
    {
        Element *copyElement = new Element();
        *copyElement = *elements->front();
        
        insertElementByPointer( copyElement );
        
        delete elements->front();
        elements->front() = nullptr;
        elements->erase( elements->begin() );
    }
    
    delete elements;    // deletes temporary vector for storing elements
    elements = nullptr;
}

void HashTable::resize()  // doubles table until largest chain size is within allowed threshold
{
    while ( largestChainSize > MAXIMUM_ALLOWED_CHAIN_SIZE && dynamicCapability )  // dynamic capability must be ON to double
    {
        setTableSize( 2 * table->size() );
    }
}

