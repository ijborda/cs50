#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    // Ask for credit card number
    long long cc_number = get_long("Number: ");

    // Get number of digits of credit card number
    long long cc_size = floor(log10(cc_number)) + 1;

    // Get digits of credit card number
    long long cc_digits[cc_size];
    long long cc_number_adj = cc_number;
    for (int i = 0; i < cc_size; i++)
    {
        long long place_value = pow(10, cc_size - 1 - i);
        cc_digits[i] = (cc_number_adj - (cc_number_adj % place_value)) / place_value;
        cc_number_adj = cc_number_adj - (cc_digits[i] * place_value);
    }

    // Get sum from Luhn's algorithm
    int cc_luhn_sum = 0;
    for (int i = 0; i < cc_size; i++)
    {
        // If digit position is not in "even from the last"
        if ((cc_size - 1 - i) % 2 == 0)
        {
            cc_luhn_sum = cc_luhn_sum + cc_digits[i];
        }
        else
        {
            // If digit position is in "even from the last" and number of digits of its double is 2
            if (floor(log10(2 * cc_digits[i])) + 1 == 2)
            {
                int ones_digit = 2 * cc_digits[i] % 10;
                int tens_digit = (2 * cc_digits[i] - ones_digit) / 10;
                cc_luhn_sum = cc_luhn_sum + ones_digit + tens_digit;
            }
            // If digit position is in "even from the last" and number of digits of its double is 1
            else
            {
                cc_luhn_sum = cc_luhn_sum + 2 * cc_digits[i];
            }
        }
    }

    // Determine credit card type or if invalid
    // Criteria: Luhn algorithm, size of credit card number, and beginning digits
    if (cc_luhn_sum % 10 == 0 && cc_size == 15 && cc_digits[0] == 3 && (cc_digits[1] == 4 || cc_digits[1] == 7))
    {
        printf("AMEX\n");
    }
    else if (cc_luhn_sum % 10 == 0 && cc_size == 16 && cc_digits[0] == 5 &&
             (cc_digits[1] == 1 || cc_digits[1] == 2 || cc_digits[1] == 3 || cc_digits[1] == 4 || cc_digits[1] == 5))
    {
        printf("MASTERCARD\n");
    }
    else if (cc_luhn_sum % 10 == 0 && (cc_size == 13 || cc_size == 16) && cc_digits[0] == 4)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}