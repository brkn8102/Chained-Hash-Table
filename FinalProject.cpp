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
    while ( !isPositiveInteger( input ) )   // initial table size must be positive integer
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
    while ( !isValidResponse( input ) )     // response must be "Y"/"y" or "N"/"n" to convert to boolean
    {
        cout << "Invalid input. Please enter \"Y\" or \"N\" to answer yes or no, respectively." << endl;
        cout << "Enter response (Y/N): ";
        cin >> input;
    }
    bool dynamicCapability = getResponse( input );  // sets dynamic capability of table when declared/initialized
    
    int maximumAllowedChainLength;
    if ( dynamicCapability )    // only need user to input maximum allowed chain length if dynamic capability is going to be enabled
    {
        cout << endl << "Enter the maximum allowed chain length: ";
        cin >> input;
        while ( !isPositiveInteger( input ) )   // maximum allowed chain length must be positive integer
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
    
    HashTable *hashTable = new HashTable( initialTableSize, dynamicCapability, maximumAllowedChainLength ); // this is how a HashTable is declared
    
    while ( true )      // this terminal program will demonstrate the methods of the HashTable class
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
            string key = getKey();  // gets valid key string from user
            int value = getValue();     // gets valid value int from user
            
            hashTable->insertElement( key, value );    // inserts new Element with key and value into
        }                                              //  hash table, then resizes table if dynamic capability is ON
        else if ( input == "2" )
        {
            string key = getKey();  // gets valid key string from user
            
            bool elementRemoved = hashTable->removeElement( key );  // removeElement( key ) returns true if element successfully removed,
                                                                    // false otherwise;
            if ( !elementRemoved ) cout << "Element not found." << endl;
        }
        else if ( input == "3" )
        {
            string key = getKey();   // gets valid key string from user
            
            Element *element = hashTable->getElement( key );  // getElement( key ) returns Element pointer with given key or null if not found
            
            if ( element == nullptr )
                cout << "Element not found." << endl;
            else
                std::cout << "Index " << hashTable->hashFunction( element->key ) << ": ( " << element->key << ", " << element->value << " )" << std::endl;
        }
        else if ( input == "4" )
        {
            cout << "Enter table size: ";
            cin >> input;
            while ( !isPositiveInteger( input ) ) // gets valid table size; table size must always be positive integer greater than zero
            {
                cout << "Invalid input. Please enter a positive integer." << endl;
                cout << "Enter the table size: ";
                cin >> input;
            }
            cout << endl;
            
            int tableSize = stoi( input );
            hashTable->setTableSize( tableSize );  // sets new size of table; resizes automatically if dynamic capability is ON
        }
        else if ( input == "5" )
        {
            bool dynamicCapability = !hashTable->getDynamicCapability();  // gets opposite of current dynamic capability
            
            int maximumAllowedChainSize = 0;
            if ( dynamicCapability )       // if about to switch on dynamic capability, must get maximum allowed chain size user wants
            {
                cout << "Enter the maximum allowed chain length: ";
                cin >> input;
                while ( !isPositiveInteger( input ) )       // maximum allowed chain size must be nonzero posive integer
                {
                    cout << "Invalid input. Please enter a positive integer." << endl;
                    cout << "Enter the maximum allowed chain length: ";
                    cin >> input;
                }
                cout << endl;
                maximumAllowedChainSize = stoi( input );
                
                hashTable->setMaximumAllowedChainSize( maximumAllowedChainSize ); // sets maximum allowed chain size
            }
            
            hashTable->setDynamicCapability( dynamicCapability, maximumAllowedChainSize ); // sets dynamic capability and maximum allowed
        }                                                                                  // chain size; ignores new max chain size if dynamic
                                                                                           // capability is being turned OFF
        else if ( input == "6" )
        {   // this is how to access hashTable properties such as table size, dynamic capability (true = ON, false = OFF ), maximum allowed
            cout << "Table size: " << hashTable->getTableSize() << endl;        // chain size, largest chain size, and number of Elements
            
            cout << "Dynamic table resizing capability: " << ( hashTable->getDynamicCapability() ? "ON" : "OFF" ) << endl;
            
            if ( hashTable->getDynamicCapability() ) cout << "Maximum allowed chain size: " << hashTable->getMaximumAllowedChainSize() << endl;
            
            cout << "Largest chain size: " << hashTable->getLargestChainSize() << endl;
            
            cout << "Number of elements: " << hashTable->getNumberOfElements() << endl;
        }
        else if ( input == "7" )
        {
            cout << hashTable->getElementsString();  // returns hash table information as string for output
        }
        else if ( input == "8" )
        {
            delete hashTable;       // deletes hashTable and all elements
            
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
