/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45
#define NUM_CHARACTERS 27

// trie data structure
typedef struct trie
{
    bool is_word;
    struct trie *p[NUM_CHARACTERS];
}
trie;

// global pointer variable to the root of the trie
trie *first;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word);

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

/**
 * A supplementary function for a recursive unload funtion
 */
bool clear(trie *x);

/**
 * Sets all the pointer arrays of the trie structure to null
 */
void set_null(trie *x);


#endif // DICTIONARY_H
