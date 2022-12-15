#include <stdio.h>
#include <cs50.h>

void space(int num_space);
void hash(int num_hash);

int main(void)
{
    // Ask user for height
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Build the pyramid
    for (int i = 0; i < height; i++)
    {
        // For each row, print series of spaces and hashes:
        space(height - (i + 1));
        hash(i + 1);
        printf("  ");
        hash(i + 1);
        printf("\n");
    }
}

// Function for printing spaces
void space(int num_space)
{
    for (int j = 0; j < num_space; j++)
    {
        printf(" ");
    }
}

// Function for printing hashes
void hash(int num_hash)
{
    for (int j = 0; j < num_hash; j++)
    {
        printf("#");
    }
}