from cs50 import get_int


def main():
    # Ask user for height
    while True:
        height = get_int("Height: ")
        if height > 0 and height < 9:
            break

    # Build pyramid
    for i in range(height):
        # For each row, print series of spaces and hashes
        space(height - (i + 1))
        hash(i + 1)
        print("  ", end="")
        hash(i + 1)
        print("")


# Function for printing spaces
def space(num_space):
    for i in range(num_space):
        print(" ", end="")


# Function for printing hashes
def hash(num_hash):
    for i in range(num_hash):
        print("#", end="")


main()