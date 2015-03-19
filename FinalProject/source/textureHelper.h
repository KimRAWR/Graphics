/*data structure for the image used for  texture mapping */
typedef struct Image {
  unsigned long sizeX;
  unsigned long sizeY;
  char *data;
} Image;


GLvoid LoadTexture(char* image_file, int texID);
static unsigned int getint(FILE *fp);
static unsigned int getshort(FILE *fp);
int ImageLoad(char *filename, Image *image);