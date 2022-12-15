def main():
    # Get text from user
    text = input("Text: ")

    # Count number of letters, words, and sentences
    text_letters = count_letters(text)
    text_words = count_words(text)
    text_sentences = count_sentences(text)

    # Calculate Coleman-Liau index
    L = text_letters / text_words * 100
    S = text_sentences / text_words * 100
    index = round(0.0588 * L - 0.296 * S - 15.8)

    # Prints result
    if index >= 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


# Function for counting letters
# Any lowercase character from a to z or any uppercase character from A to Z counts as a letter
def count_letters(text):
    text_size = len(text)
    text_letters = 0
    for i in range(text_size):
        if text[i].isalpha():
            text_letters += 1
    return text_letters


# Function for counting words
# Any sequence of characters separated by spaces counts as a word
def count_words(text):
    text_size = len(text)
    text_words = 1
    for i in range(text_size):
        if ord(text[i]) == 32:
            text_words += 1
    return text_words


# Function for counting sentences
# Any occurrence of a period, exclamation point, or question mark indicates the end of a sentence
def count_sentences(text):
    text_size = len(text)
    text_sentences = 0
    for i in range(text_size):
        if ord(text[i]) in [33, 46, 63]:
            text_sentences += 1
    return text_sentences


main()