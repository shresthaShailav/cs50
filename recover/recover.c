#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    // checks correct usage
    if (argc != 2)
    {
        fprintf(stderr, "Correct usage : ./recover <image file>");
        return 1;
    }
    
    // indexes image file
    char *image = *(argv + 1);
    
    // opens image file for reading
    FILE *img_file = fopen(image, "r");
    if (img_file == NULL)
    {
        fprintf(stderr, "Error! Cannot open %s", image);
        return 2;
    }
    
    // variables for 
    uint8_t *data = malloc(512);
    int file_count = 0;
    char *file_name = malloc(15);
    bool JPEG_found = false;
    FILE *out_file;
    
    // reads blocks (512 bytes) continuously until EOF is reached
    while (fread(data, 1, 512, img_file) == 512)
    {
        // checks if the first 4 bytes corelates with the JPEG format
        if ((*(data) == 0xff) && (*(data + 1) == 0xd8) && (*(data + 2) == 0xff)
            && ((*(data + 3) & 0xf0) == 0xe0))
        {
            // closes any previous opened JPEG
            if (JPEG_found)
            {
                fclose(out_file);
            }

            // creates a new file to store the JPEG
            sprintf(file_name, "%03d.jpg", file_count++);
            out_file = fopen(file_name, "w");

            // writes the block read into the file
            fwrite(data, 512, 1, out_file);

            // flag for next iteration
            JPEG_found = true;

        }
        
        else
        {
            // checks if JPEG file has already been found
            if (JPEG_found)
            {
                // writes the block into the output file
                fwrite(data, 512, 1, out_file);
            }
        }
        
        
    }
    
    fclose(img_file);
    free(data);
    
    
    
}
