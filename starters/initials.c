#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>



int main(void)
{
    // prompt for user input and store in the variable name
    string name = GetString();
    
    // Printing the Initial letter of the first name 
    printf( "%c" , toupper(name[0]));
    
    // Iterating and checking for spaces and printing the character next to it
    for (int i = 1, n = strlen(name); i < n ; i++)
    {
        if (name[i - 1] == ' ')
        {
            printf("%c",toupper(name[i]));
        }
    }
    printf("\n");
}