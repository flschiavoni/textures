#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

static const int BMP_HEADER_SIZE = 54;

int main(int argc, char ** argv){

   if(argc < 4){
      printf("Usage:\n %s <file_name> sample_rate bit_depth\n", argv[0]);
      exit(0);
   }

   int sample_rate = atoi(argv[2]);
   int bit_depth = atoi(argv[3]);

   int n_chars = strlen(argv[1]) + 1;
   char * filename = malloc(n_chars * sizeof(char));
   strcpy(filename, argv[1]);
   filename[n_chars - 1] = '\0';

   struct stat st;
   stat(filename, &st);
   uint32_t size = st.st_size - BMP_HEADER_SIZE;
   printf("filesize: %d\n", size);

   FILE * f = fopen (filename, "rb");

   // Chomp file header
   char * header = 0;
   header = malloc(BMP_HEADER_SIZE * sizeof(char));
   fread (header, 1, BMP_HEADER_SIZE, f);

   // Read file data
   char * buffer = 0;
   buffer = malloc(size);
   fread (buffer, 1, size, f);

   fclose (f);

   char * newfilename = malloc((n_chars + 4) * sizeof(char));
   strcpy(newfilename, filename);
   strcat(newfilename, ".wav");
   newfilename[n_chars + 3] = '\0';
   FILE *fp = fopen(newfilename,"wb+");

   fwrite("RIFF", 1, 4, fp);
   int newsize = size + 32; // file size - 8
   fwrite(&newsize, 1, sizeof(newsize), fp);
   fwrite("WAVE", 1, 4, fp);

   fwrite("fmt ", 1, 4, fp);
   int flag = 16; // chunk size
   fwrite(&flag, 1, 4, fp);
   if(bit_depth == 32)
      flag = 3; // compression code (floating point)
   else
      flag = 1; // No compression
   fwrite(&flag, 1, 2, fp);

   flag = 1; // number of channels
   fwrite(&flag, 1, 2, fp);
   flag = sample_rate; // sample rate
   fwrite(&flag, 1, 4, fp);
   flag = flag * bit_depth / 8; // bytes per second
   fwrite(&flag, 1, 4, fp);
   flag = bit_depth / 8; // block align
   fwrite(&flag, 1, 2, fp);
   flag = bit_depth; // significant bits per sample
   fwrite(&flag, 1, 2, fp);

   fwrite("data", 1, 4, fp);//data segment
   fwrite(&size, 1, 4, fp);//data segment size

   fwrite(buffer, 1, size, fp);
   fclose(fp);

}
