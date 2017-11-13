#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sndfile.h>

#include "medusa_dsp.h"
#include "medusa_types.h"

FILE * create_file(const char * file_name){
   // New file
   char * newfilename = malloc(strlen(file_name + 4));
   strcpy(newfilename, file_name);
   strcat(newfilename, ".bmp");
   return fopen(newfilename,"wb+");
}

void create_file_with_data(const char * file_name, void * buffer, int size){
   // New file
   char * newfilename = malloc(strlen(file_name + 4));
   strcpy(newfilename, file_name);
   strcat(newfilename, ".bmp");
   FILE * fp = fopen(newfilename,"wb+");
    fwrite(buffer, 1, size, fp);
    fclose(fp);
}



int main(int argc, char ** argv){

   if(argc < 3){
      printf("Usage:\n %s <file_name> height\n", argv[0]);
      exit(0);
   }

    SNDFILE *sf;
    SF_INFO info;
    sf = sf_open(argv[1], SFM_READ, &info);
    if(!sf) {
    printf("Not able to open input file %s.\n", argv[1]);
    puts(sf_strerror(NULL));
    return 0;
  }

    // File info
    printf("Frames in file: %ld frames (%fs)\n", info.frames, ((float)info.frames) / ((float)info.samplerate));
    printf("Sample rate: %d\n", info.samplerate);
    printf("Channels: %d\n", info.channels);

    // Read audio file
    float * audio;
    audio = malloc(info.channels * info.frames * sizeof(float));
    int read = sf_readf_float(sf, audio, info.frames); //return size in frames

    printf("Read size: %d frames (%fs)\n", read, ((float)read) / (float) info.samplerate);

    // Convert to mono
    float * audiomono = malloc(info.frames * sizeof(float));
    if(info.channels == 2){
        for(int i = 0 ; i < info.frames ; i++){
            audiomono[i] = (audio[2 * i] + audio[2 * i + 1]) /2;
        }
    }else{ // If it is mono, just copy to dest buffer
        for(int i = 0 ; i < info.frames ; i++){
            audiomono[i] = audio[i];
        }
    }

//    create_file_with_data(argv[1], audiomono, info.frames * sizeof(float));


    // Convert to 8Khz sample rate
    float * audio8k = malloc(info.frames * sizeof(float)); //dunno the size
    read = medusa_dsp_change_sample_rate((float)info.samplerate,
                    8000.0,
                    audiomono,
                    audio8k,
                    info.frames * sizeof(float));
    printf("Size (pos 8khz conversion): %d frames (%fs)\n", read, ((float)read) / 8000.0);

//    create_file_with_data(argv[1], audio8k, read);

    // Convert to 8 bits
    char * audio8bits = malloc(read * sizeof(char));
    read = medusa_dsp_change_quantization(MEDUSA_32_BITS, MEDUSA_8_BITS,
                audio8k,
                audio8bits,
                read);
    printf("Size (pos 8bits conversion): %d frames (%fs)\n", read, ((float)read) / 8000.0);

    unsigned char * audio8bitsunsigned = malloc(read * sizeof(char));
    for(int i = 0 ; i < read ; i++)
        audio8bitsunsigned[i] = audio8bits[i] + 128;

//    create_file_with_data(argv[1], audio8bitsunsigned, read);

    int height = atoi(argv[2]);
    int width = read / 3;
    int bpp = 24; // bits per pixels
    int size = height * read;

    printf("Image size (%d,%d) %d \n", width, height, size);

   // Read file data
   char * buffer = 0;
   buffer = malloc(height * read);
   for(int i = 0 ; i < height ; i++){
        memcpy(&buffer[i * read], audio8bitsunsigned, read);
   }

    FILE * fp = create_file(argv[1]);

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
