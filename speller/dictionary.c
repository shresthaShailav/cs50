/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

long int siz = 0;


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int counter = 0;
    trie *trav = first;
    
    while (word[counter] != '\0')
    {
        
        // sort of hashing the funtion
        //int i = (tolower(word[counter]) - 'a') % (NUM_CHARACTERS);
        
        int i;
        if (word[counter] != '\'')
        {
            i = (tolower(word[counter]) - 'a'); //% NUM_CHARACTERS;
        }
        else 
        {
            i = 26;
        }
        
        // check if path exists
        if (trav->p[i] == NULL)
        {
            // if no, then word does not exist
            return 0;
        }
        else
        {
            trav = trav->p[i];
        }
        
        counter++;
    }
    
    if (trav->is_word == true)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    first = malloc(sizeof(trie));
    
    if (first == NULL)
    {
        return 0;
    }
    trie *trav = first;
    char c;
    
    // open file implement trie structure
    FILE *f_ptr = fopen(dictionary, "r");
    if (f_ptr == NULL)
    {
        return 0;
    }
    
    while((c = fgetc(f_ptr)) != EOF)
    {
        // word is complete prepare for next word
        if (c == '\n')
        {
            trav->is_word = "true";
            trav = first;
            siz++;
            continue;
        }
        
        int i;
        if (c != '\'')
        {
            i = (tolower(c) - 'a'); //% NUM_CHARACTERS;
        }
        else 
        {
            i = 26;
        }

        
        // check if character path already exists
        if (trav->p[i] == NULL)
        {
            // if not, create a new path
            trie *temp = malloc(sizeof(trie));
            
            // the pointers are not manually set to null. May cause problems.
            
            trav->p[i] = temp;
            trav = temp;
        }
        else 
        {
            // if exists, continue that path
            trav = trav->p[i];
        }
    }

    return 1;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return siz;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    return clear (first);
}

/**
 * A supplementary function for a recursive unload funtion
 */
bool clear(trie *x)
{
    if (x == NULL)
        return 0;
    for (int i = 0; i < NUM_CHARACTERS; i++)
    {
        if (x->p[i] != NULL)
            clear (x->p[i]);
    }
    free(x);
    return 1;
}

/**
 * Sets all the pointer arrays of the trie structure to null (not used)
 */
void set_null(trie *x)
{
    for (int i = 0; i < NUM_CHARACTERS; i++)
    {
        x->p[i] = NULL;
    }
}