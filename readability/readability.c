#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Get text from user
    string text = get_string("Text: ");

    // Count number of letters, words, and sentences
    int text_letters = count_letters(text);
    int text_words = count_words(text);
    int text_sentences = count_sentences(text);

    // Calculate Coleman-Liau index
    double L = (double) text_letters / text_words * 100;
    double S = (double) text_sentences / text_words * 100;
    int index = (int) round(0.0588 * L - 0.296 * S - 15.8);

    // Prints result
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

// Function for counting letters
// Any lowercase character from a to z or any uppercase character from A to Z counts as a letter
int count_letters(string text)
{
    int text_size = strlen(text);
    int text_letters = 0;
    for (int i = 0; i < text_size; i++)
    {
        if (isupper(text[i]) || islower(text[i]))
        {
            text_letters = text_letters + 1;
        }
    }
    return text_letters;
}

// Function for counting words
// Any sequence of characters separated by spaces counts as a word
int count_words(string text)
{
    int text_size = strlen(text);
    int text_words = 1;
    for (int i = 0; i < text_size; i++)
    {
        if (text[i] == 32)
        {
            text_words = text_words + 1;
        }
    }
    return text_words;
}

// Function for counting sentences
// Any occurrence of a period, exclamation point, or question mark indicates the end of a sentence
int count_sentences(string text)
{
    int text_size = strlen(text);
    int text_sentences = 0;
    for (int i = 0; i < text_size; i++)
    {
        if (text[i] == 33 || text[i] == 46 || text[i] == 63)
        {
            text_sentences = text_sentences + 1;
        }
    }
    return text_sentences;
}