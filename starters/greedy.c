#include <stdio.h>
#include <cs50.h>

/**
 * This function implements the greedy algorithm
 * The total dollars (in cents) is passed as argument
 * The function prints the numnber of quarters,nickels,dimes and cents
 */
 
void greedy(int money)
{
    int quarters = money / 25;
    money %= 25;             
    
    int dimes = money / 10;
    money %= 10;
    
    int nickels = money / 5;
    money %= 5;
    
    int cents = money;
    
    printf("%i\n", quarters + dimes + nickels + cents);
    
}

int main(void)
{
    double money = 0;
    while (true)
    {
        printf("Oh hai! How much change is owned?");
        money = GetDouble() * 100;
        if (money > 0)
        {
            break;
            
        }
    }
   
   
    greedy((int) money);
}

