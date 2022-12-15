#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Checks validity of command line argument: Reject if argument is not given, or if more than 1 is given
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Save key and it's size in a variable
    string key = argv[1];
    int key_size = strlen(key);

    // Checks validity of key: Reject key if it does not contain 26 characters
    if (key_size != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    for (int i = 0; i < key_size; i++)
    {
        // Checks validity of key: Reject key if it contains numerical characters
        if (isdigit(key[i]))
        {
            printf("Key must only contain alphabetic characters.\n");
            return 1;
        }

        // Checks validity of key: Reject key if it contains duplicate characters
        for (int j = 0; j < key_size; j++)
        {
            if (key[i] == key[j] && i != j)
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
        }
    }

    // Ask for plaintext and save in variable
    string plaintext = get_string("plaintext: ");

    // Save plaintext size in variable
    int plaintext_size = strlen(plaintext);

    // Prints ciphertext
    printf("ciphertext: ");
    for (int i = 0; i < plaintext_size; i++)
    {
        // If letter in plaintext is uppercase, do this:
        if (isupper(plaintext[i]))
        {
            printf("%c", toupper(key[(int) plaintext[i] - 'A']));
        }

        // If letter in plaintext is lowercase, do this:
        else if (islower(plaintext[i]))
        {
            printf("%c", tolower(key[(int) plaintext[i] - 'a']));
        }

        // For other characters, retain original letter in plaintext:
        else
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
    return 0;
}