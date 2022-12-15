#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // check if argument is provided
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // check if source is valid
    FILE* input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // define byte
    typedef uint8_t BYTE;

    // define needed counters and constants
    int BLOCK_SIZE = 512;
    int count = 0;
    char* filename;
    FILE* image;

    // create buffer
    BYTE buffer[BLOCK_SIZE];

    // read files from card.raw
    while (fread(buffer, 1, BLOCK_SIZE, input) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (count != 0)
            {
                // close
                fclose(image);
            }

            // create the filename
            filename = malloc(BLOCK_SIZE);
            sprintf(filename, "%03i.jpg", count);
            printf("filename: %s", filename);
            count++;
            free(filename);

            // create jpg file with write permission
            image = fopen(filename, "w");
        }
        // write data in jpg file
        fwrite(&buffer, 1, BLOCK_SIZE, image);
    }
    // close
    fclose(image);
}