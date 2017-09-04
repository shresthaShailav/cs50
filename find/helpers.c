/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"
#define LIMIT 65536
/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // Binary Search
    int max = n - 1;
    int min = 0;
    while (max >= min)
    {
        int midpoint = (max + min) / 2;
        
        if (values[midpoint] > value)
        {
            max = midpoint - 1;
        }
        else if (values[midpoint] < value)
        {
            min = midpoint + 1;
        }
        else
            return true;
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // Counting sort O(2n+65536)
    
    int temp_arr[LIMIT] = {0};
    
    // counts the frequency of elements (1 - 65536)
    for (int i = 0; i < n; i++)
    {
        // incremets count of ith element by one in temp_arr
        temp_arr[values[i] - 1]++;
    }
    
    
    // sorts the given array
    int x = 0;
    for (int i = 1; i <= LIMIT; i++)
    {
        while ((temp_arr[i - 1]--) > 0)
        {
            values[x++] = i;
        }
    }
    
    return;
    
}
