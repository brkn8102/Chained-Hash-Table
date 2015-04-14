// FinalProject.cpp created by Brandon Knutson on 4/13/2015

#include "HashTable.h"
#include <iostream>

using namespace std;

bool isInteger( string input )
{
    for ( int i = ( input[ 0 ] == '-' ? 1 : 0 ); i < input.size(); ++i )
    {
        int ASCIIValue = input[ i ];
        if ( ASCIIValue < 48 || 57 < ASCIIValue )
        {
            return false;
        }
    }
    
    return true;
}

bool isPositiveInteger( string input )
{
    if ( !isInteger( input ) )
    {
        return false;
    }
    else
    {
        int integer = stoi( input );
        return 0 < integer;
    }
}

string getKey()
{
    string input = "";
    
    cout << "Enter the data element key (a string): ";
    cin >> input;
    
    return input;
}

int getValue()
{
    string input = "";
    
    cout << "Enter the data element value (an integer): ";
    cin >> input;
    
    while ( !isInteger( input ) )
    {
        cout << "Invalid input. Please enter an integer." << endl;
        
        cout << "Enter the data element value (an integer): ";
        cin >> input;
    }
    
    return stoi( input );
}

int main()
{
    cout << "Welcome to Brandon's Customizable and Dynamic Hash Table." << endl << endl;
    
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
    
    cout << endl << "Enter the maximum allowed chain length: ";
    cin >> input;
    while ( !isPositiveInteger( input ) )
    {
        cout << "Invalid input. Please enter a positive integer." << endl;
        cout << "Enter the maximum allowed chain length: ";
        cin >> input;
    }
    int maximumChainLength = stoi( input );
    
    HashTable *hashTable = new HashTable( initialTableSize, maximumChainLength );
    
    while ( true )
    {
        cout << endl;
        cout << "______MAIN___MENU______" << endl;
        cout << "1. Insert data element" << endl;
        cout << "2. Remove data element" << endl;
        cout << "3. Find data element" << endl;
        cout << "4. Print table properties" << endl;
        cout << "5. Print table data elements" << endl;
        cout << "6. Quit" << endl;
        
        cin >> input;
        
        if ( input == "1" )
        {
            string key = getKey();
            int value = getValue();
            
            hashTable->insertElement( key, value );
            
            hashTable->resize();
        }
        else if ( input == "2" )
        {
            string key = getKey();
            
            hashTable->removeElement( key );
        }
        else if ( input == "3" )
        {
            string key = getKey();
            
            hashTable->findElement( key );
        }
        else if ( input == "4" )
        {
            hashTable->printProperties();
        }
        else if ( input == "5" )
        {
            hashTable->printElements();
        }
        else if ( input == "6" )
        {
            cout << "Goodbye." << endl;
            break;
        }
        else
        {
            cout << "Invalid input. Please enter an option number (1-5) from the menu below." << endl;
        }
    }
    
    return 0;
}
