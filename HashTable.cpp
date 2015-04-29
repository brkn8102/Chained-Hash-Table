// HashTable.cpp created by Brandon Knutson on 4/13/2015

#include "HashTable.h"

/*
Function Prototype:
 HashTable::HashTable( int, bool, int );
 
Function description: 
 HashTable constructor sets new table's initial size to initialTableSize; sets hash table dynamic capability
 (automatic resizing for collision reduction) to dynamic (true = ON, false = OFF); sets maximum allowable
 chain size before table resizing to maximumAllowedChainSize unless dynamic is off, in which case the maximum
 chain size is set to INT_MAX.

Example:
 HashTable *hashTable = new HashTable( 5, TRUE, 2 );

Pre-condition: initialTableSize and maximumAllowedChainSize are positive integers.
Post-condition: table with given properties is created with all chain pointers set to null.
*/
HashTable::HashTable( int initialTableSize, bool dynamic, int maximumAllowedChainSize )
{
    table = new std::vector< std::vector<Element *> * >( initialTableSize ); // initializes HashTable properties
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

/*
Function Prototype:
 HashTable::~HashTable();

Function Description:
 HashTable destructor. Deletes then sets to null all hash table element and chain pointers and the table pointer.

Example:
 delete hashTable;

Pre-condition: table must not be null. 
Post-condition: all table elements and chains deleted; table set to null.
*/
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

/*
Function Prototype:
 void HashTable::insertElement( std::string, int );
 
Function Description:
 Inserts element with given key and value into table.
 
Example:
 hashTable->insertElement( "key", 52 );
 
Pre-condition: table must be non-null (which must be true in order for this method to be called).
Post condition: new element has been inserted into table and table is resized if dynamic capability is ON
 and collisions need to be reduced.
 */
void HashTable::insertElement( std::string key, int value )
{
    Element *element = new Element;     // allocates new element
    element->key = key;
    element->value = value;
    
    insertElementByPointer( element );
    
    resize();     // will resize if dynamic capability has been turned on; otherwise it won't do anything
}

/*
Function Prototype:
 void HashTable::insertElementByPointer( Element * );

Function Description:
 Inserts element by Element pointer.
 
Example:
 hashTable->insertElementByPointer( Element );
 
Pre-condition: inserted element and table must be non-null.
Post-condition: element pointer is inserted into table, increments numberOfElements, updates largestChainSize; 
 unlike insertElement function, does NOT resize table.
*/
void HashTable::insertElementByPointer( Element *element )
{
    int tableIndex = hashFunction( element->key );
    std::vector<Element *> *chain = (*table)[ tableIndex ];
    int insertFlag = 0;
    if ( chain == nullptr )     // new chain allocated if chain is null
    {
        chain = new std::vector<Element *>( 1 );
        (*table)[ tableIndex ] = chain;
        chain->front() = element;
        insertFlag = 1;
    }
    else       // adds element to ordered location in chain if chain isn't null
    {
        if ( element->key > chain->back()->key )
        {
            chain->push_back( element );
            insertFlag = 1;
        }
        else
        {
            for ( int i = 0; i < chain->size(); ++i )
            {
                if ( element->key < (*chain)[ i ]->key )
                {
                    chain->insert( chain->begin() + i, element );
                    insertFlag = 1;
                }
            }
        }
    }
    
    if ( chain->size() > largestChainSize ) // if new chain length is larger than largest, updates largest
    {
        largestChainSize = chain->size();
    }
    if(insertFlag)++numberOfElements;  // increments number of elements

}

/*
Function Prototype:
 bool HashTable::removeElement( std::string )

Function Description:
Removes element with given key from table. Returns boolean representing successful removal.

Example:
 hashTable->removeElement( "key" );
 
Pre-condition: table is not null. 
Post-condition: removes element with given key from table and returns true, or if element is not found 
 removes nothing and returns false; updates/resets largestChainSize.
*/
bool HashTable::removeElement( std::string key )
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

/*
Function Prototype:
 Element * HashTable::getElement( std::string );

Function Description:
 Gets element from table. Returns pointer to found element.

Example:
 hashTable->getElement( "key" );
 
Pre-condition: table is not null. 
Post-condition: nothing is changed since function is used only to read values; returned pointer 
 should not be used to edit element.
*/
Element * HashTable::getElement( std::string key )
{
    int index = getElementChainIndex( key );
    
    if ( index == -1 )   // if element not found in chain, return null
        return nullptr;
    else
        return (*(*table)[ hashFunction( key ) ])[ index ];  // otherwise, return pointer to element
}

/*
Function Prototype:
 void HashTable::setDynamicCapability( bool, int );
 
Function Description:
 Sets dynamicCapability (true = ON, false = OFF) and MAXIMUM_ALLOWED_CHAIN_SIZE to given values.

Example:
 hashTable->setDynamicCapability( TRUE );
 
Pre-condition: maximumAllowedChainSize should be a nonzero positive integer and table should not be null.
Post-condition: if dynamic capability is being turned ON (true), sets maximum allowed chain size, 
 otherwise sets maximum allowed chain size to INT_MAX.
*/
void HashTable::setDynamicCapability( bool dynamic, int maximumAllowedChainSize )
{
    dynamicCapability = dynamic;
    
    if ( dynamicCapability ) // if dynamic capability is being turned ON, set maximum chain size
        MAXIMUM_ALLOWED_CHAIN_SIZE = maximumAllowedChainSize;
    else   // otherwise, just make maximum chain size the maximum integer value
        MAXIMUM_ALLOWED_CHAIN_SIZE = INT_MAX;
    
    resize();  // will resize if dynamic capability has been turned on; otherwise it won't do anything
}

bool HashTable::getDynamicCapability()
{
    return dynamicCapability;
}

void HashTable::setMaximumAllowedChainSize( int maximumAllowedChainSize )
{
    MAXIMUM_ALLOWED_CHAIN_SIZE = maximumAllowedChainSize;
}

int HashTable::getMaximumAllowedChainSize()
{
    return MAXIMUM_ALLOWED_CHAIN_SIZE;
}

/*
Function Prototype:
 void HashTable::setTableSize( int );

Function Description:
 Sets table size to tableSize. 

Example:
 hashTable->setTableSize( 5 );
 
Pre-condition: tableSize is a nonzero positive integer, table is not null.
Post-condition: moves elements from previous table to new table with new table size into proper places,
 if dynamicCapability is ON, resizes table to reduce collisions.
*/
void HashTable::setTableSize( int tableSize )
{
    std::vector<Element *> *elements = moveElementsFromTable(); // moves elements from table to elements vector
    
    table = new std::vector< std::vector<Element *> * >( tableSize );  // sets table size to new value
    largestChainSize = 0; // no elements in table at this point so largest chain size must be 0
    
    moveElementsToTable( elements );  // moves elements to location in new table
    
    resize();
}

int HashTable::getTableSize()
{
    return table->size();
}

/*
Function Prototype:
 void HashTable::resetLargestChainSize();

Function Description:
 Goes through array to find largest chain size, updates largestChainSize. 

Example:
 table->resetLargestChainSize();
 
Pre-condition: table is not null.
Post-condition: updates largestChainSize to actual current largest chain size in table.
*/
void HashTable::resetLargestChainSize()
{
    largestChainSize = 0;
    
    for ( int i = 0; i < table->size(); ++i )
    {
        std::vector<Element *> *chain = (*table)[ i ];
        
        if ( chain != nullptr )
        {
            int chainSize = chain->size();
            
            if ( chainSize > largestChainSize ) // update largestChainSize if chainSize is bigger
            {
                largestChainSize = chainSize;
            }
        }
    }
}

int HashTable::getLargestChainSize()
{
    return largestChainSize;
}

int HashTable::getNumberOfElements()
{
    return numberOfElements;
}

/*
Function Prototype:
 std::string HashTable::getElementsString();

Function Description:
 Gets/returns a string with information about all the elements in the table. 
 
Example:
 hashTable->getElementsString();
 
Pre-condition: table is not null.
Post-condition: nothing is changed, only reads values.
*/
std::string HashTable::getElementsString()
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

/*
Function Prototype:
 int HashTable::hashFunction( std::string );
 
Function Description:
 Gets/returns hash function integer value for given key (the sum of the character ASCII values modulus
 the table size). This is the index in the table where elements with the given key are placed.

Example:
 hashTable->hashFunction( "key" );
 
Pre-condition: table is not null.
Post-condition: nothing is changed, only reads values and returns hash sum.
*/
int HashTable::hashFunction( std::string key )
{
    int hashSum = 0;
    
    for ( int i = 0; i < key.size(); ++i )  // sums ASCII values of all characters in key string
    {
        hashSum += key[ i ];
    }
    
    return hashSum % table->size();  // returns remainder of hash sum so that index is less than table size
}

/*
Function Prototype:
 int HashTable::getElementChainIndex( std::string );
 
Function Description:
 Gets/returns index of element with given key in a chain of the table or -1 if not found. 
 
Example:
 hashTable->getElementChainIndex( "key" );
 
Pre-condition: table is not null.
Post-condition: nothing is changed, only reads values.
*/
int HashTable::getElementChainIndex( std::string key )
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

/*
Function Prototype:
 std::vector<Element *> * HashTable::moveElementsFromTable();
 
Function Description:
 Returns pointer to vector of Element pointers, all of which have been removed from the table. 
 
Example:
 std::vector<Element *> *elements = hashTable->moveElementsFromTable();
 
Pre-condition: table is not null.
Post-condition: table is null and all elements have been removed from table, numberOfElements is set to 0.
*/
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

/*
Function Prototype:
 void HashTable::moveElementsToTable( std::vector<Element *> * );

Function Description:
 Inserts all the elements in a given vector of element pointers into the table. 
 
Example:
 hashTable->moveElementsToTable( elements );
 
Pre-condition: elements is not null.
Post-condition: all elements added into their proper place in hash table, numberOfElements 
 and largestChain size are updated, elements vector is set to null.
*/
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

/*
Function Prototype:
 void HashTable::resize();
 
Function Description:
 Continuously doubles the the table size until the largest chain size is less than or equal to 
 the maximum allowed chain size. 
 
Example:
 hashTable->resize();
 
Pre-condition: table is not null. 
Post-condition: if dynamicCapability is true (ON) and the maximum allowed chain size has been 
 exceeded, continues doubling table size and repositioning the elements into their proper location 
 until the collisions have been reduced to allowed levels; otherwise, does nothing.
*/
void HashTable::resize()
{
    while ( largestChainSize > MAXIMUM_ALLOWED_CHAIN_SIZE && dynamicCapability ) // dynamic capability must be ON
    {
        setTableSize( 2 * table->size() );
    }
}

