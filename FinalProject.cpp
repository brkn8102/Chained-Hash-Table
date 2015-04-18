// FinalProject.cpp created by Brandon Knutson on 4/13/2015

#include "InputUtilities.h"
#include "HashTable.h"
#include <iostream>

using namespace std;

int main()
{
    cout << "Welcome to the Customizable and Dynamic Hash Table." << endl << endl;
    
    string input = "";
    
    cout << "Enter the initial table size: ";
    cin >> input;
    while ( !isPositiveInteger( input ) )
    {
        cout << "Invalid input. Please enter a positive integer." << endl;
        cout << "Enter the initial table size: ";
        cin >> input;
    }
    int initialTableSize = stoi( input );
    
    //Dynamic table resizing allows for the number of hash table collisions to be kept below a set threshold by increasing the table size.
    cout << endl << "Would you like to enable the dynamic table resizing capability?" << endl;
    cout << "Enter input (Y/N): ";
    cin >> input;
    while ( !isValidResponse( input ) )
    {
        cout << "Invalid input. Please enter \"Y\" or \"N\" to answer yes or no, respectively." << endl;
        cout << "Enter response (Y/N): ";
        cin >> input;
    }
    bool dynamicCapability = getResponse( input );
    
    int maximumAllowedChainLength;
    if ( dynamicCapability )
    {
        cout << endl << "Enter the maximum allowed chain length: ";
        cin >> input;
        while ( !isPositiveInteger( input ) )
        {
            cout << "Invalid input. Please enter a positive integer." << endl;
            cout << "Enter the maximum allowed chain length: ";
            cin >> input;
        }
        maximumAllowedChainLength = stoi( input );
    }
    else
    {
        maximumAllowedChainLength = -1;
    }
    
    HashTable *hashTable = new HashTable( initialTableSize, dynamicCapability, maximumAllowedChainLength );
    
    while ( true )
    {
        cout << endl;
        cout << "______MAIN___MENU______" << endl;
        cout << "1. Insert data element" << endl;
        cout << "2. Remove data element" << endl;
        cout << "3. Find data element" << endl;
        cout << "4. Set table size" << endl;
        cout << "5. Switch " << ( hashTable->getDynamicCapability() ? "OFF" : "ON" ) << " dynamic resizing capability" << endl;
        cout << "6. Print table properties" << endl;
        cout << "7. Print table data elements" << endl;
        cout << "8. Quit" << endl;
        
        cin >> input;
        
        if ( input == "1" )
        {
            string key = getKey();
            int value = getValue();
            
            hashTable->insertElement( key, value );
        }
        else if ( input == "2" )
        {
            string key = getKey();
            
            bool elementRemoved = hashTable->removeElement( key );
            
            if ( !elementRemoved ) cout << "Element not found." << endl;
        }
        else if ( input == "3" )
        {
            string key = getKey();
            
            Element *element = hashTable->getElement( key );
            
            if ( element == nullptr )
                cout << "Element not found." << endl;
            else
                std::cout << "Index " << hashTable->hashFunction( element->key ) << ": ( " << element->key << ", " << element->value << " )" << std::endl;
        }
        else if ( input == "4" )
        {
            cout << "Enter table size: ";
            cin >> input;
            while ( !isPositiveInteger( input ) )
            {
                cout << "Invalid input. Please enter a positive integer." << endl;
                cout << "Enter the table size: ";
                cin >> input;
            }
            cout << endl;
            
            int tableSize = stoi( input );
            hashTable->setTableSize( tableSize );
        }
        else if ( input == "5" )
        {
            bool dynamicCapability = !hashTable->getDynamicCapability();
            
            int maximumAllowedChainSize = 0;
            if ( dynamicCapability )
            {
                cout << "Enter the maximum allowed chain length: ";
                cin >> input;
                while ( !isPositiveInteger( input ) )
                {
                    cout << "Invalid input. Please enter a positive integer." << endl;
                    cout << "Enter the maximum allowed chain length: ";
                    cin >> input;
                }
                cout << endl;
                maximumAllowedChainSize = stoi( input );
                
                hashTable->setMaximumAllowedChainSize( maximumAllowedChainSize );
            }
            
            hashTable->setDynamicCapability( dynamicCapability, maximumAllowedChainSize );
        }
        else if ( input == "6" )
        {
            cout << "Table size: " << hashTable->getTableSize() << endl;
            
            cout << "Dynamic table resizing capability: " << ( hashTable->getDynamicCapability() ? "ON" : "OFF" ) << endl;
            
            if ( hashTable->getDynamicCapability() ) cout << "Maximum allowed chain size: " << hashTable->getMaximumAllowedChainSize() << endl;
            
            cout << "Largest chain size: " << hashTable->getLargestChainSize() << endl;
            
            cout << "Number of elements: " << hashTable->getNumberOfElements() << endl;
        }
        else if ( input == "7" )
        {
            cout << hashTable->getElementsString();
        }
        else if ( input == "8" )
        {
            delete hashTable;
            
            cout << "Goodbye." << endl;
            break;
        }
        else
        {
            cout << "Invalid input. Please enter an option number (1-7) from the menu below." << endl;
        }
    }
    
    return 0;
}
