#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // checks correct usage
    if (argc != 4)
    {
        fprintf(stderr, "Correct usage : ./resize [f] infile outfile");
        return 1;
    }
    
    // indexes the commandline arguments
    char *res_val = *(argv + 1);
    char *in_file = *(argv + 2);
    char *out_file = *(argv + 3);
    
    // determins the resize factor f
    double f = atof(res_val);
    
    // opens the input file
    FILE *pIn_file = fopen(in_file, "r");
    if (pIn_file == NULL)
    {
        fprintf(stderr, "Error! Cannot open %s", in_file);
        return 2;
    }
    
    // opens the output file
    FILE *pOut_file = fopen(out_file, "w");
    if (pOut_file == NULL)
    {
        fprintf(stderr, "Error! Cannot open %s", out_file);
        return 3;
    }
    
    
    
    // reads BITMAPFILEHEADER from input file
    BITMAPFILEHEADER bmp_FH;
    fread(&bmp_FH, sizeof(BITMAPFILEHEADER), 1, pIn_file);
    
    // reads BITMAPINFOHEADER from output file
    BITMAPINFOHEADER bmp_IH;
    fread(&bmp_IH, sizeof(BITMAPINFOHEADER), 1, pIn_file);
    
    // ensures that the file is a 24-bit bmp file
    if (bmp_FH.bfType != 0x4d42 || bmp_FH.bfOffBits != 54 || bmp_IH.biSize != 40
        || bmp_IH.biBitCount != 24 || bmp_IH.biCompression != 0)
    {
        fclose(pIn_file);
        fclose(pOut_file);
        fprintf(stderr, "Error! The file format is not supported");
        return 4;
    }

    // stores required data for resizing
    LONG in_width = bmp_IH.biWidth;
    int in_padding = (4 - (bmp_IH.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int scanl_size = (sizeof(RGBTRIPLE) * in_width) + in_padding;
    
    
    // updates the structures to write in output file
    bmp_IH.biWidth *= f;
    bmp_IH.biHeight *= f;
    
    int out_padding = (4 - (bmp_IH.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bmp_IH.biSizeImage = ((sizeof(RGBTRIPLE) * bmp_IH.biWidth) + out_padding)
                            * abs(bmp_IH.biHeight);
                            
    bmp_FH.bfSize = (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
                            + bmp_IH.biSizeImage);
    
    // writes BITMAPFILEHEADER into outfile
    fwrite(&bmp_FH, sizeof(BITMAPFILEHEADER), 1, pOut_file);
    
    // writes BITMAPINFOHEADER into outfile
    fwrite(&bmp_IH, sizeof(BITMAPINFOHEADER), 1, pOut_file);
    
    // some arrays to help with resizing
    
    // pxl_c represents the pixel pattern to be written
    int pxl_c [bmp_IH.biWidth];
    for (int i = 0; i < bmp_IH.biWidth; i++)
    {
        // set to some minor decimal value to prevent any typecasting error
        static double j = 0.00001;
        
        int x = (int) j;
        pxl_c[i] = x;
        j = j + (1.0 / f);
        
    }
    
    // scanl_c represents the scanline pattern to be written
    int scanl_c [abs(bmp_IH.biHeight)];
    for (int i = 0, n = abs(bmp_IH.biHeight); i < n; i++)
    {
        // set to some minor decimal value to prevent any typecasting error
        static double j = 0.00001;
        
        int x = (int) j;
        scanl_c[i] = x;
        j = j + (1.0 / f);
    }
    
    
    // resizes and writes to outfile
    for (int i = 0, n = abs(bmp_IH.biHeight); i < n; i++)
    {
        // seeks to scanl[i] th row of infile
        fseek(pIn_file, bmp_FH.bfOffBits + (scanl_size * scanl_c[i]), SEEK_SET);
        
        // reads the entire scanline (excluding padding)
        RGBTRIPLE *PIX = malloc (sizeof(RGBTRIPLE) * in_width);
        
        fread(PIX, sizeof(RGBTRIPLE) * in_width, 1, pIn_file);
        
        // writes to outfile with help of pxl_c (excluding padding)
        for(int j = 0; j < bmp_IH.biWidth; j++)
        {
            fwrite(&PIX[pxl_c[j]], sizeof(RGBTRIPLE), 1, pOut_file);
        }
        
        // adds padding to outfile
        for (int j = 0; j < out_padding; j++)
        {
            fputc(0x00, pOut_file);
        }
        
        // frees the allocated memory
        free(PIX);
    }
    
    // closes input and output file
    fclose(pIn_file);
    fclose(pOut_file);
    
    // success 
    return 0;
}