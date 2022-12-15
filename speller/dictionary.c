// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <cs50.h>
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 143093;
int num_words = 0;

// Hash table
node *table[N];

// Declare pointer to file globally


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // traverse the linked list in that hash
    node *cursor = table[hash(word)];
    int index = hash(word);
    while (true)
    {
        if (table[hash(word)] == NULL)
        {
            return false;
        }
        else if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        else if (cursor->next == NULL)
        {
            return false;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    // return false if word is not found after traversing all list
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // initialize sum and index
    int sum = 0;
    int i = 0;
    // get the sum of word's ASCII number
    do
    {
        sum += pow(tolower(word[i]), 2);
        i++;
    }
    while (word[i] != '\0');
    // calculate the hash value
    int hash_value = sum;
    for (int j = 0; j < 5; j++)
    {
        hash_value = (hash_value * 2463 + 321) % N;
    }
    // return hash value
    return hash_value;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // open dictionary with fopen and save it in dict
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Could not open file.\n");
        return false;
    }
    // read strings from file
    char new_word[LENGTH];
    while (fscanf(dict, "%s", new_word) != EOF)
    {
        // create a new node
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            printf("Cannot allocate memory.\n");
            return 1;
        }
        // save new word in the new node
        strcpy(new_node->word, new_word);
        new_node->next = NULL;

        // determine which bucket to put the node
        int index = hash(new_word);

        // insert node to the hash table
        new_node->next = table[index];
        table[index] = new_node;

        // count words
        num_words++;
    }
    // return true if all is well
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return num_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        // skipn hashes that have no linked list
        if (table[i] != NULL)
        {
            // create cursor and tmp
            node *tmp = table[i];
            node *cursor = table[i];
            while (true)
            {
                // if there is no next node, free current node and exit loop
                if (cursor->next == NULL)
                {
                    free(tmp);
                    break;
                }
                // if there is a next node, free current node and proceed to the next node
                else
                {
                    cursor = cursor->next;
                    free(tmp);
                    tmp = cursor;
                }
            }
        }
    }
    return true;
}