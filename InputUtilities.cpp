// InputUtilities.cpp created by Brandon Knutson on 4/16/2015

#include "InputUtilities.h"


bool isInteger( std::string input )
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

bool isPositiveInteger( std::string input )
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

bool isValidResponse( std::string input )
{
    return ( input == "Y" || input == "y" || input == "N" || input == "n" );
}

bool getResponse( std::string input )
{
    return ( input == "Y" || input == "y" );
}

std::string getKey()
{
    std::string input = "";
    
    std::cout << "Enter the data element key (a string): ";
    std::cin >> input;
    
    return input;
}

int getValue()
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