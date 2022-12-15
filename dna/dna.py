import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: dana.py database sequence")
        sys.exit(1)

    # TODO: Read database file into a variable
    database = []
    with open(sys.argv[1], "r") as file_database:
        reader = csv.DictReader(file_database)
        for name in reader:
            database.append(name)

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as file_dna:
        dna = file_dna.read()

    # TODO: Find longest match of each STR in DNA sequence
    # Obtain a list of STRs
    list_str = list(database[0].keys())
    list_str.pop(0)  # remove 'name' key
    # Compute repeats of each STR
    dna_str = {}
    for i in range(len(list_str)):
        dna_str[list_str[i]] = longest_match(dna, list_str[i])

    # TODO: Check database for matching profiles
    for j in range(len(database)):
        check = 0
        for k in range(len(list_str)):
            if int(database[j][list_str[k]]) == dna_str[list_str[k]]:
                check += 1
            if check == len(list_str):
                print(database[j]["name"])
                return
    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
