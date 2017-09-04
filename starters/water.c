#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("minutes: ");
    
    // getting input from user and storing it in no_of_min
    int no_of_min = GetInt();
    
    // printing the total no of bottles wasted
    printf("bottles: %d \n", no_of_min * 12);
}
