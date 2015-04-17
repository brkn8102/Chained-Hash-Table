// HashTable.cpp created by Brandon Knutson on 4/13/2015

#include "HashTable.h"

HashTable::HashTable( int initialTableSize, bool dynamic, int maximumAllowedChainSize )
{
    table = new std::vector< std::vector<Element *> * >( initialTableSize );
    dynamicCapability = dynamic;
    MAXIMUM_ALLOWED_CHAIN_SIZE = maximumAllowedChainSize;
    largestChainSize = 0;
    numberOfElements = 0;
    
    for ( int i = 0; i < initialTableSize; ++i )
    {
        (*table)[ i ] = nullptr;
    }
}

HashTable::~HashTable()
{
    for ( int i = 0; i < table->size(); ++i )
    {
        std::vector<Element *> *chain = (*table)[ i ];
        
        if ( chain != nullptr )
        {
            while ( chain->size() )
            {
                chain->erase( chain->begin() );
            }
            
            delete chain;
            chain = nullptr;
        }
    }
    
    delete table;
    table = nullptr;
}

void HashTable::insertElement( std::string key, int value )
{
    Element *element = new Element;
    element->key = key;
    element->value = value;
    
    int index = hashFunction( key );
    std::vector<Element *> *chain = (*table)[ index ];
    
    if ( chain == nullptr )
    {
        chain = new std::vector<Element *>( 1 );
        (*table)[ index ] = chain;
        chain->front() = element;
    }
    else
    {
        if ( key > chain->back()->key )
        {
            chain->push_back( element );
        }
        else
        {
            for ( int i = 0; i < chain->size(); ++i )
            {
                if ( key < (*chain)[ i ]->key )
                {
                    chain->insert( chain->begin() + i, element );
                }
            }
        }
    }
    
    if ( chain->size() > largestChainSize )
    {
        largestChainSize = chain->size();
    }
    
    ++numberOfElements;
}

bool HashTable::removeElement( std::string key )
{
    int chainIndex = getElementChainIndex( key );
    
    if ( chainIndex == -1 )
    {
        return false;
    }
    else
    {
        std::vector<Element *> *chain = (*table)[ hashFunction( key ) ];
        delete (*chain)[ chainIndex ];
        (*chain)[ chainIndex ] = nullptr;
        chain->erase( chain->begin() + chainIndex );
        
        if ( chain->size() == 0 )
        {
            (*table)[ hashFunction( key ) ] = nullptr;
        }
        
        --numberOfElements;
        
        return true;
    }
}

Element * HashTable::getElement( std::string key )
{
    int index = getElementChainIndex( key );
    
    if ( index == -1 )
        return nullptr;
    else
        return (*(*table)[ hashFunction( key ) ])[ index ];
}

void HashTable::setDynamicCapability( bool dynamic )
{
    dynamicCapability = dynamic;
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

int HashTable::getTableSize()
{
    return table->size();
}

int HashTable::getLargestChainSize()
{
    return largestChainSize;
}

int HashTable::getNumberOfElements()
{
    return numberOfElements;
}

std::string HashTable::getElementsString()
{
    std::string elements = "";
    
    for ( int i = 0; i < table->size(); ++i )
    {
        elements += "Index " + std::to_string( i ) + ": ";
        
        std::vector<Element *> *chain = (*table)[ i ];
        
        if ( chain != nullptr )
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

int HashTable::hashFunction( std::string key )
{
    int hashSum = 0;
    
    for ( int i = 0; i < key.size(); ++i )
    {
        hashSum += key[ i ];
    }
    
    return hashSum % table->size();
}

void HashTable::doubleTableSize()
{
    int tableSize = table->size();
    std::vector<Element *> *elements = new std::vector<Element *>( 0 );
    
    for ( int i = 0; i < tableSize; ++i )
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
    
    table = new std::vector< std::vector<Element *> * >( 2 * tableSize );
    largestChainSize = 0;
    
    while ( elements->size() )
    {
        insertElement( elements->front()->key, elements->front()->value );
        
        delete elements->front();
        elements->front() = nullptr;
        elements->erase( elements->begin() );
    }
    
    delete elements;
    elements = nullptr;
}

int HashTable::resize()
{
    int doublings = 0;
    
    while ( largestChainSize > MAXIMUM_ALLOWED_CHAIN_SIZE && dynamicCapability )
    {
        doubleTableSize();
        
        ++doublings;
    }
    
    return doublings;
}

int HashTable::getElementChainIndex( std::string key )
{
    int index = -1;
    
    std::vector<Element *> *chain = (*table)[ hashFunction( key ) ];
    
    if ( chain != nullptr )
    {
        for ( int i = 0; i < chain->size(); ++i )
        {
            if ( key == (*chain)[ i ]->key )
            {
                index = i;
            }
        }
    }
    
    return index;
}

