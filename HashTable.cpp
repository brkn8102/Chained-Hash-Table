// HashTable.cpp created by Brandon Knutson on 4/13/2015

#include "HashTable.h"

HashTable::HashTable( int initialTableSize, int maximumAllowedChainSize )
{
    table = new std::vector< std::vector<Element *> * >( initialTableSize );
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
            
            delete [] chain;
            chain = nullptr;
        }
    }
    
    delete [] table;
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

void HashTable::removeElement( std::string key )
{
    int index = findElementIndex( key );
    
    if ( index == -1 )
    {
        std::cout << "Element not found." << std::endl;
    }
    else
    {
        std::vector<Element *> *chain = (*table)[ hashFunction( key ) ];
        delete (*chain)[ index ];
        (*chain)[ index ] = nullptr;
        chain->erase( chain->begin() + index );
        
        if ( chain->size() == 0 )
        {
            (*table)[ hashFunction( key ) ] = nullptr;
        }
        
        --numberOfElements;
    }
}

void HashTable::findElement( std::string key )
{
    int index = findElementIndex( key );
    
    if ( index == -1 )
    {
        std::cout << "Element not found." << std::endl;
    }
    else
    {
        Element *element = (*(*table)[ hashFunction( key ) ])[ index ];
        std::cout << "Index " << hashFunction( key ) << ": ( " << element->key << ", " << element->value << " )" << std::endl;
    }
}

int HashTable::findElementIndex( std::string key )
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

void HashTable::printProperties()
{
    std::cout << "Table size: " << table->size() << std::endl;
    std::cout << "Maximum allowed chain size: " << MAXIMUM_ALLOWED_CHAIN_SIZE << std::endl;
    std::cout << "Largest chain size: " << largestChainSize << std::endl;
    std::cout << "Number of elements: " << numberOfElements << std::endl;
}

void HashTable::printElements()
{
    for ( int i = 0; i < table->size(); ++i )
    {
        std::cout << "Index " << i << ": ";
        
        std::vector<Element *> *chain = (*table)[ i ];
        
        if ( chain != nullptr )
        {
            for ( int j = 0; j < chain->size() - 1; ++j )
            {
                Element *element = (*chain)[ j ];
                std::cout << "( " << element->key << ", " << element->value << " ), ";
            }
            Element *lastElement = chain->back();
            std::cout << "( " << lastElement->key << ", " << lastElement->value << " )";
        }
        
        std::cout << std::endl;
    }
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

void HashTable::resize()
{
    while ( largestChainSize > MAXIMUM_ALLOWED_CHAIN_SIZE )
    {
        doubleTableSize();
        
        std::cout << "Table size doubled." << std::endl;
    }
}