from cs50 import get_string


# Ask for credit card number
cc_number = get_string("Number: ")

# Get number of digits of credit card number
cc_size = len(cc_number)

# Get digits of credit card number
cc_digits = []
for i in range(cc_size):
    cc_digits.append(int(cc_number[i]))

# Get sum from Luhn's algorithm
cc_luhn_sum = 0
for i in range(cc_size):
    # If digit position is not in "even from the last"
    if ((cc_size - 1 - i) % 2 == 0):
        cc_luhn_sum += cc_digits[i]
    else:
        # If digit position is in "even from the last" and number of digits of its double is 2
        if len(str(2 * cc_digits[i])) == 2:
            ones_digit = int(str(2 * cc_digits[i])[1])
            tens_digit = int(str(2 * cc_digits[i])[0])
            cc_luhn_sum += ones_digit + tens_digit
        # If digit position is in "even from the last" and number of digits of its double is 1
        else:
            cc_luhn_sum += 2 * cc_digits[i]

# Determine credit card type or if invalid
# Criteria: Luhn algorithm, size of credit card number, and beginning digits
if (cc_luhn_sum % 10 == 0) and (cc_size == 15) and (cc_digits[0] in [3, 4, 7]):
    print("AMEX")
elif (cc_luhn_sum % 10 == 0) and (cc_size == 16) and (cc_digits[0] == 5 and cc_digits[1] in [1, 2, 3, 4, 5]):
    print("MASTERCARD")
elif (cc_luhn_sum % 10 == 0) and (cc_size in [13, 16]) and (cc_digits[0] == 4):
    print("VISA")
else:
    print("INVALID")