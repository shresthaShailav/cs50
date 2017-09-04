#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // checks correct usage
    if (argc != 3)
    {
        fprintf(stderr, "Correct usage : ./whodunit infile outfile\n");
        return 1;
    }
    
    // indexes input and output file
    char *in_file = argv[1];
    char *out_file = argv[2];
    
    // opens input file
    FILE *in_ptr = fopen(in_file, "r");
    if (in_ptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", in_file);
        return 2;
    }
    
    // opens output file
    FILE *out_ptr = fopen(out_file, "w");
    if (out_ptr == NULL)
    {
        fclose(in_ptr);
        fprintf(stderr, "Could not open %s.\n", out_file);
        return 3;
    }
    
    // reads input files BITMAPFILEHEADER
    BITMAPFILEHEADER bmp_FH;
    fread(&bmp_FH, sizeof(BITMAPFILEHEADER), 1, in_ptr);
    
    // reads input file's BITMAPINFOHEADER
    BITMAPINFOHEADER bmp_IH;
    fread(&bmp_IH, sizeof(BITMAPINFOHEADER), 1, in_ptr);
    
    // ensures wether the file is a 24-bit bmp 4.0
    
    if (bmp_FH.bfType != 0x4D42 || bmp_FH.bfOffBits != 54 || 
        bmp_IH.biSize != 40 || bmp_IH.biBitCount != 24 || 
        bmp_IH.biCompression != 0)
    {
        fprintf(stderr, "The file format is not supported!\n");
        fclose(in_ptr);
        fclose(out_ptr);
        return 4;
    }
        
    // writes output file's BITMAPFILEHEADER
    fwrite(&bmp_FH, sizeof(BITMAPFILEHEADER), 1, out_ptr);
    
    // writes output file's BITMAPINFOHEADER
    fwrite(&bmp_IH, sizeof(BITMAPINFOHEADER), 1, out_ptr);
    
    // determines padding in the scanline (in Bytes)
    int padding = (4 - (bmp_IH.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // iterates over input files scanline
    for (int i = 0, height = abs(bmp_IH.biHeight); i < height; i++)
    {
        // iterates over each pixel in scanline
        for (int j = 0; j < bmp_IH.biWidth; j++)
        {
            // stores each pixel temporarily
            RGBTRIPLE pixel;
            
            // reads each pixel from the input file
            fread(&pixel, sizeof(RGBTRIPLE), 1, in_ptr);
            
           // converts purely red pixels into white pixels
            if (pixel.rgbtRed == 0xff && pixel.rgbtGreen == 0x00 && 
                pixel.rgbtBlue == 0x00)
            {
                pixel.rgbtGreen = 0xff;
                pixel.rgbtBlue = 0xff;
            }
            
            // converts other coloured pixels into black ones
            if (!(pixel.rgbtRed == 0xff && pixel.rgbtGreen == 0xff && 
                pixel.rgbtBlue == 0xff))
            {
                pixel.rgbtRed = 0x00;
                pixel.rgbtBlue = 0x00;
                pixel.rgbtGreen = 0x00;
            }
            
            
            // writes the pixel onto the output file
            fwrite(&pixel, sizeof(RGBTRIPLE), 1, out_ptr);
        }
        
        // skips over padding if any in input file
        fseek(in_ptr, padding, SEEK_CUR);
        
        // adds padding to the output file if any
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, out_ptr);
        }
    }
    
    // closes input file
    fclose(in_ptr);
    
    // closes output file
    fclose(out_ptr);
    
    // reports success
    return 0;
}