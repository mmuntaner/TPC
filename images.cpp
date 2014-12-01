#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "images.h"


// =======================================================================
//                               Constructors
// =======================================================================

images::images(void)
{
  h=0; 
  w=0; 
  name=NULL;
  dat=new u_char [1];
}
images::images(const images& newImage)
{
  h=newImage.h;
  w=newImage.w;
  name=newImage.name;
  dat=new u_char[3*h*w];
}

// =======================================================================
//                                Destructor
// =======================================================================

images::~images(void)
{
  delete [] dat;
  dat=NULL;
}

//============================================================================
//                            Getters' definitions
//============================================================================

int images::GetH(void) const
{ 
  return h;
} 

int images::GetW(void) const
{
  return w;
}


 u_char* images::GetData(void) const
{
  return dat;
}

//============================================================================
//                            Setters' definitions
//============================================================================

int images::SetH(int newH)
{
  h=newH;
}

int images::SetW(int newW)
{
  w=newW;
} 

char* images::SetName( char *newName)
 {
   name=newName;
 }

u_char* images::SetDat( u_char *newDat)
{
  dat=newDat;
}


//============================================================================
//                           Function definitions
//============================================================================

void images::ppm_write_to_file(void)
{

  FILE* ppm_output = fopen(name, "wb");

  // Write header
  fprintf(ppm_output, "P6\n%d %d\n255\n", w,h);

  // Write pixels
  fwrite(dat, 3, w * h, ppm_output);

   fclose(ppm_output);
}

void images::ppm_read_from_file(void)
{

  
  FILE* ppm_input = fopen(name, "rb");
  
  // Read file header
  fscanf(ppm_input, "P6\n%d %d\n255\n",&w, &h);
 
  

  // Allocate memory according to width and height
  dat = new u_char[3 * w * h];

  // Read the actual image data
  fread(dat, 3, w * h, ppm_input);

  fclose(ppm_input);
}

void images::ppm_desaturate(void)
{
  int x, y;

  // For each pixel ...
  for (x = 0 ; x <w ; x++)
  {
    for (y = 0 ; y < h ; y++)
    {
      u_int grey_lvl = 0;
      int rgb_canal;

      // Compute the grey level
      for (rgb_canal = 0 ; rgb_canal < 3 ; rgb_canal++)
      {
        grey_lvl += dat[ 3 *( y * w + x) + rgb_canal ];
      }
      grey_lvl /= 3;
      assert(grey_lvl >= 0 && grey_lvl <=255);

      // Set the corresponding pixel's value in new_image
      memset(&dat[3 * (y *  w + x)], grey_lvl, 3);
    }
  }
}

void images:: ppm_shrink(int factor)
{
  // Compute new image size and allocate memory for the new image
  int new_width   = w / factor;
  int new_height  = h / factor;
  u_char* new_image = new u_char[3 * new_width * new_height];

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
      int i0 = 3 * (y0 * w + x0);

      // Compute RGB values for the new pixel
      int dx, dy;
      for (dx = 0 ; dx < factor ; dx++)
      {
        for (dy = 0 ; dy < factor ; dy++)
        {
          // Compute the offset of the current pixel (in the model image)
          // with regard to the top-left pixel of the current "set of pixels"
          int delta_i = 3 * (dy * w + dx);

          // Accumulate RGB values
          red   += dat[i0+delta_i];
          green += dat[i0+delta_i+1];
          blue  += dat[i0+delta_i+2];
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
  w = new_width;
  h = new_height;

  // Update image
  delete(dat);
  dat = new_image;
}












  
