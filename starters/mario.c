#include <stdio.h>
#include <cs50.h>

/**
 * The function pos_int request for a valid integer
 * it does not take an argument
 * Returns an integer if the integer is positive or 0
 * if invalid integer is entered, it continuously asks for integer.
 */

int pos_int();

int main(void)
{
    // Getting a valid integer from user
    printf("Height: ");
    int height = pos_int();
    
    // creating the mario's pyramid of stupulated height
    for (int i = 1; i <= height; i++)
    {
        for (int j = height; j >= 1; j--)
        {
            if (j <= i)
            {
                printf("#");
                
            }
            else
            {
                printf(" ");
                
            }
        }
        printf("#\n");
    }
}

int pos_int()
{
    int x = 0;
    bool check = 0;
    do
    {
        x = GetInt();
        if (x >= 0 && x < 24)
        {
            check = false;
            
        }
        else
        {
            printf("Height: ");
            check = true;
        }
    } 
    while(check);
    
    return x;
    
}

