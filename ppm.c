#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct images
{
  int w;
  int h;
  char* name;
  u_char* dat;
};
  

//============================================================================
//                           Function declarations
//============================================================================
// Write the image contained in <data> (of size <width> * <height>)
// into plain RGB ppm file <file>
void ppm_write_to_file(struct images image);

// Read the image contained in plain RGB ppm file <file>
// into <data> and set <width> and <height> accordingly
// Warning: data is malloc_ed, don't forget to free it
void ppm_read_from_file(struct images image);

// Desaturate (transform to B&W) <image> (of size <width> * <height>)
void ppm_desaturate(struct images image);

// Shrink image (of original size <width> * <height>) by factor <factor>
// <width> and <height> are updated accordingly
void ppm_shrink(struct images image,int factor);



//============================================================================
//                                  Main
//============================================================================
int main(int argc, char* argv[])
{

  //--------------------------------------------------------------------------
  // Read file "gargouille.ppm" into image (width and height)
  //--------------------------------------------------------------------------

  struct images image1;

  u_char* image = NULL;
  int width;
  int height;
  char fichier1[] = "gargouille.ppm";

  image1.w=width;
  image1.h=height;
  image1.name=fichier1;
  image1.dat=image;
  //u_char* imagedat = (u_char*) malloc(3 *image1.w *image1.h * sizeof(u_char));


  ppm_read_from_file(image1);
 


  //--------------------------------------------------------------------------
  // Create a desaturated (B&W) copy of the image we've just read and
  // write it into "gargouille_BW.ppm"
  //--------------------------------------------------------------------------
  // Copy image into image_bw
 
 
  char fichier2[]= "gargouille_BW.ppm";

  struct images imagebw; 
  imagebw.w=width;
  imagebw.h=height;
  u_char* image_bw = (u_char*) malloc(3 *imagebw.w *imagebw.h * sizeof(u_char));
  imagebw.dat=image_bw;
  imagebw.name=fichier2;

  memcpy(imagebw.dat, image, 3 *imagebw.w *imagebw.h * sizeof(u_char));

  // Desaturate image_bw
  ppm_desaturate(imagebw);

  // Write the desaturated image into "gargouille_BW.ppm"
  
  ppm_write_to_file(imagebw);
 

  // Free the desaturated image
  free(imagebw.dat);


  //--------------------------------------------------------------------------
  // Create a resized copy of the image and
  // write it into "gargouille_small.ppm"
  //--------------------------------------------------------------------------
  // Copy image into image_small
  
  struct images image_small;
  image_small.w = width;
  image_small.h = height;
  u_char* imagesmall = (u_char*) malloc(3 * width * height* sizeof(*imagesmall));
  image_small.dat=imagesmall;
     memcpy(image_small.dat, image, 3 * image_small.w *image_small.h * sizeof(*image_small.dat));

  // Shrink image_small size 2-fold
     ppm_shrink(image_small,2);

  // Write the desaturated image into "gargouille_small.ppm"
  char fichier3[]= "gargouille_small.ppm";
  ppm_write_to_file(image_small);


  // Free the not yet freed images
  free(image);
  free(image_small.dat);

  return 0;
}



//============================================================================
//                           Function declarations
//============================================================================
void ppm_write_to_file(struct images image)
{

  FILE* ppm_output = fopen(image.name, "wb");

  // Write header
  fprintf(ppm_output, "P6\n%d %d\n255\n", image.w,image.h);

  // Write pixels
  fwrite(image.dat, 3, image.w*image.h, ppm_output);

   fclose(ppm_output);
}

void ppm_read_from_file(struct images image)
{

  
  FILE* ppm_input = fopen(image.name, "rb");
  
  // Read file header
  fscanf(ppm_input, "P6\n%d %d\n255\n", &image.w, &image.h);

  // Allocate memory according to width and height
  image.dat = (u_char*) malloc(3 * image.w * image.h * sizeof(u_char));

  // Read the actual image data
  fread(image.dat, 3, image.w * image.h, ppm_input);

  fclose(ppm_input);
}

void ppm_desaturate(struct images image)
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

  void ppm_shrink(struct images image, int factor)
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


/*struct images createimages(int wdth, int hght, u_char* data, char* name_file, struct images image)
{
  image.w=wdth;
  image.h=hght;
  image.dat=(u_char*)malloc(3*wdth*hght*sizeof(u_char));
  image.name=name_file;
}
*/

  



































