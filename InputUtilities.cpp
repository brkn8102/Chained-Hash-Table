// InputUtilities.cpp created by Brandon Knutson on 4/16/2015

#include "InputUtilities.h"


bool isInteger( std::string input )    // checks if string is made of number characters (and minus sign)
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

bool isPositiveInteger( std::string input )     // checks if string represents nonzero positive integer
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

bool isValidResponse( std::string input )   // checks if string is made of "Y"/"y" or "N"/"n" characters for conversion to boolean
{
    return ( input == "Y" || input == "y" || input == "N" || input == "n" );
}

bool getResponse( std::string input )       // converts string response to boolean
{
    return ( input == "Y" || input == "y" );
}

std::string getKey()    // asks user for key string then returns response
{
    std::string input = "";
    
    std::cout << "Enter the data element key (a string): ";
    std::cin >> input;
    
    return input;
}

int getValue()      // asks user for int value until reponse is valid (must be an integer)
{
    std::string input = "";
    
    std::cout << "Enter the data element value (an integer): ";
    std::cin >> input;
    
    while ( !isInteger( input ) )
    {
        std::cout << "Invalid input. Please enter an integer." << std::endl;
        
        std::cout << "Enter the data element value (an integer): ";
        std::cin >> input;
    }
    
    return stoi( input );
}