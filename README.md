# Knutson_CSCI2270_FinalProject

# Dynamic and Customizable Hash Table

## Project Summary

This program is basically a hash table with added functionality. The essential feature is that this hash table has a dynamic capability which allows for the user to set a maximum number of allowed collisions for any given chain in the table. If this threshold is exceeded as data is inputted into the table, then the table is resized as necessary so that the the number of collisions in any chain is below or at the allowed value. This dynamic capability (which can be turned on or off), as well as the maximum number of allowed collisions, can be changed at any time. Also, the program allows for the user to set the table size. The basic data structure used to store information in this hash table is a struct called an "Element," which contains two properties: a key (string) and a value (int).

## How to Run

See [FinalProject.cpp file] (FinalProject.cpp), which contains examples of how to use the HashTable object. This cpp file is also a fully functional terminal application that the user can tinker around with for a further demonstration of the HashTable object in practice. There is also extensive documentation in the [HashTable.cpp file] (HashTable.cpp).
The following code simply demonstrates all of the HashTable public methods.
    
```go
int initialTableSize = 10;
bool dynamicCapability = true;
int maximumAllowedChainSize = 2;
HashTable *hashTable = new HashTable( initialTableSize, dynamicCapability, maximumAllowedChainSize );

string key = "key";
int value = 7;
hashTable->insertElement( key, value );

Element *element = hashTable->getElement( key );

hashTable->removeElement( key );

dynamicCapability = false;
int ignoredInteger = 4;   //maximum allowed chain size doesn't matter if turning dynamic capability off
hashTable->setDynamicCapability( dynamicCapability, ignoredInteger );

bool currentDynamicCapability = hashTable->getDynamicCapability();

int currentMaximumAllowedChainSize =  hashTable->getMaximumAllowedChainSize();

hashTable->setMaximumAllowedChainSize( currentMaximumAllowedChainSize + 1 );

int newTableSize = 15;
hashTable->setTableSize( newTableSize );

int currentTableSize = hashTable->getTableSize();
cout << "current table size: " << currentTableSize << endl;

int currentLargestChainSize = hashTable->getLargestChainSize();
cout << "current largest chain size: " << currentLargestChainSize << endl;

int currentNumberOfElements = hashTable->getNumberOfElements();
cout << "current number of elements: " << currentNumberOfElements << endl;

key = "some string";
int hashFunctionValue = hashTable->hashFunction( key );
cout << "hash function value: " << hashFunctionValue << endl;

string elementsString = hashTable->getElementsString();
cout << elementsString << endl;
```


## Dependencies

The entire program requires the following libraries that must be included into various files: vector, string, climits, iostream.

## System Requirements

This program has been used on both Mac (with Xcode) and Linux (with Code::Blocks) operating systems. 

## Group Members

* Brandon Knutson

## Contributors

* Peilun Zhang

## Open Issues/Bugs

* I was just recently made aware of a bug where if multiple data elements with the exact same key ASCII sum are entered into the hash table the terminal becomes unresponsive.