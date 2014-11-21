#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm_fonctions.h"



//============================================================================
//                           Function definitions
//============================================================================
void ppm_write_to_file(images image)
{

  FILE* ppm_output = fopen(image.name, "wb");

  // Write header
  fprintf(ppm_output, "P6\n%d %d\n255\n", image.w,image.h);

  // Write pixels
  fwrite(image.dat, 3, (image.w)*(image.h), ppm_output);

   fclose(ppm_output);
}

void ppm_read_from_file( images& image)
{

  
  FILE* ppm_input = fopen(image.name, "rb");
  
  // Read file header
  fscanf(ppm_input, "P6\n%d %d\n255\n",&image.w, &image.h);
 
  

  // Allocate memory according to width and height
  image.dat = (u_char*) malloc(3 * image.w * image.h * sizeof(u_char));

  // Read the actual image data
  fread(image.dat, 3, (image.w) * (image.h), ppm_input);

  fclose(ppm_input);
}

void ppm_desaturate(images image)
{
  int x, y;

  // For each pixel ...
  for (x = 0 ; x <image.w ; x++)
  {
    for (y = 0 ; y < image.h ; y++)
    {
      u_int grey_lvl = 0;
      int rgb_canal;

      // Compute the grey level
      for (rgb_canal = 0 ; rgb_canal < 3 ; rgb_canal++)
      {
        grey_lvl += image.dat[ 3 * (y *image.w + x) + rgb_canal ];
      }
      grey_lvl /= 3;
      assert(grey_lvl >= 0 && grey_lvl <=255);

      // Set the corresponding pixel's value in new_image
      memset(&image.dat[3 * (y *  image.w + x)], grey_lvl, 3);
    }
  }
}

  void ppm_shrink(images& image, int factor)
{
  // Compute new image size and allocate memory for the new image
  int new_width   = image.w / factor;
  int new_height  = image.h / factor;
  u_char* new_image = (u_char*) malloc(3 * new_width * new_height * sizeof(*new_image));

  // Precompute factor^2 (for performance reasons)
  int factor_squared = factor * factor;

  // For each pixel of the new image...
  int x, y;
  for (x = 0 ; x < new_width ; x++)
  {
    for (y = 0 ; y < new_height ; y++)
    {
      // ... compute the average RGB values of the set of pixels (a square of side factor)
      // that correspond to the pixel we are creating.

      // Initialize RGB values for the new image's pixel
      u_int red   = 0;
      u_int green = 0;
      u_int blue  = 0;

      // Compute coordinates and index of the first (top-left) pixel from the
      // model image corresponding to the pixel we are creating
      int x0 = x * factor;
      int y0 = y * factor;
      int i0 = 3 * (y0 * image.w + x0);

      // Compute RGB values for the new pixel
      int dx, dy;
      for (dx = 0 ; dx < factor ; dx++)
      {
        for (dy = 0 ; dy < factor ; dy++)
        {
          // Compute the offset of the current pixel (in the model image)
          // with regard to the top-left pixel of the current "set of pixels"
          int delta_i = 3 * (dy * image.w + dx);

          // Accumulate RGB values
          red   += image.dat[i0+delta_i];
          green += image.dat[i0+delta_i+1];
          blue  += image.dat[i0+delta_i+2];
        }
      }

      // Divide RGB values to get the mean values
      red   /= factor_squared;
      green /= factor_squared;
      blue  /= factor_squared;

      // Set new pixel's RGB values
      new_image[ 3 * (y * new_width + x) ]     = red;
      new_image[ 3 * (y * new_width + x) + 1 ] = green;
      new_image[ 3 * (y * new_width + x) + 2 ] = blue;
    }
  }

  // Update image size
  image.w = new_width;
  image.h = new_height;

  // Update image
  free(image.dat);
  image.dat = new_image;
}



  
