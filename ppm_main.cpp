#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm_fonctions.h"




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

  image1.name=fichier1;
  image1.dat=NULL;

  ppm_read_from_file(image1);

 


  //--------------------------------------------------------------------------
  // Create a desaturated (B&W) copy of the image we've just read and
  // write it into "gargouille_BW.ppm"
  //--------------------------------------------------------------------------
  // Copy image into image_bw
 
 
  char fichier2[]= "gargouille_BW.ppm";

  images imagebw; 
  imagebw.w=image1.w;
  imagebw.h=image1.h;
  imagebw.dat = new u_char[3 *imagebw.w *imagebw.h];
  imagebw.name=fichier2;


  memcpy(imagebw.dat, image1.dat, 3 *imagebw.w *imagebw.h * sizeof(*imagebw.dat));

  // Desaturate image_bw
  ppm_desaturate(imagebw);

  // Write the desaturated image into "gargouille_BW.ppm"
  
  ppm_write_to_file(imagebw);
 

  // Free the desaturated image
  delete(imagebw.dat);


  //--------------------------------------------------------------------------
  // Create a resized copy of the image and
  // write it into "gargouille_small.ppm"
  //--------------------------------------------------------------------------
  // Copy image into image_small
  
  images image_small;
  image_small.w = image1.w;
  image_small.h = image1.h;
  image_small.dat= new u_char[3 *image_small.w *image_small.h];
  memcpy(image_small.dat, image1.dat, 3 * image_small.w *image_small.h * sizeof(*image_small.dat));

  // Shrink image_small size 2-fold
     ppm_shrink(image_small,2);
 
  // Write the desaturated image into "gargouille_small.ppm"
  char fichier3[]= "gargouille_small.ppm";
  image_small.name=fichier3;
  ppm_write_to_file(image_small);


  // Free the not yet freed images
  delete(image1.dat);


  return 0;
}

