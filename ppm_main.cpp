#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "images.h"




//============================================================================
//                                  Main
//============================================================================
int main(int argc, char* argv[])
{

  //--------------------------------------------------------------------------
  // Read file "gargouille.ppm" into image (width and height)
  //--------------------------------------------------------------------------
  images image1;
  char fichier1[] = "gargouille.ppm";
  image1.SetName(fichier1);
  image1.ppm_read_from_file();

  //--------------------------------------------------------------------------
  // Create a desaturated (B&W) copy of the image we've just read and
  // write it into "gargouille_BW.ppm"
  //--------------------------------------------------------------------------
  // Copy image into image_bw
  char fichier2[]= "gargouille_BW.ppm";
  images imagebw(image1); 
  imagebw.SetName(fichier2);
  memcpy(imagebw.GetData(), image1.GetData(), 3 *imagebw.GetW() *imagebw.GetH() * sizeof(*imagebw.GetData()));

  // Desaturate image_bw
  imagebw.ppm_desaturate();

  // Write the desaturated image into "gargouille_BW.ppm"
  imagebw.ppm_write_to_file();
 
 
  //--------------------------------------------------------------------------
  // Create a resized copy of the image and
  // write it into "gargouille_small.ppm"
  //--------------------------------------------------------------------------
  // Copy image into image_small 
  images image_small(image1);
  char fichier3[]= "gargouille_small.ppm";
  image_small.SetName(fichier3);

  memcpy(image_small.GetData(), image1.GetData(), 3 * image_small.GetW() *image_small.GetH() * sizeof(*image_small.GetData()));
  
  // Shrink image_small size 2-fold
   image_small.ppm_shrink(2);

   // Write the desaturated image into "gargouille_small.ppm"
  image_small.ppm_write_to_file();

   return 0;
}

