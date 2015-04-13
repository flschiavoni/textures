#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char ** argv){

   if(argc < 4){
      printf("Usage:\n %s <file_name> \
         width bits_per_pixel\n", argv[0]);
      printf("      bits per pixel can be 1, 4, 8, or 24\n");
      exit(0);
   }

   int width = atoi(argv[2]);
   int bpp = atoi(argv[3]);
   int height = 0;

   char * filename = malloc(strlen(argv[1]));
   strcpy(filename, argv[1]);

   struct stat st;
   stat(filename, &st);
   uint32_t size = st.st_size;
   printf("filesize: %d\n", size);

   height = size / bpp / width;

   // Read file data
   char * buffer = 0;
   FILE * f = fopen (filename, "rb");
   buffer = malloc(size);
   fread (buffer, 1, size, f);
   fclose (f);

   // New file
   char * newfilename = malloc(strlen(argv[1] + 4));
   strcpy(newfilename, filename);
   strcat(newfilename, ".bmp");
   FILE *fp = fopen(newfilename,"wb+");

   fwrite("BM", 1, 2, fp); //signature, must be 4D42 hex
   int newsize = size + 54; // file size
   fwrite(&newsize, 1, 4, fp);

   int flag = 0; // reserved: Must be zero
   fwrite(&flag, 1, 2, fp);
   fwrite(&flag, 1, 2, fp);
   flag = 54; // offset to start the image
   fwrite(&flag, 1, 4, fp);
   flag = 40; // size of BITMAPINFOHEADER structure, must be 40
   fwrite(&flag, 1, 4, fp);
   flag = width; // image width in pixels
   fwrite(&flag, 1, 4, fp);
   flag = height; // image height in pixels
   fwrite(&flag, 1, 4, fp);
   flag = 1; // number of planes in the image, must be 1
   fwrite(&flag, 1, 2, fp);
   flag = bpp; // number of bits per pixel (1, 4, 8, or 24)
   fwrite(&flag, 1, 2, fp);
   flag = 0; // compression type (0=none, 1=RLE-8, 2=RLE-4)
   fwrite(&flag, 1, 4, fp);
   flag = size; // size of image data in bytes (including padding)
   fwrite(&size, 1, 4, fp);
   flag = 0; // horizontal resolution in pixels per meter (unreliable)
   fwrite(&flag, 1, 4, fp);
   flag = 0; // vertical resolution in pixels per meter (unreliable)
   fwrite(&flag, 1, 4, fp);
   flag = 0; // number of colors in image, or zero
   fwrite(&flag, 1, 4, fp);
   flag = 0; // number of important colors, or zero
   fwrite(&flag, 1, 4, fp);


   fwrite(buffer, 1, size, fp);
   fclose(fp);

}
