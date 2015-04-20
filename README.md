# Knutson_CSCI2270_FinalProject

# Dynamic and Customizable Hash Table

## Project Summary

This program is basically a hash table with added functionality. The essential feature is that this hash table has a dynamic capability which allows for the user to set a maximum number of allowed collisions for any given chain in the table. If this threshold is exceeded as data is inputted into the table, then the table is resized as necessary so that the the number of collisions in any chain is below or at the allowed value. This dynamic capability (which can be turned on or off), as well as the maximum number of allowed collisions, can be changed at any time. Also, the program allows for the user to set the table size. The basic data structure used to store information in this hash table is a struct called an "Element," which contains two properties: a key (string) and a value (int).

## How to Run

See [FinalProject.cpp file] (FinalProject.cpp), which contains examples of how to use the HashTable object. This cpp file is also a fully functional terminal application that the user can tinker around with for a further demonstration of the HashTable object in practice.

## Dependencies

The entire program requires the following libraries that must be included into various files: vector, string, climits, iostream.

## System Requirements

This program has been used on both Mac (with Xcode) and Linux (with Code::Blocks) operating systems. 

## Group Members

* Brandon Knutson

## Contributors

N/A

## Open Issues/Bugs

N/A