#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * The functions takes in a character and rotates the character by the key
 * The AZ rotates only uppercase character and prints the ciphered character
 * az roates only lowercase characters and print the ciphered character
 * 
 * 
 */
void cipher_AZ(char x, int key);
void cipher_az(char x, int key);



int main(int argc, string argv[])

{
    // getting the key through command line
    string k;
    if (argc == 2)
    {
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            if (!(isalpha(argv[1][i])))
            {
                printf("Key should only have alphabet");
                return 1;
                
            }
        }
       
        k = argv[1];
    }
    else 
    {
        printf("Bad Input! Expected ./<program_name> <key>");
        return 1;
    }
    
    
    // creating and array of keys from the commandline input
    int key_letters = strlen(k);
    int key[key_letters];
    
    for (int i = 0; i < key_letters; i++)
    {
        if (isupper(k[i]))
        {
            key[i] = k[i] - 'A';
            continue;
        }
        
        if (islower(k[i]))
        {
            key[i] = k[i] - 'a';
            continue;
        }
    }
    
    
   
    // sgetting user prompt
    string prompt = GetString();
    
    // iterating the string to change the charcter
    for (int i = 0, n = strlen(prompt); i < n; i++)
    {
        // iterating over the keys with every loop
        static int j = 0;
        j %= key_letters;
        
        
        if (isspace(prompt[i]))
        {
            printf(" ");
            continue;
        }
        
        if (isupper(prompt[i]))
        {
            cipher_AZ(prompt[i],key[j++]);
            continue;
        }
        
        if (islower(prompt[i]))
        {
            cipher_az(prompt[i],key[j++]);
            continue;
        }
        
        printf("%c",prompt[i]);
    }
    printf("\n");
    
}


void cipher_AZ(char x, int key)
{
    x -= 'A';
    x += key;
    x %= 26;
    printf("%c", x + 'A');
}

void cipher_az(char x, int key)
{
    x -= 'a';
    x += key;
    x %= 26;
    printf("%c", x + 'a');
}
