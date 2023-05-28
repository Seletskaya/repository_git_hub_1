#include <stdlib.h> 
#include <stdio.h> 
#include <math.h>
#include <strings.h> 
#include "lodepng.h"
 
int i,j; 

 char* loadPng(const char* filename, int*width, int*height) {
  unsigned char* image = NULL;
  int error = lodepng_decode32_file(&image, width, height, filename);
  if(error){
    printf("error %u: %s\n", error, lodepng_error_text(error));
 }
 return (image);
}

void writePng(const char* filename, const unsigned char* image, unsigned width, unsigned height) {
  unsigned error;
  unsigned char* png;
  size_t pngsize;
  LodePNGState state;
  lodepng_state_init(&state);
  error = lodepng_encode(&png, &pngsize,image,width, height, &state);
  if(!error){
    lodepng_save_file(png, pngsize,filename);
  }
  if(error){ 
    printf("error %u: %s\n", error,lodepng_error_text(error));
  }
  lodepng_state_cleanup(&state);
  free(png);
}



void preparation(unsigned char *old, int height, int width){ //делить изображение на ч/б/с
 for(i=0;i<height;i++) 
  for(j=0;j<width;j++){ 
    if(old[width*i+j]<100){ //bl
      old[width*i+j]=0;
    }
    if(old[width*i+j]>160){
      old[width*i+j]=255;
    }// white
  } 
 return; 
} 

void Gauss(unsigned char *old_ar, unsigned char *new_ar, int hight, int width){ 
int l;
  for (i=2;i<hight-1;i=i+1) 
    for(j=2;j<width-1;j=j+1){ 
      for(l=0;l<4;l++){
      //i*=4;
      //j*=4;
      new_ar[width*4*i+j*4]=0.12*old_ar[width*4*(i-1)+4*(j-1)+l]+0.12*old_ar[width*4*(i)+4*(j-1)+l]+0.12*old_ar[4*width*(i+1)+4*(j-1)+l]+ 0.12*old_ar[width*4*(i-1)+4*(j)+l]+0.12*old_ar[width*4*(i+1)+4*(j)+l]+ 0.12*old_ar[width*4*(i-1)+4*(j+1)+l]+0.12*old_ar[width*4*(i)+4*(j+1)+l]+0.12*old_ar[width*4*(i+1)+4*(j+1)+l];
      }
    } 
  return; 
} 
 
void colouring(unsigned char *ar_c, unsigned char *ar_n, int hight, int width, int n){ 
 for(i=1;i<width*hight;i++) { 
    ar_n[i*n]=ar_c[i]+1.5*ar_c[i-1]+204; //red
    ar_n[i*n+1]=180+ar_c[i]; // green
    ar_n[i*n+2]=200+ar_c[i]*2; //blue
    ar_n[i*n+3]=250; // прозрачность
 } 
 return; 
} 
void skull(unsigned char *ar_s,int width,int height,unsigned char *ar_c,unsigned char *ar_n,int n){
  preparation(ar_s,height,width); 
  printf("5\n");
  Gauss(ar_s, ar_c,height,width); 
  printf("6\n");
  colouring(ar_c, ar_n, height, width, n); //раскраска картинки 
  printf("7\n");
  char *outputPath="skull_2.png"; 
  printf("8\n");
  writePng(outputPath,ar_n, width, height);
  printf("9\n");
  return;
}
int main(void){ 
 char *inputPath ="skull_1.png"; 
  printf("1\n");
  int width,height;
  int n=4,k = 0; //3 базовых цвета+прозрачность
  char *idata=loadPng(inputPath, &width, &height); 
  if(idata==NULL){ 
     printf("ERROR: can't read file %s\n", inputPath); 
     return 1; 
  } 
  printf("2\n");
  unsigned char *ar_s=(unsigned char*)malloc(height*width*sizeof(unsigned char)),*ar_c=(unsigned char*)malloc(height*width*sizeof(unsigned char)),*ar_n=(unsigned char*)malloc(height*width*n*sizeof(unsigned char)); // start, cerrent,new ar
 printf("3\n");
  for(i=0;i<height*width; i++){ 
  ar_s[k]=0.299*idata[4*i]+0.587*idata[i*4+1]+0.114*idata[4*i+2]; 
    k++; 
  } 
  printf("4\n");
  skull(ar_s,width,height,ar_c,ar_n, n);

  free (ar_s);
  free (ar_c);
  free (ar_n);
  return 0; 
}