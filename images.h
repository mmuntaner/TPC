
class images
{
 private:

  int w;
  int h;
  char* name;
  u_char* dat;



public:

// =======================================================================
//                               Constructors
// =======================================================================
  images(void);
  images(const images& newImage);

// =======================================================================
//                               Destructor
// =======================================================================
  ~images(void); 

// =======================================================================
//                             Accessors: getters
// =======================================================================
 int GetH(void) const;
 int GetW(void) const;
 u_char* GetData(void) const;

// =======================================================================
//                               Accessors: setters
// =======================================================================

 int SetH(int newH);
 int SetW(int newW);
 char* SetName( char *newName); 
 u_char* SetDat( u_char *newDat); 
 

// =======================================================================
//                              Public Methods
// =======================================================================

// Write the image contained in <data> (of size <width> * <height>)
// into plain RGB ppm file <file>
void ppm_write_to_file(void);

// Read the image contained in plain RGB ppm file <file>
// into <data> and set <width> and <height> accordingly
// Warning: data is malloc_ed, don't forget to free it
void ppm_read_from_file(void);

// Desaturate (transform to B&W) <image> (of size <width> * <height>)
void ppm_desaturate(void);

// Shrink image (of original size <width> * <height>) by factor <factor>
// <width> and <height> are updated accordingly
void ppm_shrink(int factor);

};
