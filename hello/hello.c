#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Gets user name
    string name = get_string("Name: ");

    // Prints greetings
    printf("hello, %s\n", name);
}