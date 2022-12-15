// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    uint8_t header[HEADER_SIZE];    // prepare array for header file
    fread(header, sizeof(uint8_t), HEADER_SIZE, input);   // read header of input file and save it to header[] array
    fwrite(header, sizeof(uint8_t), HEADER_SIZE, output); // write content of header[] array to output file

    // TODO: Read samples from input file and write updated data to output file
    int16_t buffer; // prepare temporary varibale (2 bytes) to strore each sample from the input file
    while (fread(&buffer, sizeof(int16_t), 1, input)) // read samples of input file and save it to buffer
    {
        buffer = factor * buffer; // adjust volume by multiplying factor to each sample
        fwrite((&buffer), sizeof(int16_t), 1, output); // save the new value from buffer to output
    }

    // Close files
    fclose(input);
    fclose(output);
}